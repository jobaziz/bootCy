/**
 * @file rndis_driver.h
 * @brief RNDIS driver
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

#ifndef _RNDIS_DRIVER_H
#define _RNDIS_DRIVER_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef RNDIS_TX_BUFFER_COUNT
   #define RNDIS_TX_BUFFER_COUNT 2
#elif (RNDIS_TX_BUFFER_COUNT < 1)
   #error RNDIS_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef RNDIS_TX_BUFFER_SIZE
   #define RNDIS_TX_BUFFER_SIZE 2048
#elif (RNDIS_TX_BUFFER_SIZE != 2048)
   #error RNDIS_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef RNDIS_RX_BUFFER_COUNT
   #define RNDIS_RX_BUFFER_COUNT 1
#elif (RNDIS_RX_BUFFER_COUNT < 1)
   #error RNDIS_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef RNDIS_RX_BUFFER_SIZE
   #define RNDIS_RX_BUFFER_SIZE 2048
#elif (RNDIS_RX_BUFFER_SIZE != 2048)
   #error RNDIS_RX_BUFFER_SIZE parameter is not valid
#endif


/**
 * @brief TX buffer descriptor
 **/

typedef struct
{
   bool_t ready;
   size_t length;
   uint8_t data[RNDIS_TX_BUFFER_SIZE];
} RndisTxBufferDesc;


/**
 * @brief RX buffer descriptor
 **/

typedef struct
{
   bool_t ready;
   size_t length;
   uint8_t data[RNDIS_RX_BUFFER_SIZE];
} RndisRxBufferDesc;


//RNDIS driver
extern const NicDriver rndisDriver;
//Underlying network interface
extern NetInterface *rndisDriverInterface;

//TX and RX buffers
extern RndisTxBufferDesc rndisTxBuffer[RNDIS_TX_BUFFER_COUNT];
extern RndisRxBufferDesc rndisRxBuffer[RNDIS_RX_BUFFER_COUNT];

//Buffer indexes
extern uint_t rndisTxWriteIndex;
extern uint_t rndisTxReadIndex;
extern uint_t rndisRxWriteIndex;
extern uint_t rndisRxReadIndex;

//RNDIS driver related functions
error_t rndisDriverInit(NetInterface *interface);

void rndisDriverTick(NetInterface *interface);

void rndisDriverEnableIrq(NetInterface *interface);
void rndisDriverDisableIrq(NetInterface *interface);
void rndisDriverEventHandler(NetInterface *interface);

error_t rndisDriverSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t rndisDriverReceivePacket(NetInterface *interface,
   uint8_t *buffer, size_t size, size_t *length);

error_t rndisDriverSetMulticastFilter(NetInterface *interface);

#endif
