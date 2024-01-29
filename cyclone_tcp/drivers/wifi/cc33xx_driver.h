/**
 * @file cc33xx_driver.h
 * @brief CC3300/CC3301 Wi-Fi controller
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

#ifndef _CC33XX_DRIVER_H
#define _CC33XX_DRIVER_H

//Dependencies
#include "core/nic.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CC33xx driver (STA mode)
extern const NicDriver cc33xxStaDriver;
//CC33xx driver (AP mode)
extern const NicDriver cc33xxApDriver;

//CC33xx related functions
error_t cc33xxInit(NetInterface *interface);

void cc33xxTick(NetInterface *interface);

void cc33xxEnableIrq(NetInterface *interface);
void cc33xxDisableIrq(NetInterface *interface);
void cc33xxEventHandler(NetInterface *interface);

error_t cc33xxSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t cc33xxUpdateMacAddrFilter(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
