/**
 * @file loopback_driver.c
 * @brief Loopback interface driver
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
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "loopback_driver.h"
#include "debug.h"

//Loopback interface queue
static LoopbackDriverQueueEntry queue[LOOPBACK_DRIVER_QUEUE_SIZE];

static uint_t queueLength;
static uint_t queueTxIndex;
static uint_t queueRxIndex;


/**
 * @brief Loopback interface driver
 **/

const NicDriver loopbackDriver =
{
   NIC_TYPE_LOOPBACK,
   ETH_MTU,
   loopbackDriverInit,
   loopbackDriverTick,
   loopbackDriverEnableIrq,
   loopbackDriverDisableIrq,
   loopbackDriverEventHandler,
   loopbackDriverSendPacket,
   loopbackDriverUpdateMacAddrFilter,
   NULL,
   NULL,
   NULL,
   FALSE,
   FALSE,
   FALSE,
   FALSE
};


/**
 * @brief Loopback interface initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t loopbackDriverInit(NetInterface *interface)
{
   //Debug message
   TRACE_INFO("Initializing loopback interface...\r\n");

   //Initialize variables
   queueLength = 0;
   queueTxIndex = 0;
   queueRxIndex = 0;

   //Force the TCP/IP stack to poll the link state at startup
   interface->nicEvent = TRUE;
   osSetEvent(&netEvent);

   //The loopback interface is now ready to send
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Loopback interface timer handler
 *
 * This routine is periodically called by the TCP/IP stack to handle periodic
 * operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void loopbackDriverTick(NetInterface *interface)
{
   //Not implemented
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void loopbackDriverEnableIrq(NetInterface *interface)
{
   //Not implemented
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void loopbackDriverDisableIrq(NetInterface *interface)
{
   //Not implemented
}


/**
 * @brief Loopback interface event handler
 * @param[in] interface Underlying network interface
 **/

void loopbackDriverEventHandler(NetInterface *interface)
{
   //Link up event is pending?
   if(!interface->linkState)
   {
      //Link is up
      interface->linkState = TRUE;
      //Process link state change event
      nicNotifyLinkChange(interface);
   }

   //Read incoming packet
   loopbackDriverReceivePacket(interface);

   //Check whether another packet is pending in the queue
   if(queueLength > 0)
   {
      //Set event flag
      interface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      osSetEvent(&netEvent);
   }
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @param[in] ancillary Additional options passed to the stack along with
 *   the packet
 * @return Error code
 **/

error_t loopbackDriverSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary)
{
   error_t error;
   size_t length;

   //Initialize status code
   error = NO_ERROR;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Valid packet length?
   if(length <= ETH_MTU)
   {
      //Check whether the queue is full
      if(queueLength < LOOPBACK_DRIVER_QUEUE_SIZE)
      {
         //Retrieve the length of the packet
         queue[queueTxIndex].length = netBufferGetLength(buffer) - offset;

         //Copy data to the queue
         netBufferRead(queue[queueTxIndex].data, buffer,
            offset, queue[queueTxIndex].length);

         //Increment index and wrap around if necessary
         if(++queueTxIndex >= LOOPBACK_DRIVER_QUEUE_SIZE)
         {
            queueTxIndex = 0;
         }

         //Update the length of the queue
         queueLength++;

         //Set event flag
         interface->nicEvent = TRUE;
         //Notify the TCP/IP stack of the event
         osSetEvent(&netEvent);
      }
   }
   else
   {
      //Report an error
      error = ERROR_INVALID_LENGTH;
   }

   //The transmitter can accept another packet
   osSetEvent(&interface->nicTxEvent);

   //Return status code
   return error;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t loopbackDriverReceivePacket(NetInterface *interface)
{
   error_t error;
   NetRxAncillary ancillary;

   //Check whether a packet is pending in the queue
   if(queueLength > 0)
   {
      //Additional options can be passed to the stack along with the packet
      ancillary = NET_DEFAULT_RX_ANCILLARY;

      //Pass the packet to the upper layer
      nicProcessPacket(interface, queue[queueRxIndex].data,
         queue[queueRxIndex].length, &ancillary);

      //Increment index and wrap around if necessary
      if(++queueRxIndex >= LOOPBACK_DRIVER_QUEUE_SIZE)
      {
         queueRxIndex = 0;
      }

      //Update the length of the queue
      queueLength--;

      //Packet successfully received
      error = NO_ERROR;
   }
   else
   {
      //No more packet in the queue
      error = ERROR_BUFFER_EMPTY;
   }

   //Return status code
   return error;
}


/**
 * @brief Configure MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t loopbackDriverUpdateMacAddrFilter(NetInterface *interface)
{
   //Not implemented
   return NO_ERROR;
}
