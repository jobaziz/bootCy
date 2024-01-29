/**
 * @file pic32mz_eth_driver.h
 * @brief PIC32MZ Ethernet MAC driver
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

#ifndef _PIC32MZ_ETH_DRIVER_H
#define _PIC32MZ_ETH_DRIVER_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef PIC32MZ_ETH_TX_BUFFER_COUNT
   #define PIC32MZ_ETH_TX_BUFFER_COUNT 3
#elif (PIC32MZ_ETH_TX_BUFFER_COUNT < 1)
   #error PIC32MZ_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef PIC32MZ_ETH_TX_BUFFER_SIZE
   #define PIC32MZ_ETH_TX_BUFFER_SIZE 1536
#elif (PIC32MZ_ETH_TX_BUFFER_SIZE != 1536)
   #error PIC32MZ_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef PIC32MZ_ETH_RX_BUFFER_COUNT
   #define PIC32MZ_ETH_RX_BUFFER_COUNT 6
#elif (PIC32MZ_ETH_RX_BUFFER_COUNT < 1)
   #error PIC32MZ_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef PIC32MZ_ETH_RX_BUFFER_SIZE
   #define PIC32MZ_ETH_RX_BUFFER_SIZE 1536
#elif (PIC32MZ_ETH_RX_BUFFER_SIZE != 1536)
   #error PIC32MZ_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Ethernet interrupt priority
#ifndef PIC32MZ_ETH_IRQ_PRIORITY
   #define PIC32MZ_ETH_IRQ_PRIORITY 2
#elif (PIC32MZ_ETH_IRQ_PRIORITY < 0)
   #error PIC32MZ_ETH_IRQ_PRIORITY parameter is not valid
#endif

//Ethernet interrupt subpriority
#ifndef PIC32MZ_ETH_IRQ_SUB_PRIORITY
   #define PIC32MZ_ETH_IRQ_SUB_PRIORITY 0
#elif (PIC32MZ_ETH_IRQ_SUB_PRIORITY < 0)
   #error PIC32MZ_ETH_IRQ_SUB_PRIORITY parameter is not valid
#endif

//EMAC1MCFG register
#define _EMAC1MCFG_CLKSEL_DIV4  (0 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV6  (2 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV8  (3 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV10 (4 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV14 (5 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV20 (6 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV28 (7 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV40 (8 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV48 (9 << _EMAC1MCFG_CLKSEL_POSITION)
#define _EMAC1MCFG_CLKSEL_DIV50 (10 << _EMAC1MCFG_CLKSEL_POSITION)

//Transmit buffer descriptor flags
#define ETH_TX_CTRL_SOP                   0x80000000
#define ETH_TX_CTRL_EOP                   0x40000000
#define ETH_TX_CTRL_BYTE_COUNT            0x07FF0000
#define ETH_TX_CTRL_NPV                   0x00000100
#define ETH_TX_CTRL_EOWN                  0x00000080
#define ETH_TX_STATUS1_VLAN               0x00080000
#define ETH_TX_STATUS1_BACKPRESSURE       0x00040000
#define ETH_TX_STATUS1_PAUSE              0x00020000
#define ETH_TX_STATUS1_CONTROL            0x00010000
#define ETH_TX_STATUS1_TOTAL_BYTES        0x0000FFFF
#define ETH_TX_STATUS2_UNDERRUN           0x80000000
#define ETH_TX_STATUS2_GIANT              0x40000000
#define ETH_TX_STATUS2_LATE_COL           0x20000000
#define ETH_TX_STATUS2_MAX_COL            0x10000000
#define ETH_TX_STATUS2_EXCESSIVE_DEFER    0x08000000
#define ETH_TX_STATUS2_PACKET_DEFER       0x04000000
#define ETH_TX_STATUS2_BROADCAST          0x02000000
#define ETH_TX_STATUS2_MULTICAST          0x01000000
#define ETH_TX_STATUS2_DONE               0x00800000
#define ETH_TX_STATUS2_LEN_OUT_OF_RANGE   0x00400000
#define ETH_TX_STATUS2_LEN_CHECK_ERROR    0x00200000
#define ETH_TX_STATUS2_CRC_ERROR          0x00100000
#define ETH_TX_STATUS2_COL_COUNT          0x000F0000
#define ETH_TX_STATUS2_BYTE_COUNT         0x0000FFFF

//Receive buffer descriptor flags
#define ETH_RX_CTRL_SOP                   0x80000000
#define ETH_RX_CTRL_EOP                   0x40000000
#define ETH_RX_CTRL_BYTE_COUNT            0x07FF0000
#define ETH_RX_CTRL_NPV                   0x00000100
#define ETH_RX_CTRL_EOWN                  0x00000080
#define ETH_RX_STATUS1_MULTICAST_MATCH    0x80000000
#define ETH_RX_STATUS1_BROADCAST_MATCH    0x40000000
#define ETH_RX_STATUS1_UNICAST_MATCH      0x20000000
#define ETH_RX_STATUS1_PATTERN_MATCH      0x10000000
#define ETH_RX_STATUS1_MAGIC_PACKET_MATCH 0x08000000
#define ETH_RX_STATUS1_HASH_TABLE_MATCH   0x04000000
#define ETH_RX_STATUS1_NOT_MATCH          0x02000000
#define ETH_RX_STATUS1_RUNT_PACKET        0x01000000
#define ETH_RX_STATUS1_PACKET_CHECKSUM    0x0000FFFF
#define ETH_RX_STATUS2_VLAN               0x40000000
#define ETH_RX_STATUS2_UNKNOWN_OP_CODE    0x20000000
#define ETH_RX_STATUS2_PAUSE              0x10000000
#define ETH_RX_STATUS2_CONTROL            0x08000000
#define ETH_RX_STATUS2_DRIBBLE_NIBBLE     0x04000000
#define ETH_RX_STATUS2_BROADCAST          0x02000000
#define ETH_RX_STATUS2_MULTICAST          0x01000000
#define ETH_RX_STATUS2_OK                 0x00800000
#define ETH_RX_STATUS2_LEN_OUT_OF_RANGE   0x00400000
#define ETH_RX_STATUS2_LEN_CHECK_ERROR    0x00200000
#define ETH_RX_STATUS2_CRC_ERROR          0x00100000
#define ETH_RX_STATUS2_CODE_VIOLATION     0x00080000
#define ETH_RX_STATUS2_CARRIER_EVENT      0x00040000
#define ETH_RX_STATUS2_RXDV_EVENT         0x00020000
#define ETH_RX_STATUS2_LONG_EVENT         0x00010000
#define ETH_RX_STATUS2_BYTE_COUNT         0x0000FFFF

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief TX buffer descriptor
 **/

typedef struct
{
   uint32_t control;
   uint32_t address;
   uint32_t status1;
   uint32_t status2;
   uint32_t next;
} Pic32mzTxBufferDesc;


/**
 * @brief RX buffer descriptor
 **/

typedef struct
{
   uint32_t control;
   uint32_t address;
   uint32_t status1;
   uint32_t status2;
   uint32_t next;
} Pic32mzRxBufferDesc;


//PIC32MZ Ethernet MAC driver
extern const NicDriver pic32mzEthDriver;

//PIC32MZ Ethernet MAC related functions
error_t pic32mzEthInit(NetInterface *interface);
void pic32mzEthInitGpio(NetInterface *interface);
void pic32mzEthInitBufferDesc(NetInterface *interface);

void pic32mzEthTick(NetInterface *interface);

void pic32mzEthEnableIrq(NetInterface *interface);
void pic32mzEthDisableIrq(NetInterface *interface);
void pic32mzEthIrqHandler(void);
void pic32mzEthEventHandler(NetInterface *interface);

error_t pic32mzEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t pic32mzEthReceivePacket(NetInterface *interface);

error_t pic32mzEthUpdateMacAddrFilter(NetInterface *interface);
error_t pic32mzEthUpdateMacConfig(NetInterface *interface);

void pic32mzEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t pic32mzEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

uint32_t pic32mzEthCalcCrc(const void *data, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
