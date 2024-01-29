/**
 * @file rza2_eth1_driver.h
 * @brief RZ/A2 Ethernet MAC driver (ETHERC0 instance)
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

#ifndef _RZA2_ETH1_DRIVER_H
#define _RZA2_ETH1_DRIVER_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef RZA2_ETH1_TX_BUFFER_COUNT
   #define RZA2_ETH1_TX_BUFFER_COUNT 8
#elif (RZA2_ETH1_TX_BUFFER_COUNT < 1)
   #error RZA2_ETH1_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef RZA2_ETH1_TX_BUFFER_SIZE
   #define RZA2_ETH1_TX_BUFFER_SIZE 1536
#elif (RZA2_ETH1_TX_BUFFER_SIZE != 1536)
   #error RZA2_ETH1_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef RZA2_ETH1_RX_BUFFER_COUNT
   #define RZA2_ETH1_RX_BUFFER_COUNT 8
#elif (RZA2_ETH1_RX_BUFFER_COUNT < 1)
   #error RZA2_ETH1_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef RZA2_ETH1_RX_BUFFER_SIZE
   #define RZA2_ETH1_RX_BUFFER_SIZE 1536
#elif (RZA2_ETH1_RX_BUFFER_SIZE != 1536)
   #error RZA2_ETH1_RX_BUFFER_SIZE parameter is not valid
#endif

//Ethernet interrupt priority
#ifndef RZA2_ETH1_IRQ_PRIORITY
   #define RZA2_ETH1_IRQ_PRIORITY 25
#elif (RZA2_ETH1_IRQ_PRIORITY < 0)
   #error RZA2_ETH1_IRQ_PRIORITY parameter is not valid
#endif

//Name of the section where to place DMA buffers
#ifndef RZA2_ETH1_RAM_SECTION
   #define RZA2_ETH1_RAM_SECTION ".BSS_DMAC_SAMPLE_INTERNAL_RAM"
#endif

//Get physical address
#ifndef RZA2_ETH1_GET_PHYSICAL_ADDR
   #define RZA2_ETH1_GET_PHYSICAL_ADDR(addr) ((uint32_t) (addr) - 0x02000000U)
#endif

//EDMR register
#define EDMAC_EDMR_DL_16     0x00000000
#define EDMAC_EDMR_DL_32     0x00000010
#define EDMAC_EDMR_DL_64     0x00000020

//FDR register
#define EDMAC_FDR_TFD_2048   0x00000700
#define EDMAC_FDR_RFD_4096   0x0000000F

//Transmit DMA descriptor flags
#define EDMAC_TD0_TACT       0x80000000
#define EDMAC_TD0_TDLE       0x40000000
#define EDMAC_TD0_TFP_SOF    0x20000000
#define EDMAC_TD0_TFP_EOF    0x10000000
#define EDMAC_TD0_TFE        0x08000000
#define EDMAC_TD0_TWBI       0x04000000
#define EDMAC_TD0_TFS_MASK   0x0000010F
#define EDMAC_TD0_TFS_TABT   0x00000100
#define EDMAC_TD0_TFS_CND    0x00000008
#define EDMAC_TD0_TFS_DLC    0x00000004
#define EDMAC_TD0_TFS_CD     0x00000002
#define EDMAC_TD0_TFS_TRO    0x00000001
#define EDMAC_TD1_TBL        0xFFFF0000
#define EDMAC_TD2_TBA        0xFFFFFFFF

//Receive DMA descriptor flags
#define EDMAC_RD0_RACT       0x80000000
#define EDMAC_RD0_RDLE       0x40000000
#define EDMAC_RD0_RFP_SOF    0x20000000
#define EDMAC_RD0_RFP_EOF    0x10000000
#define EDMAC_RD0_RFE        0x08000000
#define EDMAC_RD0_RFS_MASK   0x0000039F
#define EDMAC_RD0_RFS_RFOF   0x00000200
#define EDMAC_RD0_RFS_RABT   0x00000100
#define EDMAC_RD0_RFS_RMAF   0x00000080
#define EDMAC_RD0_RFS_RRF    0x00000010
#define EDMAC_RD0_RFS_RTLF   0x00000008
#define EDMAC_RD0_RFS_RTSF   0x00000004
#define EDMAC_RD0_RFS_PRE    0x00000002
#define EDMAC_RD0_RFS_CERF   0x00000001
#define EDMAC_RD1_RBL        0xFFFF0000
#define EDMAC_RD1_RFL        0x0000FFFF
#define EDMAC_RD2_RBA        0xFFFFFFFF

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Transmit DMA descriptor
 **/

typedef struct
{
   uint32_t td0;
   uint32_t td1;
   uint32_t td2;
   uint32_t padding;
} Rza2Eth1TxDmaDesc;


/**
 * @brief Receive DMA descriptor
 **/

typedef struct
{
   uint32_t rd0;
   uint32_t rd1;
   uint32_t rd2;
   uint32_t padding;
} Rza2Eth1RxDmaDesc;


//RZ/A2 Ethernet MAC driver (ETHERC0 instance)
extern const NicDriver rza2Eth1Driver;

//RZ/A2 Ethernet MAC related functions
error_t rza2Eth1Init(NetInterface *interface);
void rza2Eth1InitGpio(NetInterface *interface);
void rza2Eth1InitDmaDesc(NetInterface *interface);

void rza2Eth1Tick(NetInterface *interface);

void rza2Eth1EnableIrq(NetInterface *interface);
void rza2Eth1DisableIrq(NetInterface *interface);
void rza2Eth1IrqHandler(uint32_t intSense);
void rza2Eth1EventHandler(NetInterface *interface);

error_t rza2Eth1SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t rza2Eth1ReceivePacket(NetInterface *interface);

error_t rza2Eth1UpdateMacAddrFilter(NetInterface *interface);
error_t rza2Eth1UpdateMacConfig(NetInterface *interface);

void rza2Eth1WritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t rza2Eth1ReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

void rza2Eth1WriteSmi(uint32_t data, uint_t length);
uint32_t rza2Eth1ReadSmi(uint_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
