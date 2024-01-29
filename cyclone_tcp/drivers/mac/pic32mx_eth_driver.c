/**
 * @file pic32mx_eth_driver.c
 * @brief PIC32MX Ethernet MAC driver
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

//Switch to the appropriate trace level
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include <p32xxxx.h>
#include <sys/kmem.h>
#include "core/net.h"
#include "drivers/mac/pic32mx_eth_driver.h"
#include "debug.h"

//Underlying network interface
static NetInterface *nicDriverInterface;

//Transmit buffer
static uint8_t txBuffer[PIC32MX_ETH_TX_BUFFER_COUNT][PIC32MX_ETH_TX_BUFFER_SIZE]
   __attribute__((coherent, aligned(4)));
//Receive buffer
static uint8_t rxBuffer[PIC32MX_ETH_RX_BUFFER_COUNT][PIC32MX_ETH_RX_BUFFER_SIZE]
   __attribute__((coherent, aligned(4)));
//Transmit buffer descriptors
static Pic32mxTxBufferDesc txBufferDesc[PIC32MX_ETH_TX_BUFFER_COUNT]
   __attribute__((coherent, aligned(4)));
//Receive buffer descriptors
static Pic32mxRxBufferDesc rxBufferDesc[PIC32MX_ETH_RX_BUFFER_COUNT]
   __attribute__((coherent, aligned(4)));

//Pointer to the current TX buffer descriptor
static Pic32mxTxBufferDesc *txCurBufferDesc;
//Pointer to the current RX buffer descriptor
static Pic32mxRxBufferDesc *rxCurBufferDesc;


/**
 * @brief PIC32MX Ethernet MAC driver
 **/

const NicDriver pic32mxEthDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   pic32mxEthInit,
   pic32mxEthTick,
   pic32mxEthEnableIrq,
   pic32mxEthDisableIrq,
   pic32mxEthEventHandler,
   pic32mxEthSendPacket,
   pic32mxEthUpdateMacAddrFilter,
   pic32mxEthUpdateMacConfig,
   pic32mxEthWritePhyReg,
   pic32mxEthReadPhyReg,
   TRUE,
   TRUE,
   TRUE,
   FALSE
};


/**
 * @brief PIC32MX Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t pic32mxEthInit(NetInterface *interface)
{
   error_t error;

   //Debug message
   TRACE_INFO("Initializing PIC32MX Ethernet MAC...\r\n");

   //Save underlying network interface
   nicDriverInterface = interface;

   //GPIO configuration
   pic32mxEthInitGpio(interface);

   //Disable Ethernet interrupts
   IEC1CLR = _IEC1_ETHIE_MASK;
   //Turn the Ethernet controller off
   ETHCON1CLR = _ETHCON1_ON_MASK | _ETHCON1_TXRTS_POSITION | _ETHCON1_RXEN_MASK;

   //Wait activity abort by polling the ETHBUSY bit
   while((ETHSTAT & _ETHSTAT_ETHBUSY_MASK) != 0)
   {
   }

   //Enable the Ethernet controller by setting the ON bit
   ETHCON1SET = _ETHCON1_ON_MASK;

   //Clear Ethernet interrupt flag
   IFS1CLR = _IFS1_ETHIF_MASK;
   //Disable any Ethernet controller interrupt generation
   ETHIEN = 0;
   ETHIRQ = 0;
   //Clear the TX and RX start addresses
   ETHTXST = 0;
   ETHRXST = 0;

   //Reset the MAC using SOFTRESET
   EMAC1CFG1SET = _EMAC1CFG1_SOFTRESET_MASK;
   EMAC1CFG1CLR = _EMAC1CFG1_SOFTRESET_MASK;

   //Reset the RMII module
   EMAC1SUPPSET = _EMAC1SUPP_RESETRMII_MASK;
   EMAC1SUPPCLR = _EMAC1SUPP_RESETRMII_MASK;

   //Issue an MIIM block reset by setting the RESETMGMT bit
   EMAC1MCFGSET = _EMAC1MCFG_RESETMGMT_MASK;
   EMAC1MCFGCLR = _EMAC1MCFG_RESETMGMT_MASK;

   //Select the proper divider for the MDC clock
   EMAC1MCFG = _EMAC1MCFG_CLKSEL_DIV40;

   //Valid Ethernet PHY or switch driver?
   if(interface->phyDriver != NULL)
   {
      //Ethernet PHY initialization
      error = interface->phyDriver->init(interface);
   }
   else if(interface->switchDriver != NULL)
   {
      //Ethernet switch initialization
      error = interface->switchDriver->init(interface);
   }
   else
   {
      //The interface is not properly configured
      error = ERROR_FAILURE;
   }

   //Any error to report?
   if(error)
   {
      return error;
   }

   //Optionally set the station MAC address
   if(macCompAddr(&interface->macAddr, &MAC_UNSPECIFIED_ADDR))
   {
      //Use the factory preprogrammed station address
      interface->macAddr.w[0] = EMAC1SA2;
      interface->macAddr.w[1] = EMAC1SA1;
      interface->macAddr.w[2] = EMAC1SA0;

      //Generate the 64-bit interface identifier
      macAddrToEui64(&interface->macAddr, &interface->eui64);
   }
   else
   {
      //Override the factory preprogrammed address
      EMAC1SA0 = interface->macAddr.w[2];
      EMAC1SA1 = interface->macAddr.w[1];
      EMAC1SA2 = interface->macAddr.w[0];
   }

   //Initialize hash table
   ETHHT0 = 0;
   ETHHT1 = 0;

   //Configure the receive filter
   ETHRXFC = _ETHRXFC_HTEN_MASK | _ETHRXFC_CRCOKEN_MASK |
      _ETHRXFC_RUNTEN_MASK | _ETHRXFC_UCEN_MASK | _ETHRXFC_BCEN_MASK;

   //Disable flow control
   EMAC1CFG1 = _EMAC1CFG1_RXENABLE_MASK;
   //Automatic padding and CRC generation
   EMAC1CFG2 = _EMAC1CFG2_PADENABLE_MASK | _EMAC1CFG2_CRCENABLE_MASK;
   //Set the maximum frame length
   EMAC1MAXF = PIC32MX_ETH_RX_BUFFER_SIZE;

   //Initialize DMA descriptor lists
   pic32mxEthInitBufferDesc(interface);

   //Enable desired interrupts
   ETHIENSET = _ETHIEN_PKTPENDIE_MASK | _ETHIEN_TXDONEIE_MASK;

   //Set interrupt priority
   IPC12CLR = _IPC12_ETHIP_MASK;
   IPC12SET = (PIC32MX_ETH_IRQ_PRIORITY << _IPC12_ETHIP_POSITION);
   //Set interrupt subpriority
   IPC12CLR = _IPC12_ETHIS_MASK;
   IPC12SET = (PIC32MX_ETH_IRQ_SUB_PRIORITY << _IPC12_ETHIS_POSITION);

   //Enable the reception by setting the RXEN bit
   ETHCON1SET = _ETHCON1_RXEN_MASK;

   //Accept any packets from the upper layer
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

__weak_func void pic32mxEthInitGpio(NetInterface *interface)
{
}


/**
 * @brief Initialize DMA descriptor lists
 * @param[in] interface Underlying network interface
 **/

void pic32mxEthInitBufferDesc(NetInterface *interface)
{
   uint_t i;

   //Initialize TX descriptor list
   for(i = 0; i < PIC32MX_ETH_TX_BUFFER_COUNT; i++)
   {
      //Point to the current descriptor
      txCurBufferDesc = &txBufferDesc[i];

      //Use linked list rather than linear list
      txCurBufferDesc->control = ETH_TX_CTRL_NPV;
      //Transmit buffer address
      txCurBufferDesc->address = KVA_TO_PA(txBuffer[i]);
      //Transmit status vector
      txCurBufferDesc->status1 = 0;
      txCurBufferDesc->status2 = 0;
      //Next descriptor address
      txCurBufferDesc->next = KVA_TO_PA(&txBufferDesc[i + 1]);
   }

   //The last descriptor is chained to the first entry
   txCurBufferDesc->next = KVA_TO_PA(&txBufferDesc[0]);
   //Point to the very first descriptor
   txCurBufferDesc = &txBufferDesc[0];

   //Initialize RX descriptor list
   for(i = 0; i < PIC32MX_ETH_RX_BUFFER_COUNT; i++)
   {
      //Point to the current descriptor
      rxCurBufferDesc = &rxBufferDesc[i];

      //The descriptor is initially owned by the DMA
      rxCurBufferDesc->control = ETH_RX_CTRL_NPV | ETH_RX_CTRL_EOWN;
      //Receive buffer address
      rxCurBufferDesc->address = KVA_TO_PA(rxBuffer[i]);
      //Receive status vector
      rxCurBufferDesc->status1 = 0;
      rxCurBufferDesc->status2 = 0;
      //Next descriptor address
      rxCurBufferDesc->next = KVA_TO_PA(&rxBufferDesc[i + 1]);
   }

   //The last descriptor is chained to the first entry
   rxCurBufferDesc->next = KVA_TO_PA(&rxBufferDesc[0]);
   //Point to the very first descriptor
   rxCurBufferDesc = &rxBufferDesc[0];

   //Starting address of TX descriptor table
   ETHTXST = KVA_TO_PA(&txBufferDesc[0]);
   //Starting address of RX descriptor table
   ETHRXST = KVA_TO_PA(&rxBufferDesc[0]);
   //Set receive buffer size
   ETHCON2 = PIC32MX_ETH_RX_BUFFER_SIZE;
}


/**
 * @brief PIC32MX Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to handle periodic
 * operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void pic32mxEthTick(NetInterface *interface)
{
   //Valid Ethernet PHY or switch driver?
   if(interface->phyDriver != NULL)
   {
      //Handle periodic operations
      interface->phyDriver->tick(interface);
   }
   else if(interface->switchDriver != NULL)
   {
      //Handle periodic operations
      interface->switchDriver->tick(interface);
   }
   else
   {
      //Just for sanity
   }
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void pic32mxEthEnableIrq(NetInterface *interface)
{
   //Enable Ethernet MAC interrupts
   IEC1SET = _IEC1_ETHIE_MASK;

   //Valid Ethernet PHY or switch driver?
   if(interface->phyDriver != NULL)
   {
      //Enable Ethernet PHY interrupts
      interface->phyDriver->enableIrq(interface);
   }
   else if(interface->switchDriver != NULL)
   {
      //Enable Ethernet switch interrupts
      interface->switchDriver->enableIrq(interface);
   }
   else
   {
      //Just for sanity
   }
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void pic32mxEthDisableIrq(NetInterface *interface)
{
   //Disable Ethernet MAC interrupts
   IEC1CLR = _IEC1_ETHIE_MASK;

   //Valid Ethernet PHY or switch driver?
   if(interface->phyDriver != NULL)
   {
      //Disable Ethernet PHY interrupts
      interface->phyDriver->disableIrq(interface);
   }
   else if(interface->switchDriver != NULL)
   {
      //Disable Ethernet switch interrupts
      interface->switchDriver->disableIrq(interface);
   }
   else
   {
      //Just for sanity
   }
}


/**
 * @brief PIC32MX Ethernet MAC interrupt service routine
 **/

void pic32mxEthIrqHandler(void)
{
   bool_t flag;
   uint32_t status;

   //Interrupt service routine prologue
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Read interrupt status register
   status = ETHIRQ;

   //Packet transmitted?
   if((status & _ETHIRQ_TXDONE_MASK) != 0)
   {
      //Clear TXDONE interrupt flag
      ETHIRQCLR = _ETHIRQ_TXDONE_MASK;

      //Check whether the TX buffer is available for writing
      if((txCurBufferDesc->control & ETH_TX_CTRL_EOWN) == 0)
      {
         //Notify the TCP/IP stack that the transmitter is ready to send
         flag |= osSetEventFromIsr(&nicDriverInterface->nicTxEvent);
      }
   }

   //Packet received?
   if((status & _ETHIRQ_PKTPEND_MASK) != 0)
   {
      //Disable PKTPEND interrupt
      ETHIENCLR = _ETHIEN_PKTPENDIE_MASK;

      //Set event flag
      nicDriverInterface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Clear ETHIF interrupt flag before exiting the service routine
   IFS1CLR = _IFS1_ETHIF_MASK;

   //Interrupt service routine epilogue
   osExitIsr(flag);
}


/**
 * @brief PIC32MX Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void pic32mxEthEventHandler(NetInterface *interface)
{
   error_t error;

   //Packet received?
   if((ETHIRQ & _ETHIRQ_PKTPEND_MASK) != 0)
   {
      //Process all pending packets
      do
      {
         //Read incoming packet
         error = pic32mxEthReceivePacket(interface);

         //No more data in the receive buffer?
      } while(error != ERROR_BUFFER_EMPTY);
   }

   //Re-enable PKTPEND interrupt
   ETHIENSET = _ETHIEN_PKTPENDIE_MASK;
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @param[in] ancillary Additional options passed to the stack along with
 *   the packet
 * @return Error code
 **/

error_t pic32mxEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary)
{
   size_t length;
   uint32_t value;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > PIC32MX_ETH_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return ERROR_INVALID_LENGTH;
   }

   //Make sure the current buffer is available for writing
   if((txCurBufferDesc->control & ETH_TX_CTRL_EOWN) != 0)
   {
      return ERROR_FAILURE;
   }

   //Copy user data to the transmit buffer
   netBufferRead(PA_TO_KVA1(txCurBufferDesc->address), buffer, offset, length);

   //Write the number of bytes to send
   value = (length << 16) & ETH_TX_CTRL_BYTE_COUNT;
   //Set SOP and EOP flags since the data fits in a single buffer
   value |= ETH_TX_CTRL_SOP | ETH_TX_CTRL_EOP | ETH_TX_CTRL_NPV;
   //Give the ownership of the descriptor to the DMA
   txCurBufferDesc->control = value | ETH_TX_CTRL_EOWN;

   //Set TXRTS bit to start the transmission
   ETHCON1SET = _ETHCON1_TXRTS_MASK;

   //Point to the next descriptor in the list
   txCurBufferDesc = PA_TO_KVA1(txCurBufferDesc->next);

   //Check whether the next buffer is available for writing
   if((txCurBufferDesc->control & ETH_TX_CTRL_EOWN) == 0)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
   }

   //Data successfully written
   return NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t pic32mxEthReceivePacket(NetInterface *interface)
{
   static uint32_t temp[PIC32MX_ETH_RX_BUFFER_SIZE / 4];
   error_t error;
   size_t n;
   NetRxAncillary ancillary;

   //Current buffer available for reading?
   if((rxCurBufferDesc->control & ETH_RX_CTRL_EOWN) == 0)
   {
      //SOP and EOP flags should be set
      if((rxCurBufferDesc->control & ETH_RX_CTRL_SOP) != 0 &&
         (rxCurBufferDesc->control & ETH_RX_CTRL_EOP) != 0)
      {
         //Make sure no error occurred
         if((rxCurBufferDesc->status2 & ETH_RX_STATUS2_OK) != 0)
         {
            //Retrieve the length of the frame
            n = (rxCurBufferDesc->control & ETH_RX_CTRL_BYTE_COUNT) >> 16;
            //Limit the number of data to read
            n = MIN(n, PIC32MX_ETH_RX_BUFFER_SIZE);

            //Copy data from the receive buffer
            osMemcpy(temp, PA_TO_KVA1(rxCurBufferDesc->address), n);

            //Additional options can be passed to the stack along with the packet
            ancillary = NET_DEFAULT_RX_ANCILLARY;

            //Pass the packet to the upper layer
            nicProcessPacket(interface, (uint8_t *) temp, n, &ancillary);

            //Valid packet received
            error = NO_ERROR;
         }
         else
         {
            //The received packet contains an error
            error = ERROR_INVALID_PACKET;
         }
      }
      else
      {
         //The packet is not valid
         error = ERROR_INVALID_PACKET;
      }

      //Give the ownership of the descriptor back to the DMA
      rxCurBufferDesc->control = ETH_RX_CTRL_NPV | ETH_RX_CTRL_EOWN;

      //Point to the next descriptor in the list
      rxCurBufferDesc = PA_TO_KVA1(rxCurBufferDesc->next);

      //Once software processes a received packet, it should write the BUFCDEC
      //bit in order to decrement the packet buffer count BUFCNT
      ETHCON1SET = _ETHCON1_BUFCDEC_MASK;
   }
   else
   {
      //No more data in the receive buffer
      error = ERROR_BUFFER_EMPTY;
   }

   //Return status code
   return error;
}


/**
 * @brief Configure MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t pic32mxEthUpdateMacAddrFilter(NetInterface *interface)
{
   uint_t i;
   uint_t k;
   uint32_t crc;
   uint32_t hashTable[2];
   MacFilterEntry *entry;

   //Debug message
   TRACE_DEBUG("Updating MAC filter...\r\n");

   //Clear hash table
   hashTable[0] = 0;
   hashTable[1] = 0;

   //The MAC address filter contains the list of MAC addresses to accept
   //when receiving an Ethernet frame
   for(i = 0; i < MAC_ADDR_FILTER_SIZE; i++)
   {
      //Point to the current entry
      entry = &interface->macAddrFilter[i];

      //Valid entry?
      if(entry->refCount > 0)
      {
         //Compute CRC over the current MAC address
         crc = pic32mxEthCalcCrc(&entry->addr, sizeof(MacAddr));
         //Calculate the corresponding index in the table
         k = (crc >> 23) & 0x3F;
         //Update hash table contents
         hashTable[k / 32] |= (1 << (k % 32));
      }
   }

   //Write the hash table
   ETHHT0 = hashTable[0];
   ETHHT1 = hashTable[1];

   //Debug message
   TRACE_DEBUG("  ETHHT0 = %08" PRIX32 "\r\n", ETHHT0);
   TRACE_DEBUG("  ETHHT1 = %08" PRIX32 "\r\n", ETHHT1);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t pic32mxEthUpdateMacConfig(NetInterface *interface)
{
   //Check current operating speed
   if(interface->linkSpeed == NIC_LINK_SPEED_100MBPS)
   {
      //100BASE-TX operation mode
      EMAC1SUPPSET = _EMAC1SUPP_SPEEDRMII_MASK;
   }
   else
   {
      //10BASE-T operation mode
      EMAC1SUPPCLR = _EMAC1SUPP_SPEEDRMII_MASK;
   }

   //Half-duplex or full-duplex mode?
   if(interface->duplexMode == NIC_FULL_DUPLEX_MODE)
   {
      //Configure FULLDPLX bit to match the current duplex mode
      EMAC1CFG2SET = _EMAC1CFG2_FULLDPLX_MASK;
      //Configure the Back-to-Back Inter-Packet Gap register
      EMAC1IPGT = 0x15;
   }
   else
   {
      //Configure FULLDPLX bit to match the current duplex mode
      EMAC1CFG2CLR = _EMAC1CFG2_FULLDPLX_MASK;
      //Configure the Back-to-Back Inter-Packet Gap register
      EMAC1IPGT = 0x12;
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] opcode Access type (2 bits)
 * @param[in] phyAddr PHY address (5 bits)
 * @param[in] regAddr Register address (5 bits)
 * @param[in] data Register value
 **/

void pic32mxEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data)
{
   //Valid opcode?
   if(opcode == SMI_OPCODE_WRITE)
   {
      //Set PHY address and register address
      EMAC1MADR = (phyAddr << _EMAC1MADR_PHYADDR_POSITION) | regAddr;
      //Start a write operation
      EMAC1MWTD = data & _EMAC1MWTD_MWTD_MASK;

      //Wait for busy bit to be set
      __asm__ __volatile__ ("nop;");
      __asm__ __volatile__ ("nop;");
      __asm__ __volatile__ ("nop;");

      //Wait for the write to complete
      while((EMAC1MIND & _EMAC1MIND_MIIMBUSY_MASK) != 0)
      {
      }
   }
   else
   {
      //The MAC peripheral only supports standard Clause 22 opcodes
   }
}


/**
 * @brief Read PHY register
 * @param[in] opcode Access type (2 bits)
 * @param[in] phyAddr PHY address (5 bits)
 * @param[in] regAddr Register address (5 bits)
 * @return Register value
 **/

uint16_t pic32mxEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr)
{
   uint16_t data;

   //Valid opcode?
   if(opcode == SMI_OPCODE_READ)
   {
      //Set PHY address and register address
      EMAC1MADR = (phyAddr << _EMAC1MADR_PHYADDR_POSITION) | regAddr;
      //Start a read operation
      EMAC1MCMD = _EMAC1MCMD_READ_MASK;

      //Wait for busy bit to be set
      __asm__ __volatile__ ("nop;");
      __asm__ __volatile__ ("nop;");
      __asm__ __volatile__ ("nop;");

      //Wait for the read to complete
      while((EMAC1MIND & _EMAC1MIND_MIIMBUSY_MASK) != 0)
      {
      }

      //Clear command register
      EMAC1MCMD = 0;
      //Get register value
      data = EMAC1MRDD & _EMAC1MRDD_MRDD_MASK;
   }
   else
   {
      //The MAC peripheral only supports standard Clause 22 opcodes
      data = 0;
   }

   //Return the value of the PHY register
   return data;
}


/**
 * @brief CRC calculation
 * @param[in] data Pointer to the data over which to calculate the CRC
 * @param[in] length Number of bytes to process
 * @return Resulting CRC value
 **/

uint32_t pic32mxEthCalcCrc(const void *data, size_t length)
{
   uint_t i;
   uint_t j;
   uint32_t crc;
   const uint8_t *p;

   //Point to the data over which to calculate the CRC
   p = (uint8_t *) data;
   //CRC preset value
   crc = 0xFFFFFFFF;

   //Loop through data
   for(i = 0; i < length; i++)
   {
      //The message is processed bit by bit
      for(j = 0; j < 8; j++)
      {
         //Update CRC value
         if((((crc >> 31) ^ (p[i] >> j)) & 0x01) != 0)
         {
            crc = (crc << 1) ^ 0x04C11DB7;
         }
         else
         {
            crc = crc << 1;
         }
      }
   }

   //Return CRC value
   return crc;
}
