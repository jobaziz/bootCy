/**
 * @file igmp_snooping.c
 * @brief IGMP snooping switch
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.3.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL IGMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "ipv4/ipv4.h"
#include "ipv4/ipv4_misc.h"
#include "igmp/igmp_snooping.h"
#include "igmp/igmp_snooping_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (IPV4_SUPPORT == ENABLED && IGMP_SNOOPING_SUPPORT == ENABLED)


/**
 * @brief Initialize settings with default values
 * @param[out] settings Structure that contains IGMP snooping settings
 **/

void igmpSnoopingGetDefaultSettings(IgmpSnoopingSettings *settings)
{
   //Primary interface on an attached network
   settings->interface = NULL;

   //Nnumber of ports
   settings->numPorts = 0;
   //Ports
   settings->ports = NULL;

   //Maximum number of multicast groups
   settings->numGroups = 0;
   //Multicast groups
   settings->groups = NULL;

   //Flood IGMP report messages to all ports (not only to router ports)
   settings->floodReports = FALSE;
   //Flood unregistered multicast traffic to all ports
   settings->floodUnknownMulticastPackets = TRUE;

   //Leave latency
   settings->lastMemberQueryTime = IGMP_LAST_MEMBER_QUERY_TIME;
}


/**
 * @brief IGMP snooping switch initialization
 * @param[in] context Pointer to the IGMP snooping switch context
 * @param[in] settings IGMP snooping specific settings
 * @return Error code
 **/

error_t igmpSnoopingInit(IgmpSnoopingContext *context,
   const IgmpSnoopingSettings *settings)
{
   uint_t i;
   NetInterface *interface;

   //Debug message
   TRACE_INFO("Initializing IGMP snooping switch...\r\n");

   //Ensure the parameters are valid
   if(context == NULL || settings == NULL)
      return ERROR_INVALID_PARAMETER;

   //The IGMP router must be bound to a valid interface
   if(settings->interface == NULL)
      return ERROR_INVALID_PARAMETER;

   //Sanity check
   if(settings->numPorts < 1 || settings->ports == NULL)
      return ERROR_INVALID_PARAMETER;

   if(settings->numGroups < 1 || settings->groups == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to the underlying network interface
   interface = settings->interface;

   //Clear the IGMP snooping switch context
   osMemset(context, 0, sizeof(IgmpSnoopingContext));

   //Initialize IGMP snooping switch context
   context->interface = settings->interface;
   context->numPorts = settings->numPorts;
   context->ports = settings->ports;
   context->numGroups = settings->numGroups;
   context->groups = settings->groups;
   context->floodReports = settings->floodReports;
   context->floodUnknownMulticastPackets = settings->floodUnknownMulticastPackets;
   context->lastMemberQueryTime = settings->lastMemberQueryTime;

   //Loop through multicast groups
   for(i = 0; i < context->numGroups; i++)
   {
      //The "No Members Present" state, when there are no hosts on the network
      //which have sent reports for this multicast group is the initial state
      //for all groups
      context->groups[i].state = IGMP_SNOOPING_GROUP_STATE_NO_MEMBERS_PRESENT;
   }

   //Attach the IGMP snooping switch context to the network interface
   interface->igmpSnoopingContext = context;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Start IGMP snooping switch
 * @param[in] context Pointer to the IGMP snooping switch context
 * @return Error code
 **/

error_t igmpSnoopingStart(IgmpSnoopingContext *context)
{
   //Make sure the IGMP snooping switch context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Debug message
   TRACE_INFO("Starting IGMP snooping switch...\r\n");

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Enable IGMP monitoring
   igmpSnoopingEnableMonitoring(context, TRUE);

   //The IGMP snooping switch is now running
   context->running = TRUE;

   //If a switch receives an unregistered packet, it must forward that packet
   //on all ports to which an IGMP router is attached. A switch may default to
   //forwarding unregistered packets on all ports (refer to RFC 4541, section
   //2.1.2)
   if(!context->floodUnknownMulticastPackets)
   {
      igmpSnoopingSetUnknownMcastFwdPorts(context, TRUE, 0);
   }
   else
   {
      igmpSnoopingSetUnknownMcastFwdPorts(context, FALSE, 0);
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Stop IGMP snooping switch
 * @param[in] context Pointer to the IGMP snooping switch context
 * @return Error code
 **/

error_t igmpSnoopingStop(IgmpSnoopingContext *context)
{
   uint_t i;

   //Make sure the IGMP snooping switch context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Debug message
   TRACE_INFO("Stopping IGMP snooping switch...\r\n");

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Disable IGMP monitoring
   igmpSnoopingEnableMonitoring(context, FALSE);
   //Flood unregistered multicast traffic to all ports
   igmpSnoopingSetUnknownMcastFwdPorts(context, FALSE, 0);

   //Clear the list of multicast routers
   for(i = 0; i < context->numPorts; i++)
   {
      context->ports[i].routerPresent = FALSE;
   }

   //Clear the list of multicast groups
   for(i = 0; i < context->numGroups; i++)
   {
      //Check whether there are hosts on the network which have sent reports
      //for this multicast group
      if(context->groups[i].state != IGMP_SNOOPING_GROUP_STATE_NO_MEMBERS_PRESENT)
      {
         //Delete the corresponding entry
         igmpSnoopingDeleteGroup(context, &context->groups[i]);
      }
   }

   //The IGMP snooping switch is not running anymore
   context->running = FALSE;

   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief IGMP snooping switch timer handler
 * @param[in] context Pointer to the IGMP snooping switch context
 **/

void igmpSnoopingTick(IgmpSnoopingContext *context)
{
   uint_t i;
   bool_t update;
   uint32_t routerPorts;
   IgmpSnoopingGroup *group;

   //Initialize flag
   update = FALSE;

   //Check whether the IGMP snooping switch is running
   if(context->running)
   {
      //A switch supporting IGMP snooping must maintain a list of multicast
      //routers and the ports on which they are attached
      for(i = 0; i < context->numPorts; i++)
      {
         //Check whether any IGMP router is attached to this port
         if(context->ports[i].routerPresent)
         {
            //If the timer has expired, then the router is no longer present
            if(netTimerExpired(&context->ports[i].timer))
            {
               //This port is no longer considered as a router port
               context->ports[i].routerPresent = FALSE;
               //The list of router ports has changed
               update = TRUE;
            }
         }
      }

      //The snooping switch must update its forwarding table when the list of
      //router ports has changed
      if(update)
      {
         //Loop through multicast groups
         for(i = 0; i < context->numGroups; i++)
         {
            //Point to the current group
            group = &context->groups[i];

            //Check whether there are hosts on the network which have sent reports
            //for this multicast group
            if(group->state != IGMP_SNOOPING_GROUP_STATE_NO_MEMBERS_PRESENT)
            {
               //Update the corresponding entry in forwarding table
               igmpSnoopingUpdateStaticFdbEntry(context, group->addr);
            }
         }

         //Check whether unregistered packets should be forwarded to router
         //ports only
         if(!context->floodUnknownMulticastPackets)
         {
            //Retrieve the port map identifying router ports
            routerPorts = igmpSnoopingGetRouterPorts(context);

            //Forward unknown multicast packets on all ports to which
            //an IGMP router is attached
            igmpSnoopingSetUnknownMcastFwdPorts(context, TRUE, routerPorts);
         }
      }

      //The snooping switch must not rely exclusively on the appearance of IGMP
      //Group Leave announcements to determine when entries should be removed
      //from the forwarding table. It should implement a membership timeout
      //mechanism (refer to RFC 4541, section 2.1.1)
      for(i = 0; i < context->numGroups; i++)
      {
         //Point to the current group
         group = &context->groups[i];

         //Check whether there are hosts on the network which have sent reports
         //for this multicast group
         if(group->state != IGMP_SNOOPING_GROUP_STATE_NO_MEMBERS_PRESENT)
         {
            //Membership timeout?
            if(netTimerExpired(&group->timer))
            {
               //If no reports are received after the response time of the last
               //query expires, the group has no local members
               igmpSnoopingDeleteGroup(context, group);
            }
         }
      }
   }
}

#endif
