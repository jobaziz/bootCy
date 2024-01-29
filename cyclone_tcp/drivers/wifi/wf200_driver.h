/**
 * @file wf200_driver.h
 * @brief WF200 Wi-Fi controller
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

#ifndef _WF200_DRIVER_H
#define _WF200_DRIVER_H

//Dependencies
#include "core/nic.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//WF200 driver (STA mode)
extern const NicDriver wf200StaDriver;
//WF200 driver (AP mode)
extern const NicDriver wf200ApDriver;

//WF200 related functions
error_t wf200Init(NetInterface *interface);

void wf200Tick(NetInterface *interface);

void wf200EnableIrq(NetInterface *interface);
void wf200DisableIrq(NetInterface *interface);
void wf200EventHandler(NetInterface *interface);

error_t wf200SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t wf200UpdateMacAddrFilter(NetInterface *interface);

void wf200ConnectCallback(void);
void wf200DisconnectCallback(void);
void wf200StartApCallback(void);
void wf200StopApCallback(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
