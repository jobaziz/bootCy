/**
 * @file gd32f2xx_eth_driver.h
 * @brief GigaDevice GD32F2 Ethernet MAC driver
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

#ifndef _GD32F2XX_ETH_DRIVER_H
#define _GD32F2XX_ETH_DRIVER_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef GD32F2XX_ETH_TX_BUFFER_COUNT
   #define GD32F2XX_ETH_TX_BUFFER_COUNT 3
#elif (GD32F2XX_ETH_TX_BUFFER_COUNT < 1)
   #error GD32F2XX_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef GD32F2XX_ETH_TX_BUFFER_SIZE
   #define GD32F2XX_ETH_TX_BUFFER_SIZE 1536
#elif (GD32F2XX_ETH_TX_BUFFER_SIZE != 1536)
   #error GD32F2XX_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef GD32F2XX_ETH_RX_BUFFER_COUNT
   #define GD32F2XX_ETH_RX_BUFFER_COUNT 6
#elif (GD32F2XX_ETH_RX_BUFFER_COUNT < 1)
   #error GD32F2XX_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef GD32F2XX_ETH_RX_BUFFER_SIZE
   #define GD32F2XX_ETH_RX_BUFFER_SIZE 1536
#elif (GD32F2XX_ETH_RX_BUFFER_SIZE != 1536)
   #error GD32F2XX_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Interrupt priority grouping
#ifndef GD32F2XX_ETH_IRQ_PRIORITY_GROUPING
   #define GD32F2XX_ETH_IRQ_PRIORITY_GROUPING 3
#elif (GD32F2XX_ETH_IRQ_PRIORITY_GROUPING < 0)
   #error GD32F2XX_ETH_IRQ_PRIORITY_GROUPING parameter is not valid
#endif

//Ethernet interrupt group priority
#ifndef GD32F2XX_ETH_IRQ_GROUP_PRIORITY
   #define GD32F2XX_ETH_IRQ_GROUP_PRIORITY 12
#elif (GD32F2XX_ETH_IRQ_GROUP_PRIORITY < 0)
   #error GD32F2XX_ETH_IRQ_GROUP_PRIORITY parameter is not valid
#endif

//Ethernet interrupt subpriority
#ifndef GD32F2XX_ETH_IRQ_SUB_PRIORITY
   #define GD32F2XX_ETH_IRQ_SUB_PRIORITY 0
#elif (GD32F2XX_ETH_IRQ_SUB_PRIORITY < 0)
   #error GD32F2XX_ETH_IRQ_SUB_PRIORITY parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enhanced TX DMA descriptor
 **/

typedef struct
{
   uint32_t tdes0;
   uint32_t tdes1;
   uint32_t tdes2;
   uint32_t tdes3;
} Gd32f2xxTxDmaDesc;


/**
 * @brief Enhanced RX DMA descriptor
 **/

typedef struct
{
   uint32_t rdes0;
   uint32_t rdes1;
   uint32_t rdes2;
   uint32_t rdes3;
} Gd32f2xxRxDmaDesc;


//GD32F2XX Ethernet MAC driver
extern const NicDriver gd32f2xxEthDriver;

//GD32F2XX Ethernet MAC related functions
error_t gd32f2xxEthInit(NetInterface *interface);
void gd32f2xxEthInitGpio(NetInterface *interface);
void gd32f2xxEthInitDmaDesc(NetInterface *interface);

void gd32f2xxEthTick(NetInterface *interface);

void gd32f2xxEthEnableIrq(NetInterface *interface);
void gd32f2xxEthDisableIrq(NetInterface *interface);
void gd32f2xxEthEventHandler(NetInterface *interface);

error_t gd32f2xxEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t gd32f2xxEthReceivePacket(NetInterface *interface);

error_t gd32f2xxEthUpdateMacAddrFilter(NetInterface *interface);
error_t gd32f2xxEthUpdateMacConfig(NetInterface *interface);

void gd32f2xxEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t gd32f2xxEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

uint32_t gd32f2xxEthCalcCrc(const void *data, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
