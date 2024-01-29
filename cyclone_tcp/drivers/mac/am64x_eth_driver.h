/**
 * @file am64x_eth_driver.h
 * @brief AM64x Ethernet MAC driver
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

#ifndef _AM64X_ETH_DRIVER_H
#define _AM64X_ETH_DRIVER_H

//Dependencies
#include "core/nic.h"

//CPSW ports
#define CPSW_PORT0 0
#define CPSW_PORT1 1
#define CPSW_PORT2 2

//CPSW channels
#define CPSW_CH0 0
#define CPSW_CH1 1
#define CPSW_CH2 2
#define CPSW_CH3 3
#define CPSW_CH4 4
#define CPSW_CH5 5
#define CPSW_CH6 6
#define CPSW_CH7 7

//Number of entries in the ALE table
#define CPSW_ALE_MAX_ENTRIES                      512

//ALE table entry
#define CPSW_ALE_WORD1_ENTRY_TYPE_MASK            (3 << 28)
#define CPSW_ALE_WORD1_ENTRY_TYPE_FREE            (0 << 28)
#define CPSW_ALE_WORD1_ENTRY_TYPE_ADDR            (1 << 28)
#define CPSW_ALE_WORD1_ENTRY_TYPE_VLAN            (2 << 28)
#define CPSW_ALE_WORD1_ENTRY_TYPE_VLAN_ADDR       (3 << 28)
#define CPSW_ALE_WORD1_MULTICAST                  (1 << 8)

//Unicast address table entry
#define CPSW_ALE_WORD2_TRUNK                      (1 << 4)
#define CPSW_ALE_WORD2_PORT_NUMBER_MASK           (3 << 2)
#define CPSW_ALE_WORD2_PORT_NUMBER(n)             ((n) << 2)
#define CPSW_ALE_WORD2_BLOCK                      (1 << 1)
#define CPSW_ALE_WORD2_SECURE                     (1 << 0)
#define CPSW_ALE_WORD1_UNICAST_TYPE_MASK          (3 << 30)
#define CPSW_ALE_WORD1_UNICAST_TYPE(n)            ((n) << 30)

//Multicast address table entry
#define CPSW_ALE_WORD2_PORT_MASK_MASK             (3 << 2)
#define CPSW_ALE_WORD2_PORT_MASK(n)               ((n) << 2)
#define CPSW_ALE_WORD2_SUPER                      (1 << 1)
#define CPSW_ALE_WORD1_MCAST_FWD_STATE_MASK       (3 << 30)
#define CPSW_ALE_WORD1_MCAST_FWD_STATE(n)         ((n) << 30)

//VLAN table entry
#define CPSW_ALE_WORD2_NO_LEARN_MASK_MASK         (7 << 2)
#define CPSW_ALE_WORD2_NO_LEARN_MASK(n)           ((n) << 2)
#define CPSW_ALE_WORD2_VLAN_FORCE_INGRESS_CHECK   (7 << 1)
#define CPSW_ALE_WORD1_VLAN_ID_MASK               (4095 << 16)
#define CPSW_ALE_WORD1_VLAN_ID(n)                 ((n) << 16)
#define CPSW_ALE_WORD1_NOFRAG                     (1 << 15)
#define CPSW_ALE_WORD1_REG_MCAST_FLOOD_INDEX_MASK (7 << 4)
#define CPSW_ALE_WORD1_REG_MCAST_FLOOD_INDEX(n)   ((n) << 4)
#define CPSW_ALE_WORD0_FORCE_UNTAG_EGRESS_MASK    (7 << 24)
#define CPSW_ALE_WORD0_FORCE_UNTAG_EGRESS(n)      ((n) << 24)
#define CPSW_ALE_WORD0_LMTNXTHDR                  (1 << 23)
#define CPSW_ALE_WORD0_UREGMSK_MASK               (7 << 12)
#define CPSW_ALE_WORD0_UREGMSK(n)                 ((n) << 12)
#define CPSW_ALE_WORD0_VLAN_MEMBER_LIST_MASK      (7 << 0)
#define CPSW_ALE_WORD0_VLAN_MEMBER_LIST(n)        ((n) << 0)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ALE table entry
 **/

typedef struct
{
   uint32_t word2;
   uint32_t word1;
   uint32_t word0;
} Am64xAleEntry;


/**
 * @brief Enhanced TX DMA descriptor
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
} Am64xTxDmaDesc;


/**
 * @brief Enhanced RX DMA descriptor
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
} Am64xRxDmaDesc;


//AM64x Ethernet MAC driver
extern const NicDriver am64xEthPort1Driver;
extern const NicDriver am64xEthPort2Driver;

//AM64x Ethernet MAC related functions
error_t am64xEthInitPort1(NetInterface *interface);
error_t am64xEthInitPort2(NetInterface *interface);
void am64xEthInitInstance(NetInterface *interface);
void am64xEthInitGpio(NetInterface *interface);

void am64xEthTick(NetInterface *interface);

void am64xEthEnableIrq(NetInterface *interface);
void am64xEthDisableIrq(NetInterface *interface);
void am64xEthRxIrqHandler(void *arg);
void am64xEthEventHandler(NetInterface *interface);

error_t am64xEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t am64xEthUpdateMacAddrFilter(NetInterface *interface);
error_t am64xEthUpdateMacConfig(NetInterface *interface);

void am64xEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t am64xEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

void am64xEthWriteEntry(uint_t index, const Am64xAleEntry *entry);
void am64xEthReadEntry(uint_t index, Am64xAleEntry *entry);

uint_t am64xEthFindFreeEntry(void);
uint_t am64xEthFindVlanEntry(uint_t vlanId);
uint_t am64xEthFindVlanAddrEntry(uint_t vlanId, MacAddr *macAddr);

error_t am64xEthAddVlanEntry(uint_t port, uint_t vlanId);
error_t am64xEthAddVlanAddrEntry(uint_t port, uint_t vlanId, MacAddr *macAddr);
error_t am64xEthDeleteVlanAddrEntry(uint_t port, uint_t vlanId, MacAddr *macAddr);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
