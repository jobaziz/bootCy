/**
 * @file tc2xx_eth_driver.h
 * @brief Infineon AURIX TriCore TC2xx Ethernet MAC driver
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

#ifndef _TC2XX_ETH_DRIVER_H
#define _TC2XX_ETH_DRIVER_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef TC2XX_ETH_TX_BUFFER_COUNT
   #define TC2XX_ETH_TX_BUFFER_COUNT 3
#elif (TC2XX_ETH_TX_BUFFER_COUNT < 1)
   #error TC2XX_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef TC2XX_ETH_TX_BUFFER_SIZE
   #define TC2XX_ETH_TX_BUFFER_SIZE 1536
#elif (TC2XX_ETH_TX_BUFFER_SIZE != 1536)
   #error TC2XX_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef TC2XX_ETH_RX_BUFFER_COUNT
   #define TC2XX_ETH_RX_BUFFER_COUNT 6
#elif (TC2XX_ETH_RX_BUFFER_COUNT < 1)
   #error TC2XX_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef TC2XX_ETH_RX_BUFFER_SIZE
   #define TC2XX_ETH_RX_BUFFER_SIZE 1536
#elif (TC2XX_ETH_RX_BUFFER_SIZE != 1536)
   #error TC2XX_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Ethernet interrupt priority
#ifndef TC2XX_ETH_IRQ_PRIORITY
   #define TC2XX_ETH_IRQ_PRIORITY 10
#elif (TC2XX_ETH_IRQ_PRIORITY < 0)
   #error TC2XX_ETH_IRQ_PRIORITY parameter is not valid
#endif

//ETH_STATUS register
#define ETH_STATUS_GLPII         0x40000000
#define ETH_STATUS_TTI           0x20000000
#define ETH_STATUS_GPI           0x10000000
#define ETH_STATUS_GMI           0x08000000
#define ETH_STATUS_GLI           0x04000000
#define ETH_STATUS_EB            0x03800000
#define ETH_STATUS_TS            0x00700000
#define ETH_STATUS_RS            0x000E0000
#define ETH_STATUS_NIS           0x00010000
#define ETH_STATUS_AIS           0x00008000
#define ETH_STATUS_ERI           0x00004000
#define ETH_STATUS_FBI           0x00002000
#define ETH_STATUS_ETI           0x00000400
#define ETH_STATUS_RWT           0x00000200
#define ETH_STATUS_RPS           0x00000100
#define ETH_STATUS_RU            0x00000080
#define ETH_STATUS_RI            0x00000040
#define ETH_STATUS_UNF           0x00000020
#define ETH_STATUS_OVF           0x00000010
#define ETH_STATUS_TJT           0x00000008
#define ETH_STATUS_TU            0x00000004
#define ETH_STATUS_TPS           0x00000002
#define ETH_STATUS_TI            0x00000001

//ETH_INTERRUPT_ENABLE register
#define ETH_INTERRUPT_ENABLE_NIE 0x00010000
#define ETH_INTERRUPT_ENABLE_AIE 0x00008000
#define ETH_INTERRUPT_ENABLE_ERE 0x00004000
#define ETH_INTERRUPT_ENABLE_FBE 0x00002000
#define ETH_INTERRUPT_ENABLE_ETE 0x00000400
#define ETH_INTERRUPT_ENABLE_RWE 0x00000200
#define ETH_INTERRUPT_ENABLE_RSE 0x00000100
#define ETH_INTERRUPT_ENABLE_RUE 0x00000080
#define ETH_INTERRUPT_ENABLE_RIE 0x00000040
#define ETH_INTERRUPT_ENABLE_UNE 0x00000020
#define ETH_INTERRUPT_ENABLE_OVE 0x00000010
#define ETH_INTERRUPT_ENABLE_TJE 0x00000008
#define ETH_INTERRUPT_ENABLE_TUE 0x00000004
#define ETH_INTERRUPT_ENABLE_TSE 0x00000002
#define ETH_INTERRUPT_ENABLE_TIE 0x00000001

//Transmit DMA descriptor flags
#define ETH_TDES0_OWN            0x80000000
#define ETH_TDES0_IC             0x40000000
#define ETH_TDES0_LS             0x20000000
#define ETH_TDES0_FS             0x10000000
#define ETH_TDES0_DC             0x08000000
#define ETH_TDES0_DP             0x04000000
#define ETH_TDES0_TTSE           0x02000000
#define ETH_TDES0_CIC            0x00C00000
#define ETH_TDES0_TER            0x00200000
#define ETH_TDES0_TCH            0x00100000
#define ETH_TDES0_TTSS           0x00020000
#define ETH_TDES0_IHE            0x00010000
#define ETH_TDES0_ES             0x00008000
#define ETH_TDES0_JT             0x00004000
#define ETH_TDES0_FF             0x00002000
#define ETH_TDES0_IPE            0x00001000
#define ETH_TDES0_LCA            0x00000800
#define ETH_TDES0_NC             0x00000400
#define ETH_TDES0_LCO            0x00000200
#define ETH_TDES0_EC             0x00000100
#define ETH_TDES0_VF             0x00000080
#define ETH_TDES0_CC             0x00000078
#define ETH_TDES0_ED             0x00000004
#define ETH_TDES0_UF             0x00000002
#define ETH_TDES0_DB             0x00000001
#define ETH_TDES1_TBS2           0x1FFF0000
#define ETH_TDES1_TBS1           0x00001FFF
#define ETH_TDES2_B1AP           0xFFFFFFFF
#define ETH_TDES3_B2AP           0xFFFFFFFF
#define ETH_TDES6_TTSL           0xFFFFFFFF
#define ETH_TDES7_TTSH           0xFFFFFFFF

//Receive DMA descriptor flags
#define ETH_RDES0_OWN            0x80000000
#define ETH_RDES0_AFM            0x40000000
#define ETH_RDES0_FL             0x3FFF0000
#define ETH_RDES0_ES             0x00008000
#define ETH_RDES0_DE             0x00004000
#define ETH_RDES0_SAF            0x00002000
#define ETH_RDES0_LE             0x00001000
#define ETH_RDES0_OE             0x00000800
#define ETH_RDES0_VLAN           0x00000400
#define ETH_RDES0_FS             0x00000200
#define ETH_RDES0_LS             0x00000100
#define ETH_RDES0_TS             0x00000080
#define ETH_RDES0_LCO            0x00000040
#define ETH_RDES0_FT             0x00000020
#define ETH_RDES0_RWT            0x00000010
#define ETH_RDES0_RE             0x00000008
#define ETH_RDES0_DBE            0x00000004
#define ETH_RDES0_CE             0x00000002
#define ETH_RDES0_ESA            0x00000001
#define ETH_RDES1_DIC            0x80000000
#define ETH_RDES1_RBS2           0x1FFF0000
#define ETH_RDES1_RER            0x00008000
#define ETH_RDES1_RCH            0x00004000
#define ETH_RDES1_RBS1           0x00001FFF
#define ETH_RDES2_B1AP           0xFFFFFFFF
#define ETH_RDES3_B2AP           0xFFFFFFFF
#define ETH_RDES4_PV             0x00002000
#define ETH_RDES4_PFT            0x00001000
#define ETH_RDES4_MT             0x00000F00
#define ETH_RDES4_IP6R           0x00000080
#define ETH_RDES4_IP4R           0x00000040
#define ETH_RDES4_IPCB           0x00000020
#define ETH_RDES4_IPE            0x00000010
#define ETH_RDES4_IPHE           0x00000008
#define ETH_RDES4_IPT            0x00000007
#define ETH_RDES6_RTSL           0xFFFFFFFF
#define ETH_RDES7_RTSH           0xFFFFFFFF

//Get CPU core identifier
#define ETH_CPU_ID() (_mfcr(CPU_CORE_ID) & IFX_CPU_CORE_ID_CORE_ID_MSK)

//Convert a local DSPR address to a global DSPR address
#define ETH_GLOBAL_DSPR_ADDR(address) \
   ((((uint32_t) (address) & 0xF0000000) == 0xD0000000) ? \
   ((((uint32_t) (address) & 0x000FFFFF) | 0x70000000) - (ETH_CPU_ID() * 0x10000000)) : \
   (uint32_t) (address))

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Transmit DMA descriptor
 **/

typedef struct
{
   uint32_t tdes0;
   uint32_t tdes1;
   uint32_t tdes2;
   uint32_t tdes3;
   uint32_t tdes4;
   uint32_t tdes5;
   uint32_t tdes6;
   uint32_t tdes7;
} Tc2xxTxDmaDesc;


/**
 * @brief Receive DMA descriptor
 **/

typedef struct
{
   uint32_t rdes0;
   uint32_t rdes1;
   uint32_t rdes2;
   uint32_t rdes3;
   uint32_t rdes4;
   uint32_t rdes5;
   uint32_t rdes6;
   uint32_t rdes7;
} Tc2xxRxDmaDesc;


//TC2xx Ethernet MAC driver
extern const NicDriver tc2xxEthDriver;

//TC2xx Ethernet MAC related functions
error_t tc2xxEthInit(NetInterface *interface);
void tc2xxEthInitGpio(NetInterface *interface);
void tc2xxEthInitDmaDesc(NetInterface *interface);

void tc2xxEthTick(NetInterface *interface);

void tc2xxEthEnableIrq(NetInterface *interface);
void tc2xxEthDisableIrq(NetInterface *interface);
void tc2xxEthIrqHandler(int_t arg);
void tc2xxEthEventHandler(NetInterface *interface);

error_t tc2xxEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t tc2xxEthReceivePacket(NetInterface *interface);

error_t tc2xxEthUpdateMacAddrFilter(NetInterface *interface);
error_t tc2xxEthUpdateMacConfig(NetInterface *interface);

void tc2xxEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t tc2xxEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

uint32_t tc2xxEthCalcCrc(const void *data, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif