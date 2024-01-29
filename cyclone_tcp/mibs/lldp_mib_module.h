/**
 * @file lldp_mib_module.h
 * @brief LLDP MIB module
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

#ifndef _LLDP_MIB_MODULE_H
#define _LLDP_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
#include "lldp/lldp.h"

//LLDP MIB module support
#ifndef LLDP_MIB_SUPPORT
   #define LLDP_MIB_SUPPORT DISABLED
#elif (LLDP_MIB_SUPPORT != ENABLED && LLDP_MIB_SUPPORT != DISABLED)
   #error LLDP_MIB_SUPPORT parameter is not valid
#endif

//Support for SET operations
#ifndef LLDP_MIB_SET_SUPPORT
   #define LLDP_MIB_SET_SUPPORT DISABLED
#elif (LLDP_MIB_SET_SUPPORT != ENABLED && LLDP_MIB_SET_SUPPORT != DISABLED)
   #error LLDP_MIB_SET_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Administrative status
 **/

typedef enum
{
   LLDP_MIB_ADMIN_STATUS_INVALID         = 0,
   LLDP_MIB_ADMIN_STATUS_ENABLED_TX_ONLY = 1,
   LLDP_MIB_ADMIN_STATUS_ENABLED_RX_ONLY = 2,
   LLDP_MIB_ADMIN_STATUS_ENABLED_TX_RX   = 3,
   LLDP_MIB_ADMIN_STATUS_DISABLED        = 4
} LldpMibAdminStatus;


/**
 * @brief Type of interface associated with a management address
 **/

typedef enum
{
   LLDP_MIB_MAN_ADDR_IF_SUBTYPE_UNKNOWN      = 1, ///<Unknown
   LLDP_MIB_MAN_ADDR_IF_SUBTYPE_IF_INDEX     = 2, ///<Interface index
   LLDP_MIB_MAN_ADDR_IF_SUBTYPE_SYS_PORT_NUM = 3  ///<System port number
} LldpMibManAddrIfSubtype;


/**
 * @brief LLDP MIB base
 **/

typedef struct
{
   LldpAgentContext *lldpAgentContext;
} LldpMibBase;


//LLDP MIB related constants
extern LldpMibBase lldpMibBase;
extern const MibObject lldpMibObjects[];
extern const MibModule lldpMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
