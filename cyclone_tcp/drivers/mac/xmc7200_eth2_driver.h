/**
 * @file xmc7200_eth2_driver.h
 * @brief XMC7200 Ethernet MAC driver (ETH1 instance)
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

#ifndef _XMC7200_ETH2_DRIVER_H
#define _XMC7200_ETH2_DRIVER_H

//Number of TX buffers
#ifndef XMC7200_ETH2_TX_BUFFER_COUNT
   #define XMC7200_ETH2_TX_BUFFER_COUNT 4
#elif (XMC7200_ETH2_TX_BUFFER_COUNT < 1)
   #error XMC7200_ETH2_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef XMC7200_ETH2_TX_BUFFER_SIZE
   #define XMC7200_ETH2_TX_BUFFER_SIZE 1536
#elif (XMC7200_ETH2_TX_BUFFER_SIZE != 1536)
   #error XMC7200_ETH2_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef XMC7200_ETH2_RX_BUFFER_COUNT
   #define XMC7200_ETH2_RX_BUFFER_COUNT 96
#elif (XMC7200_ETH2_RX_BUFFER_COUNT < 12)
   #error XMC7200_ETH2_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef XMC7200_ETH2_RX_BUFFER_SIZE
   #define XMC7200_ETH2_RX_BUFFER_SIZE 128
#elif (XMC7200_ETH2_RX_BUFFER_SIZE != 128)
   #error XMC7200_ETH2_RX_BUFFER_SIZE parameter is not valid
#endif

//Number of dummy buffers
#ifndef XMC7200_ETH2_DUMMY_BUFFER_COUNT
   #define XMC7200_ETH2_DUMMY_BUFFER_COUNT 2
#elif (XMC7200_ETH2_DUMMY_BUFFER_COUNT < 1)
   #error XMC7200_ETH2_DUMMY_BUFFER_COUNT parameter is not valid
#endif

//Dummy buffer size
#ifndef XMC7200_ETH2_DUMMY_BUFFER_SIZE
   #define XMC7200_ETH2_DUMMY_BUFFER_SIZE 128
#elif (XMC7200_ETH2_DUMMY_BUFFER_SIZE != 128)
   #error XMC7200_ETH2_DUMMY_BUFFER_SIZE parameter is not valid
#endif

//Interrupt priority grouping
#ifndef XMC7200_ETH2_IRQ_PRIORITY_GROUPING
   #define XMC7200_ETH2_IRQ_PRIORITY_GROUPING 4
#elif (XMC7200_ETH2_IRQ_PRIORITY_GROUPING < 0)
   #error XMC7200_ETH2_IRQ_PRIORITY_GROUPING parameter is not valid
#endif

//Ethernet interrupt group priority
#ifndef XMC7200_ETH2_IRQ_GROUP_PRIORITY
   #define XMC7200_ETH2_IRQ_GROUP_PRIORITY 6
#elif (XMC7200_ETH2_IRQ_GROUP_PRIORITY < 0)
   #error XMC7200_ETH2_IRQ_GROUP_PRIORITY parameter is not valid
#endif

//Ethernet interrupt subpriority
#ifndef XMC7200_ETH2_IRQ_SUB_PRIORITY
   #define XMC7200_ETH2_IRQ_SUB_PRIORITY 0
#elif (XMC7200_ETH2_IRQ_SUB_PRIORITY < 0)
   #error XMC7200_ETH2_IRQ_SUB_PRIORITY parameter is not valid
#endif

//Name of the section where to place DMA buffers
#ifndef XMC7200_ETH2_RAM_SECTION
   #define XMC7200_ETH2_RAM_SECTION ".ram_no_cache"
#endif

//TX buffer descriptor flags
#define ETH_TX_USED           0x80000000
#define ETH_TX_WRAP           0x40000000
#define ETH_TX_RLE_ERROR      0x20000000
#define ETH_TX_UNDERRUN_ERROR 0x10000000
#define ETH_TX_AHB_ERROR      0x08000000
#define ETH_TX_LATE_COL_ERROR 0x04000000
#define ETH_TX_CHECKSUM_ERROR 0x00700000
#define ETH_TX_NO_CRC         0x00010000
#define ETH_TX_LAST           0x00008000
#define ETH_TX_LENGTH         0x00003FFF

//RX buffer descriptor flags
#define ETH_RX_ADDRESS        0xFFFFFFFC
#define ETH_RX_WRAP           0x00000002
#define ETH_RX_OWNERSHIP      0x00000001
#define ETH_RX_BROADCAST      0x80000000
#define ETH_RX_MULTICAST_HASH 0x40000000
#define ETH_RX_UNICAST_HASH   0x20000000
#define ETH_RX_SAR            0x08000000
#define ETH_RX_SAR_MASK       0x06000000
#define ETH_RX_TYPE_ID        0x01000000
#define ETH_RX_SNAP           0x01000000
#define ETH_RX_TYPE_ID_MASK   0x00C00000
#define ETH_RX_CHECKSUM_VALID 0x00C00000
#define ETH_RX_VLAN_TAG       0x00200000
#define ETH_RX_PRIORITY_TAG   0x00100000
#define ETH_RX_VLAN_PRIORITY  0x000E0000
#define ETH_RX_CFI            0x00010000
#define ETH_RX_EOF            0x00008000
#define ETH_RX_SOF            0x00004000
#define ETH_RX_LENGTH_MSB     0x00002000
#define ETH_RX_BAD_FCS        0x00002000
#define ETH_RX_LENGTH         0x00001FFF

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Transmit buffer descriptor
 **/

typedef struct
{
   uint32_t address;
   uint32_t status;
} Xmc7200Eth2TxBufferDesc;


/**
 * @brief Receive buffer descriptor
 **/

typedef struct
{
   uint32_t address;
   uint32_t status;
} Xmc7200Eth2RxBufferDesc;


//XMC7200 Ethernet MAC driver (ETH1 instance)
extern const NicDriver xmc7200Eth2Driver;

//XMC7200 Ethernet MAC related functions
error_t xmc7200Eth2Init(NetInterface *interface);
void xmc7200Eth2InitGpio(NetInterface *interface);
void xmc7200Eth2InitBufferDesc(NetInterface *interface);

void xmc7200Eth2Tick(NetInterface *interface);

void xmc7200Eth2EnableIrq(NetInterface *interface);
void xmc7200Eth2DisableIrq(NetInterface *interface);
void xmc7200Eth2EventHandler(NetInterface *interface);

error_t xmc7200Eth2SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t xmc7200Eth2ReceivePacket(NetInterface *interface);

error_t xmc7200Eth2UpdateMacAddrFilter(NetInterface *interface);
error_t xmc7200Eth2UpdateMacConfig(NetInterface *interface);

void xmc7200Eth2WritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t xmc7200Eth2ReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
