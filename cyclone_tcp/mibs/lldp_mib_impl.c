/**
 * @file lldp_mib_impl.c
 * @brief LLDP MIB module implementation
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
#include "mibs/lldp_mib_module.h"
#include "mibs/lldp_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "lldp/lldp_mgmt.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (LLDP_MIB_SUPPORT == ENABLED)


/**
 * @brief LLDP MIB module initialization
 * @return Error code
 **/

error_t lldpMibInit(void)
{
   //Debug message
   TRACE_INFO("Initializing LLDP MIB base...\r\n");

   //Clear LLDP MIB base
   memset(&lldpMibBase, 0, sizeof(lldpMibBase));

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Lock LLDP MIB base
 **/

void lldpMibLock(void)
{
   //Acquire exclusive access to the LLDP agent context
   lldpMgmtLock(lldpMibBase.lldpAgentContext);
}


/**
 * @brief Unlock LLDP MIB base
 **/

void lldpMibUnlock(void)
{
   //Release exclusive access to the LLDP agent context
   lldpMgmtUnlock(lldpMibBase.lldpAgentContext);
}


/**
 * @brief Attach LLDP agent context
 * @param[in] context Pointer to the LLDP agent context
 * @return Error code
 **/

error_t lldpMibSetLldpAgentContext(LldpAgentContext *context)
{
   //Attach LLDP agent context
   lldpMibBase.lldpAgentContext = context;

   //Successful processing
   return NO_ERROR;
}

#endif
