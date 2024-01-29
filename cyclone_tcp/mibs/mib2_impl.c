/**
 * @file mib2_impl.c
 * @brief MIB-II module implementation
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
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/mib2_module.h"
#include "mibs/mib2_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (MIB2_SUPPORT == ENABLED)


/**
 * @brief MIB-II module initialization
 * @return Error code
 **/

error_t mib2Init(void)
{
   //Debug message
   TRACE_INFO("Initializing MIB-II base...\r\n");

   //Clear MIB-II base
   osMemset(&mib2Base, 0, sizeof(mib2Base));

#if (MIB2_SYS_GROUP_SUPPORT == ENABLED)
   //System group initialization
   mib2InitSysGroup(&mib2Base.sysGroup);
#endif

#if (MIB2_IF_GROUP_SUPPORT == ENABLED)
   //Interface group initialization
   mib2InitIfGroup(&mib2Base.ifGroup);
#endif

#if (MIB2_IP_GROUP_SUPPORT == ENABLED)
   //IP group initialization
   mib2InitIpGroup(&mib2Base.ipGroup);
#endif

#if (MIB2_TCP_GROUP_SUPPORT == ENABLED)
   //TCP group initialization
   mib2InitTcpGroup(&mib2Base.tcpGroup);
#endif

#if (MIB2_SNMP_GROUP_SUPPORT == ENABLED)
   //SNMP group initialization
   mib2InitSnmpGroup(&mib2Base.snmpGroup);
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief System group initialization
 * @param[in] sysGroup Pointer to the System group
 **/

void mib2InitSysGroup(Mib2SysGroup *sysGroup)
{
#if (MIB2_SYS_DESCR_SIZE > 0)
   //sysDescr object
   osStrcpy(sysGroup->sysDescr, "Description");
   sysGroup->sysDescrLen = osStrlen(sysGroup->sysDescr);
#endif

#if (MIB2_SYS_OBJECT_ID_SIZE > 0)
   //sysObjectID object
   sysGroup->sysObjectID[0] = 0;
   sysGroup->sysObjectIDLen = 1;
#endif

#if (MIB2_SYS_CONTACT_SIZE > 0)
   //sysContact object
   osStrcpy(sysGroup->sysContact, "Contact");
   sysGroup->sysContactLen = osStrlen(sysGroup->sysContact);
#endif

#if (MIB2_SYS_NAME_SIZE > 0)
   //sysName object
   osStrcpy(sysGroup->sysName, "Name");
   sysGroup->sysNameLen = osStrlen(sysGroup->sysName);
#endif

#if (MIB2_SYS_LOCATION_SIZE > 0)
   //sysLocation object
   osStrcpy(sysGroup->sysLocation, "Location");
   sysGroup->sysLocationLen = osStrlen(sysGroup->sysLocation);
#endif

   //sysServices object
   sysGroup->sysServices = MIB2_SYS_SERVICE_INTERNET;
}


/**
 * @brief Interface group initialization
 * @param[in] ifGroup Pointer to the Interface group
 **/

void mib2InitIfGroup(Mib2IfGroup *ifGroup)
{
   uint_t i;

   //ifNumber object
   ifGroup->ifNumber = NET_INTERFACE_COUNT;

   //Interfaces table entry
   for(i = 0; i < NET_INTERFACE_COUNT; i++)
   {
      //ifSpecific object
      ifGroup->ifTable[i].ifSpecific[0] = 0;
      ifGroup->ifTable[i].ifSpecificLen = 1;
   }
}


/**
 * @brief IP group initialization
 * @param[in] ipGroup Pointer to the IP group
 **/

void mib2InitIpGroup(Mib2IpGroup *ipGroup)
{
   //ipForwarding object
   ipGroup->ipForwarding = MIB2_IP_FORWARDING_DISABLED;
   //ipDefaultTTL object
   ipGroup->ipDefaultTTL = IPV4_DEFAULT_TTL;
   //ipReasmTimeout object
   ipGroup->ipReasmTimeout = IPV4_FRAG_TIME_TO_LIVE / 1000;
}


/**
 * @brief TCP group initialization
 * @param[in] tcpGroup Pointer to the TCP group
 **/

void mib2InitTcpGroup(Mib2TcpGroup *tcpGroup)
{
   //tcpRtoAlgorithm object
   tcpGroup->tcpRtoAlgorithm = MIB2_TCP_RTO_ALGORITHM_VANJ;
   //tcpRtoMin object
   tcpGroup->tcpRtoMin = TCP_MIN_RTO;
   //tcpRtoMax object
   tcpGroup->tcpRtoMax = TCP_MAX_RTO;
   //tcpMaxConn object
   tcpGroup->tcpMaxConn = SOCKET_MAX_COUNT;
}


/**
 * @brief SNMP group initialization
 * @param[in] snmpGroup Pointer to the SNMP group
 **/

void mib2InitSnmpGroup(Mib2SnmpGroup *snmpGroup)
{
   //snmpEnableAuthenTraps object
   snmpGroup->snmpEnableAuthenTraps = MIB2_AUTHEN_TRAPS_DISABLED;
}

#endif
