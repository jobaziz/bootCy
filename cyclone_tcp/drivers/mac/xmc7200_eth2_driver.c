/**
 * @file xmc7200_eth2_driver.c
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

//Switch to the appropriate trace level
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include <limits.h>
#include "cy_device.h"
#include "core/net.h"
#include "drivers/mac/xmc7200_eth2_driver.h"
#include "debug.h"

//Underlying network interface
static NetInterface *nicDriverInterface;

//IAR EWARM compiler?
#if defined(__ICCARM__)

//TX buffer
#pragma data_alignment = 8
#pragma location = XMC7200_ETH2_RAM_SECTION
static uint8_t txBuffer[XMC7200_ETH2_TX_BUFFER_COUNT][XMC7200_ETH2_TX_BUFFER_SIZE];
//RX buffer
#pragma data_alignment = 8
#pragma location = XMC7200_ETH2_RAM_SECTION
static uint8_t rxBuffer[XMC7200_ETH2_RX_BUFFER_COUNT][XMC7200_ETH2_RX_BUFFER_SIZE];
//TX buffer descriptors
#pragma data_alignment = 4
#pragma location = XMC7200_ETH2_RAM_SECTION
static Xmc7200Eth2TxBufferDesc txBufferDesc[XMC7200_ETH2_TX_BUFFER_COUNT];
//RX buffer descriptors
#pragma data_alignment = 4
#pragma location = XMC7200_ETH2_RAM_SECTION
static Xmc7200Eth2RxBufferDesc rxBufferDesc[XMC7200_ETH2_RX_BUFFER_COUNT];

//Dummy TX buffer
#pragma data_alignment = 8
#pragma location = XMC7200_ETH2_RAM_SECTION
static uint8_t dummyTxBuffer[XMC7200_ETH2_DUMMY_BUFFER_COUNT][XMC7200_ETH2_DUMMY_BUFFER_SIZE];
//Dummy RX buffer
#pragma data_alignment = 8
#pragma location = XMC7200_ETH2_RAM_SECTION
static uint8_t dummyRxBuffer[XMC7200_ETH2_DUMMY_BUFFER_COUNT][XMC7200_ETH2_DUMMY_BUFFER_SIZE];
//Dummy TX buffer descriptors
#pragma data_alignment = 4
#pragma location = XMC7200_ETH2_RAM_SECTION
static Xmc7200Eth2TxBufferDesc dummyTxBufferDesc[XMC7200_ETH2_DUMMY_BUFFER_COUNT];
//Dummy RX buffer descriptors
#pragma data_alignment = 4
#pragma location = XMC7200_ETH2_RAM_SECTION
static Xmc7200Eth2RxBufferDesc dummyRxBufferDesc[XMC7200_ETH2_DUMMY_BUFFER_COUNT];

//Keil MDK-ARM or GCC compiler?
#else

//TX buffer
static uint8_t txBuffer[XMC7200_ETH2_TX_BUFFER_COUNT][XMC7200_ETH2_TX_BUFFER_SIZE]
   __attribute__((aligned(8), __section__(XMC7200_ETH2_RAM_SECTION)));
//RX buffer
static uint8_t rxBuffer[XMC7200_ETH2_RX_BUFFER_COUNT][XMC7200_ETH2_RX_BUFFER_SIZE]
   __attribute__((aligned(8), __section__(XMC7200_ETH2_RAM_SECTION)));
//TX buffer descriptors
static Xmc7200Eth2TxBufferDesc txBufferDesc[XMC7200_ETH2_TX_BUFFER_COUNT]
   __attribute__((aligned(4), __section__(XMC7200_ETH2_RAM_SECTION)));
//RX buffer descriptors
static Xmc7200Eth2RxBufferDesc rxBufferDesc[XMC7200_ETH2_RX_BUFFER_COUNT]
   __attribute__((aligned(4), __section__(XMC7200_ETH2_RAM_SECTION)));

//Dummy TX buffer
static uint8_t dummyTxBuffer[XMC7200_ETH2_DUMMY_BUFFER_COUNT][XMC7200_ETH2_DUMMY_BUFFER_SIZE]
   __attribute__((aligned(8), __section__(XMC7200_ETH2_RAM_SECTION)));
//Dummy RX buffer
static uint8_t dummyRxBuffer[XMC7200_ETH2_DUMMY_BUFFER_COUNT][XMC7200_ETH2_DUMMY_BUFFER_SIZE]
   __attribute__((aligned(8), __section__(XMC7200_ETH2_RAM_SECTION)));
//Dummy TX buffer descriptors
static Xmc7200Eth2TxBufferDesc dummyTxBufferDesc[XMC7200_ETH2_DUMMY_BUFFER_COUNT]
   __attribute__((aligned(4), __section__(XMC7200_ETH2_RAM_SECTION)));
//Dummy RX buffer descriptors
static Xmc7200Eth2RxBufferDesc dummyRxBufferDesc[XMC7200_ETH2_DUMMY_BUFFER_COUNT]
   __attribute__((aligned(4), __section__(XMC7200_ETH2_RAM_SECTION)));

#endif

//TX buffer index
static uint_t txBufferIndex;
//RX buffer index
static uint_t rxBufferIndex;


/**
 * @brief XMC7200 Ethernet MAC driver (ETH1 instance)
 **/

const NicDriver xmc7200Eth2Driver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   xmc7200Eth2Init,
   xmc7200Eth2Tick,
   xmc7200Eth2EnableIrq,
   xmc7200Eth2DisableIrq,
   xmc7200Eth2EventHandler,
   xmc7200Eth2SendPacket,
   xmc7200Eth2UpdateMacAddrFilter,
   xmc7200Eth2UpdateMacConfig,
   xmc7200Eth2WritePhyReg,
   xmc7200Eth2ReadPhyReg,
   TRUE,
   TRUE,
   TRUE,
   FALSE
};


/**
 * @brief XMC7200 Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t xmc7200Eth2Init(NetInterface *interface)
{
   error_t error;
   volatile uint32_t status;

   //Debug message
   TRACE_INFO("Initializing XMC7200 Ethernet MAC (ETH1)...\r\n");

   //Save underlying network interface
   nicDriverInterface = interface;

   //Enable GMAC peripheral clock
   PMC_REGS->PMC_PCER1 = (1 << (ID_GMAC - 32));

   //Disable transmit and receive circuits
   ETH1->NETWORK_CONTROL = 0;

   //GPIO configuration
   xmc7200Eth2InitGpio(interface);

   //Configure MDC clock speed
   ETH1->NETWORK_CONFIG = 7 << ETH_NETWORK_CONFIG_MDC_CLOCK_DIVISION_Pos;
   //Enable management port (MDC and MDIO)
   ETH1->NETWORK_CONTROL |= ETH_NETWORK_CONTROL_MAN_PORT_EN_Msk;

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

   //Set the MAC address of the station
   ETH1->SPEC_ADD1_BOTTOM = interface->macAddr.w[0] | (interface->macAddr.w[1] << 16);
   ETH1->SPEC_ADD1_TOP = interface->macAddr.w[2];

   //The MAC supports 3 additional addresses for unicast perfect filtering
   ETH1->SPEC_ADD2_BOTTOM = 0;
   ETH1->SPEC_ADD3_BOTTOM = 0;
   ETH1->SPEC_ADD4_BOTTOM = 0;

   //Initialize hash table
   ETH1->HASH_BOTTOM = 0;
   ETH1->HASH_TOP = 0;

   //Configure the receive filter
   ETH1->NETWORK_CONFIG |= ETH_NETWORK_CONFIG_RECEIVE_1536_BYTE_FRAMES_Msk |
      ETH_NETWORK_CONFIG_MULTICAST_HASH_ENABLE_Msk;

   //DMA configuration
   ETH1->GMAC_DCFGR = GMAC_DCFGR_DRBS(XMC7200_ETH2_RX_BUFFER_SIZE / 64) |
      GMAC_DCFGR_TXPBMS_Msk | GMAC_DCFGR_RXBMS_FULL | GMAC_DCFGR_FBLDO_INCR4;

   ETH1->DMA_RXBUF_SIZE_Q1 = XMC7200_ETH2_DUMMY_BUFFER_SIZE / 64;
   ETH1->DMA_RXBUF_SIZE_Q2 = XMC7200_ETH2_DUMMY_BUFFER_SIZE / 64;

   //Initialize buffer descriptors
   xmc7200Eth2InitBufferDesc(interface);

   //Clear transmit status register
   ETH1->TRANSMIT_STATUS = ETH_TRANSMIT_STATUS_RESP_NOT_OK123_Msk |
      ETH_TRANSMIT_STATUS_LATE_COLLISION_OCCURRED_Msk |
      ETH_TRANSMIT_STATUS_TRANSMIT_UNDER_RUN123_Msk |
      ETH_TRANSMIT_STATUS_TRANSMIT_COMPLETE123_Msk |
      ETH_TRANSMIT_STATUS_AMBA_ERROR123_Msk |
      ETH_TRANSMIT_STATUS_TRANSMIT_GO_Msk |
      ETH_TRANSMIT_STATUS_RETRY_LIMIT_EXCEEDED_Msk |
      ETH_TRANSMIT_STATUS_COLLISION_OCCURRED_Msk |
      ETH_TRANSMIT_STATUS_USED_BIT_READ_Msk;

   //Clear receive status register
   ETH1->RECEIVE_STATUS = ETH_RECEIVE_STATUS_RESP_NOT_OK1234_Msk |
      ETH_RECEIVE_STATUS_RECEIVE_OVERRUN123_Msk |
      ETH_RECEIVE_STATUS_FRAME_RECEIVED_Msk |
      ETH_RECEIVE_STATUS_BUFFER_NOT_AVAILABLE_Msk;

   //First disable all GMAC interrupts
   ETH1->INT_DISABLE = 0xFFFFFFFF;
   ETH1->INT_Q1_DISABLE = 0xFFFFFFFF;
   ETH1->INT_Q2_DISABLE = 0xFFFFFFFF;

   //Only the desired ones are enabled
   ETH1->INT_ENABLE = ETH_INT_ENABLE_ENABLE_RESP_NOT_OK_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_RECEIVE_OVERRUN_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_TRANSMIT_COMPLETE_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_TRANSMIT_FRAME_CORRUPTION_DUE_TO_AMBA_ERROR_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_RETRY_LIMIT_EXCEEDED_OR_LATE_COLLISION_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_TRANSMIT_BUFFER_UNDER_RUN_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_RECEIVE_USED_BIT_READ_INTERRUPT_Msk |
      ETH_INT_ENABLE_ENABLE_RECEIVE_COMPLETE_INTERRUPT_Msk;

   //Read INT_STATUS register to clear any pending interrupt
   status = ETH1->INT_STATUS;
   status = ETH1->INT_Q1_STATUS;
   status = ETH1->INT_Q2_STATUS;
   (void) status;

   //Set priority grouping (3 bits for pre-emption priority, no bits for subpriority)
   NVIC_SetPriorityGrouping(XMC7200_ETH2_IRQ_PRIORITY_GROUPING);

   //Configure GMAC interrupt priority
   NVIC_SetPriority(GMAC_IRQn, NVIC_EncodePriority(XMC7200_ETH2_IRQ_PRIORITY_GROUPING,
      XMC7200_ETH2_IRQ_GROUP_PRIORITY, XMC7200_ETH2_IRQ_SUB_PRIORITY));

   //Enable the GMAC to transmit and receive data
   ETH1->NETWORK_CONTROL |= ETH_NETWORK_CONTROL_ENABLE_TRANSMIT_Msk |
      ETH_NETWORK_CONTROL_ENABLE_RECEIVE_Msk;

   //Accept any packets from the upper layer
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

__weak_func void xmc7200Eth2InitGpio(NetInterface *interface)
{
//KIT-XMC72-EVK evaluation board?
#if defined(USE_KIT_XMC72_EVK)
#endif
}


/**
 * @brief Initialize buffer descriptors
 * @param[in] interface Underlying network interface
 **/

void xmc7200Eth2InitBufferDesc(NetInterface *interface)
{
   uint_t i;
   uint32_t address;

   //Initialize TX buffer descriptors
   for(i = 0; i < XMC7200_ETH2_TX_BUFFER_COUNT; i++)
   {
      //Calculate the address of the current TX buffer
      address = (uint32_t) txBuffer[i];
      //Write the address to the descriptor entry
      txBufferDesc[i].address = address;
      //Initialize status field
      txBufferDesc[i].status = ETH_TX_USED;
   }

   //Mark the last descriptor entry with the wrap flag
   txBufferDesc[i - 1].status |= ETH_TX_WRAP;
   //Initialize TX buffer index
   txBufferIndex = 0;

   //Initialize RX buffer descriptors
   for(i = 0; i < XMC7200_ETH2_RX_BUFFER_COUNT; i++)
   {
      //Calculate the address of the current RX buffer
      address = (uint32_t) rxBuffer[i];
      //Write the address to the descriptor entry
      rxBufferDesc[i].address = address & ETH_RX_ADDRESS;
      //Clear status field
      rxBufferDesc[i].status = 0;
   }

   //Mark the last descriptor entry with the wrap flag
   rxBufferDesc[i - 1].address |= ETH_RX_WRAP;
   //Initialize RX buffer index
   rxBufferIndex = 0;

   //Initialize dummy TX buffer descriptors
   for(i = 0; i < XMC7200_ETH2_DUMMY_BUFFER_COUNT; i++)
   {
      //Calculate the address of the current TX buffer
      address = (uint32_t) dummyTxBuffer[i];
      //Write the address to the descriptor entry
      dummyTxBufferDesc[i].address = address;
      //Initialize status field
      dummyTxBufferDesc[i].status = ETH_TX_USED;
   }

   //Mark the last descriptor entry with the wrap flag
   dummyTxBufferDesc[i - 1].status |= ETH_TX_WRAP;

   //Initialize dummy RX buffer descriptors
   for(i = 0; i < XMC7200_ETH2_DUMMY_BUFFER_COUNT; i++)
   {
      //Calculate the address of the current RX buffer
      address = (uint32_t) dummyRxBuffer[i];
      //Write the address to the descriptor entry
      dummyRxBufferDesc[i].address = (address & ETH_RX_ADDRESS) | ETH_RX_OWNERSHIP;
      //Clear status field
      dummyRxBufferDesc[i].status = 0;
   }

   //Mark the last descriptor entry with the wrap flag
   dummyRxBufferDesc[i - 1].address |= ETH_RX_WRAP;

   //Start location of the TX descriptor list
   ETH1->TRANSMIT_Q_PTR = (uint32_t) txBufferDesc;
   ETH1->TRANSMIT_Q1_PTR = (uint32_t) dummyTxBufferDesc;
   ETH1->TRANSMIT_Q2_PTR = (uint32_t) dummyTxBufferDesc;

   //Start location of the RX descriptor list
   ETH1->RECEIVE_Q_PTR = (uint32_t) rxBufferDesc;
   ETH1->RECEIVE_Q1_PTR = (uint32_t) dummyRxBufferDesc;
   ETH1->RECEIVE_Q2_PTR = (uint32_t) dummyRxBufferDesc;
}


/**
 * @brief XMC7200 Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to handle periodic
 * operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void xmc7200Eth2Tick(NetInterface *interface)
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

void xmc7200Eth2EnableIrq(NetInterface *interface)
{
   //Enable Ethernet MAC interrupts
   NVIC_EnableIRQ(GMAC_IRQn);

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

void xmc7200Eth2DisableIrq(NetInterface *interface)
{
   //Disable Ethernet MAC interrupts
   NVIC_DisableIRQ(GMAC_IRQn);

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
 * @brief XMC7200 Ethernet MAC interrupt service routine
 **/

void GMAC_Handler(void)
{
   bool_t flag;
   volatile uint32_t isr;
   volatile uint32_t tsr;
   volatile uint32_t rsr;

   //Interrupt service routine prologue
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Each time the software reads INT_STATUS, it has to check the contents
   //of TRANSMIT_STATUS, RECEIVE_STATUS and NETWORK_STATUS
   isr = ETH1->INT_Q1_STATUS;
   isr = ETH1->INT_Q2_STATUS;
   isr = ETH1->INT_STATUS;
   tsr = ETH1->TRANSMIT_STATUS;
   rsr = ETH1->RECEIVE_STATUS;
   (void) isr;

   //Packet transmitted?
   if((tsr & (ETH_TRANSMIT_STATUS_RESP_NOT_OK123_Msk |
      ETH_TRANSMIT_STATUS_LATE_COLLISION_OCCURRED_Msk |
      ETH_TRANSMIT_STATUS_TRANSMIT_UNDER_RUN123_Msk |
      ETH_TRANSMIT_STATUS_TRANSMIT_COMPLETE123_Msk |
      ETH_TRANSMIT_STATUS_AMBA_ERROR123_Msk |
      ETH_TRANSMIT_STATUS_TRANSMIT_GO_Msk |
      ETH_TRANSMIT_STATUS_RETRY_LIMIT_EXCEEDED_Msk |
      ETH_TRANSMIT_STATUS_COLLISION_OCCURRED_Msk |
      ETH_TRANSMIT_STATUS_USED_BIT_READ_Msk)) != 0)
   {
      //Only clear TRANSMIT_STATUS flags that are currently set
      ETH1->TRANSMIT_STATUS = tsr;

      //Check whether the TX buffer is available for writing
      if((txBufferDesc[txBufferIndex].status & GMAC_TX_USED) != 0)
      {
         //Notify the TCP/IP stack that the transmitter is ready to send
         flag |= osSetEventFromIsr(&nicDriverInterface->nicTxEvent);
      }
   }

   //Packet received?
   if((rsr & (ETH_RECEIVE_STATUS_RESP_NOT_OK1234_Msk |
      ETH_RECEIVE_STATUS_RECEIVE_OVERRUN123_Msk |
      ETH_RECEIVE_STATUS_FRAME_RECEIVED_Msk |
      ETH_RECEIVE_STATUS_BUFFER_NOT_AVAILABLE_Msk)) != 0)
   {
      //Set event flag
      nicDriverInterface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Interrupt service routine epilogue
   osExitIsr(flag);
}


/**
 * @brief XMC7200 Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void xmc7200Eth2EventHandler(NetInterface *interface)
{
   error_t error;
   uint32_t rsr;

   //Read receive status
   rsr = ETH1->RECEIVE_STATUS;

   //Packet received?
   if((rsr & (ETH_RECEIVE_STATUS_RESP_NOT_OK1234_Msk |
      ETH_RECEIVE_STATUS_RECEIVE_OVERRUN123_Msk |
      ETH_RECEIVE_STATUS_FRAME_RECEIVED_Msk |
      ETH_RECEIVE_STATUS_BUFFER_NOT_AVAILABLE_Msk)) != 0)
   {
      //Only clear RECEIVE_STATUS flags that are currently set
      ETH1->RECEIVE_STATUS = rsr;

      //Process all pending packets
      do
      {
         //Read incoming packet
         error = xmc7200Eth2ReceivePacket(interface);

         //No more data in the receive buffer?
      } while(error != ERROR_BUFFER_EMPTY);
   }
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

error_t xmc7200Eth2SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary)
{
   static uint32_t temp[XMC7200_ETH2_TX_BUFFER_SIZE / 4];
   size_t length;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > XMC7200_ETH2_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return ERROR_INVALID_LENGTH;
   }

   //Make sure the current buffer is available for writing
   if((txBufferDesc[txBufferIndex].status & ETH_TX_USED) == 0)
   {
      return ERROR_FAILURE;
   }

   //Copy user data to the transmit buffer
   netBufferRead(temp, buffer, offset, length);
   osMemcpy(txBuffer[txBufferIndex], temp, (length + 3) & ~3UL);

   //Set the necessary flags in the descriptor entry
   if(txBufferIndex < (XMC7200_ETH2_TX_BUFFER_COUNT - 1))
   {
      //Write the status word
      txBufferDesc[txBufferIndex].status = ETH_TX_LAST |
         (length & ETH_TX_LENGTH);

      //Point to the next buffer
      txBufferIndex++;
   }
   else
   {
      //Write the status word
      txBufferDesc[txBufferIndex].status = ETH_TX_WRAP | ETH_TX_LAST |
         (length & ETH_TX_LENGTH);

      //Wrap around
      txBufferIndex = 0;
   }

   //Data synchronization barrier
   __DSB();

   //Set the TSTART bit to initiate transmission
   ETH1->NETWORK_CONTROL |= ETH_NETWORK_CONTROL_TX_START_PCLK_Msk;

   //Check whether the next buffer is available for writing
   if((txBufferDesc[txBufferIndex].status & GMAC_TX_USED) != 0)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t xmc7200Eth2ReceivePacket(NetInterface *interface)
{
   static uint32_t temp[ETH_MAX_FRAME_SIZE / 4];
   error_t error;
   uint_t i;
   uint_t j;
   uint_t sofIndex;
   uint_t eofIndex;
   size_t n;
   size_t size;
   size_t length;

   //Initialize variables
   size = 0;
   sofIndex = UINT_MAX;
   eofIndex = UINT_MAX;

   //Search for SOF and EOF flags
   for(i = 0; i < XMC7200_ETH2_RX_BUFFER_COUNT; i++)
   {
      //Point to the current entry
      j = rxBufferIndex + i;

      //Wrap around to the beginning of the buffer if necessary
      if(j >= XMC7200_ETH2_RX_BUFFER_COUNT)
      {
         j -= XMC7200_ETH2_RX_BUFFER_COUNT;
      }

      //No more entries to process?
      if((rxBufferDesc[j].address & ETH_RX_OWNERSHIP) == 0)
      {
         //Stop processing
         break;
      }

      //A valid SOF has been found?
      if((rxBufferDesc[j].status & ETH_RX_SOF) != 0)
      {
         //Save the position of the SOF
         sofIndex = i;
      }

      //A valid EOF has been found?
      if((rxBufferDesc[j].status & ETH_RX_EOF) != 0 && sofIndex != UINT_MAX)
      {
         //Save the position of the EOF
         eofIndex = i;
         //Retrieve the length of the frame
         size = rxBufferDesc[j].status & ETH_RX_LENGTH;
         //Limit the number of data to read
         size = MIN(size, ETH_MAX_FRAME_SIZE);
         //Stop processing since we have reached the end of the frame
         break;
      }
   }

   //Determine the number of entries to process
   if(eofIndex != UINT_MAX)
   {
      j = eofIndex + 1;
   }
   else if(sofIndex != UINT_MAX)
   {
      j = sofIndex;
   }
   else
   {
      j = i;
   }

   //Total number of bytes that have been copied from the receive buffer
   length = 0;

   //Process incoming frame
   for(i = 0; i < j; i++)
   {
      //Any data to copy from current buffer?
      if(eofIndex != UINT_MAX && i >= sofIndex && i <= eofIndex)
      {
         //Calculate the number of bytes to read at a time
         n = MIN(size, XMC7200_ETH2_RX_BUFFER_SIZE);
         //Copy data from receive buffer
         osMemcpy((uint8_t *) temp + length, rxBuffer[rxBufferIndex], (n + 3) & ~3UL);
         //Update byte counters
         length += n;
         size -= n;
      }

      //Mark the current buffer as free
      rxBufferDesc[rxBufferIndex].address &= ~ETH_RX_OWNERSHIP;

      //Point to the following entry
      rxBufferIndex++;

      //Wrap around to the beginning of the buffer if necessary
      if(rxBufferIndex >= XMC7200_ETH2_RX_BUFFER_COUNT)
      {
         rxBufferIndex = 0;
      }
   }

   //Any packet to process?
   if(length > 0)
   {
      NetRxAncillary ancillary;

      //Additional options can be passed to the stack along with the packet
      ancillary = NET_DEFAULT_RX_ANCILLARY;

      //Pass the packet to the upper layer
      nicProcessPacket(interface, (uint8_t *) temp, length, &ancillary);
      //Valid packet received
      error = NO_ERROR;
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

error_t xmc7200Eth2UpdateMacAddrFilter(NetInterface *interface)
{
   uint_t i;
   uint_t j;
   uint_t k;
   uint8_t *p;
   uint32_t hashTable[2];
   MacAddr unicastMacAddr[3];
   MacFilterEntry *entry;

   //Debug message
   TRACE_DEBUG("Updating MAC filter...\r\n");

   //Set the MAC address of the station
   ETH1->SPEC_ADD1_BOTTOM = interface->macAddr.w[0] | (interface->macAddr.w[1] << 16);
   ETH1->SPEC_ADD1_TOP = interface->macAddr.w[2];

   //The MAC supports 3 additional addresses for unicast perfect filtering
   unicastMacAddr[0] = MAC_UNSPECIFIED_ADDR;
   unicastMacAddr[1] = MAC_UNSPECIFIED_ADDR;
   unicastMacAddr[2] = MAC_UNSPECIFIED_ADDR;

   //The hash table is used for multicast address filtering
   hashTable[0] = 0;
   hashTable[1] = 0;

   //The MAC address filter contains the list of MAC addresses to accept
   //when receiving an Ethernet frame
   for(i = 0, j = 0; i < MAC_ADDR_FILTER_SIZE; i++)
   {
      //Point to the current entry
      entry = &interface->macAddrFilter[i];

      //Valid entry?
      if(entry->refCount > 0)
      {
         //Multicast address?
         if(macIsMulticastAddr(&entry->addr))
         {
            //Point to the MAC address
            p = entry->addr.b;

            //Apply the hash function
            k = (p[0] >> 6) ^ p[0];
            k ^= (p[1] >> 4) ^ (p[1] << 2);
            k ^= (p[2] >> 2) ^ (p[2] << 4);
            k ^= (p[3] >> 6) ^ p[3];
            k ^= (p[4] >> 4) ^ (p[4] << 2);
            k ^= (p[5] >> 2) ^ (p[5] << 4);

            //The hash value is reduced to a 6-bit index
            k &= 0x3F;

            //Update hash table contents
            hashTable[k / 32] |= (1 << (k % 32));
         }
         else
         {
            //Up to 3 additional MAC addresses can be specified
            if(j < 3)
            {
               //Save the unicast address
               unicastMacAddr[j] = entry->addr;
            }
            else
            {
               //Point to the MAC address
               p = entry->addr.b;

               //Apply the hash function
               k = (p[0] >> 6) ^ p[0];
               k ^= (p[1] >> 4) ^ (p[1] << 2);
               k ^= (p[2] >> 2) ^ (p[2] << 4);
               k ^= (p[3] >> 6) ^ p[3];
               k ^= (p[4] >> 4) ^ (p[4] << 2);
               k ^= (p[5] >> 2) ^ (p[5] << 4);

               //The hash value is reduced to a 6-bit index
               k &= 0x3F;

               //Update hash table contents
               hashTable[k / 32] |= (1 << (k % 32));
            }

            //Increment the number of unicast addresses
            j++;
         }
      }
   }

   //Configure the first unicast address filter
   if(j >= 1)
   {
      //The address is activated when SAT register is written
      ETH1->SPEC_ADD2_BOTTOM = unicastMacAddr[0].w[0] | (unicastMacAddr[0].w[1] << 16);
      ETH1->SPEC_ADD2_TOP = unicastMacAddr[0].w[2];
   }
   else
   {
      //The address is deactivated when SAB register is written
      ETH1->SPEC_ADD2_BOTTOM = 0;
   }

   //Configure the second unicast address filter
   if(j >= 2)
   {
      //The address is activated when SAT register is written
      ETH1->SPEC_ADD3_BOTTOM = unicastMacAddr[1].w[0] | (unicastMacAddr[1].w[1] << 16);
      ETH1->SPEC_ADD3_TOP = unicastMacAddr[1].w[2];
   }
   else
   {
      //The address is deactivated when SAB register is written
      ETH1->SPEC_ADD3_BOTTOM = 0;
   }

   //Configure the third unicast address filter
   if(j >= 3)
   {
      //The address is activated when SAT register is written
      ETH1->SPEC_ADD4_BOTTOM = unicastMacAddr[2].w[0] | (unicastMacAddr[2].w[1] << 16);
      ETH1->SPEC_ADD4_TOP = unicastMacAddr[2].w[2];
   }
   else
   {
      //The address is deactivated when SAB register is written
      ETH1->SPEC_ADD4_BOTTOM = 0;
   }

   //The perfect MAC filter supports only 3 unicast addresses
   if(j >= 4)
   {
      ETH1->NETWORK_CONFIG |= ETH_NETWORK_CONFIG_UNICAST_HASH_ENABLE_Msk;
   }
   else
   {
      ETH1->NETWORK_CONFIG &= ~ETH_NETWORK_CONFIG_UNICAST_HASH_ENABLE_Msk;
   }

   //Configure the multicast hash table
   ETH1->HASH_BOTTOM = hashTable[0];
   ETH1->HASH_TOP = hashTable[1];

   //Debug message
   TRACE_DEBUG("  HASH_BOTTOM = %08" PRIX32 "\r\n", ETH1->HASH_BOTTOM);
   TRACE_DEBUG("  HASH_TOP = %08" PRIX32 "\r\n", ETH1->HASH_TOP);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t xmc7200Eth2UpdateMacConfig(NetInterface *interface)
{
   uint32_t config;

   //Read network configuration register
   config = ETH1->NETWORK_CONFIG;

   //1000BASE-T operation mode?
   if(interface->linkSpeed == NIC_LINK_SPEED_1GBPS)
   {
      config |= ETH_NETWORK_CONFIG_GIGABIT_MODE_ENABLE_Msk;
      config &= ~ETH_NETWORK_CONFIG_SPEED_Msk;
   }
   //100BASE-TX operation mode?
   else if(interface->linkSpeed == NIC_LINK_SPEED_100MBPS)
   {
      config &= ~ETH_NETWORK_CONFIG_GIGABIT_MODE_ENABLE_Msk;
      config |= ETH_NETWORK_CONFIG_SPEED_Msk;
   }
   //10BASE-T operation mode?
   else
   {
      config &= ~ETH_NETWORK_CONFIG_GIGABIT_MODE_ENABLE_Msk;
      config &= ~ETH_NETWORK_CONFIG_SPEED_Msk;
   }

   //Half-duplex or full-duplex mode?
   if(interface->duplexMode == NIC_FULL_DUPLEX_MODE)
   {
      config |= ETH_NETWORK_CONFIG_FULL_DUPLEX_Msk;
   }
   else
   {
      config &= ~ETH_NETWORK_CONFIG_FULL_DUPLEX_Msk;
   }

   //Write configuration value back to ETH_NETWORK_CONFIG register
   ETH1->NETWORK_CONFIG = config;

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

void xmc7200Eth2WritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data)
{
   uint32_t temp;

   //Valid opcode?
   if(opcode == SMI_OPCODE_WRITE)
   {
      //Set up a write operation
      temp = ETH_PHY_MANAGEMENT_WRITE1_Msk | (1 << ETH_PHY_MANAGEMENT_OPERATION_Pos) |
         (2 << ETH_PHY_MANAGEMENT_WRITE10_Pos);

      //PHY address
      temp |= (phyAddr << ETH_PHY_MANAGEMENT_PHY_ADDRESS_Pos) &
         ETH_PHY_MANAGEMENT_PHY_ADDRESS_Msk;

      //Register address
      temp |= (regAddr << ETH_PHY_MANAGEMENT_REGISTER_ADDRESS_Pos) &
         ETH_PHY_MANAGEMENT_REGISTER_ADDRESS_Msk;

      //Register value
      temp |= data & ETH_PHY_MANAGEMENT_PHY_WRITE_READ_DATA_Msk;

      //Start a write operation
      ETH1->PHY_MANAGEMENT = temp;
      //Wait for the write to complete
      while((ETH1->NETWORK_STATUS & ETH_NETWORK_STATUS_MAN_DONE_Msk) == 0)
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

uint16_t xmc7200Eth2ReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr)
{
   uint16_t data;
   uint32_t temp;

   //Valid opcode?
   if(opcode == SMI_OPCODE_READ)
   {
      //Set up a read operation
      temp = ETH_PHY_MANAGEMENT_WRITE1_Msk | (2 << ETH_PHY_MANAGEMENT_OPERATION_Pos) |
         (2 << ETH_PHY_MANAGEMENT_WRITE10_Pos);

      //PHY address
      temp |= (phyAddr << ETH_PHY_MANAGEMENT_PHY_ADDRESS_Pos) &
         ETH_PHY_MANAGEMENT_PHY_ADDRESS_Msk;

      //Register address
      temp |= (regAddr << ETH_PHY_MANAGEMENT_REGISTER_ADDRESS_Pos) &
         ETH_PHY_MANAGEMENT_REGISTER_ADDRESS_Msk;

      //Start a read operation
      ETH1->PHY_MANAGEMENT = temp;
      //Wait for the read to complete
      while((ETH1->NETWORK_STATUS & ETH_NETWORK_STATUS_MAN_DONE_Msk) == 0)
      {
      }

      //Get register value
      data = ETH1->PHY_MANAGEMENT & ETH_PHY_MANAGEMENT_PHY_WRITE_READ_DATA_Msk;
   }
   else
   {
      //The MAC peripheral only supports standard Clause 22 opcodes
      data = 0;
   }

   //Return the value of the PHY register
   return data;
}
