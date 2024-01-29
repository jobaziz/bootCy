/**
 * @file winc3400_driver.h
 * @brief WINC3400 Wi-Fi controller
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

#ifndef _WINC3400_DRIVER_H
#define _WINC3400_DRIVER_H

//Dependencies
#include "core/nic.h"

//TX buffer size
#ifndef WINC3400_TX_BUFFER_SIZE
   #define WINC3400_TX_BUFFER_SIZE 1600
#elif (WINC3400_TX_BUFFER_SIZE != 1600)
   #error WINC3400_TX_BUFFER_SIZE parameter is not valid
#endif

//RX buffer size
#ifndef WINC3400_RX_BUFFER_SIZE
   #define WINC3400_RX_BUFFER_SIZE 1600
#elif (WINC3400_RX_BUFFER_SIZE != 1600)
   #error WINC3400_RX_BUFFER_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//WINC3400 driver
extern const NicDriver winc3400Driver;

//WINC3400 related functions
error_t winc3400Init(NetInterface *interface);

void winc3400Tick(NetInterface *interface);

void winc3400EnableIrq(NetInterface *interface);
void winc3400DisableIrq(NetInterface *interface);
bool_t winc3400IrqHandler(void);
void winc3400EventHandler(NetInterface *interface);

error_t winc3400SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t winc3400UpdateMacAddrFilter(NetInterface *interface);

void winc3400AppWifiEvent(uint8_t msgType, void *msg);
void winc3400AppEthEvent(uint8_t msgType, void *msg, void *ctrlBuf);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
