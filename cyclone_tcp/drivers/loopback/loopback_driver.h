/**
 * @file loopback_driver.h
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

#ifndef _LOOPBACK_DRIVER_H
#define _LOOPBACK_DRIVER_H

//Dependencies
#include "core/nic.h"

//Queue size
#ifndef LOOPBACK_DRIVER_QUEUE_SIZE
   #define LOOPBACK_DRIVER_QUEUE_SIZE 6
#elif (LOOPBACK_DRIVER_QUEUE_SIZE < 1)
   #error LOOPBACK_DRIVER_QUEUE_SIZE parameter is not valid
#endif


/**
 * @brief Loopback interface queue entry
 **/

typedef struct
{
   size_t length;
   uint8_t data[ETH_MTU];
} LoopbackDriverQueueEntry;


//Loopback interface driver
extern const NicDriver loopbackDriver;

//Loopback interface related functions
error_t loopbackDriverInit(NetInterface *interface);

void loopbackDriverTick(NetInterface *interface);

void loopbackDriverEnableIrq(NetInterface *interface);
void loopbackDriverDisableIrq(NetInterface *interface);
void loopbackDriverEventHandler(NetInterface *interface);

error_t loopbackDriverSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t loopbackDriverReceivePacket(NetInterface *interface);

error_t loopbackDriverUpdateMacAddrFilter(NetInterface *interface);

#endif
