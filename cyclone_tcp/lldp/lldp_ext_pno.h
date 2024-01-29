/**
 * @file lldp_ext_pno.h
 * @brief PROFINET extension
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

#ifndef _LLDP_EXT_PNO_H
#define _LLDP_EXT_PNO_H

//Dependencies
#include "core/net.h"
#include "lldp/lldp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief PROFINET subtypes
 **/

typedef enum
{
   LLDP_PNO_SUBTYPE_RESERVED              = 0, ///<Reserved
   LLDP_PNO_SUBTYPE_MEASURED_DELAY_VALUES = 1, ///<Measured Delay Values
   LLDP_PNO_SUBTYPE_PORT_STATUS           = 2, ///<Port Status
   LLDP_PNO_SUBTYPE_ALIAS                 = 3, ///<Alias
   LLDP_PNO_SUBTYPE_MRP_PORT_STATUS       = 4, ///<MRP Port Status
   LLDP_PNO_SUBTYPE_INTERFACE_MAC_ADDR    = 5, ///<Interface MAC address
   LLDP_PNO_SUBTYPE_PTCP_STATUS           = 6  ///<PTCP Status
} LldpPnoSubtype;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
