/**
 * @file smi_driver.c
 * @brief Serial management interface driver
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
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
 * @version 3.0.0
 **/

//Dependencies
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "core/net.h"
#include "smi_driver.h"
#include "debug.h"

//MDIO pin
#define SMI_MDIO_PIN GPIO_PIN_12
#define SMI_MDIO_GPIO GPIOJ

//MDC pin
#define SMI_MDC_PIN GPIO_PIN_13
#define SMI_MDC_GPIO GPIOJ


/**
 * @brief SMI driver
 **/

const SmiDriver smiDriver =
{
   smiInit,
   smiWritePhyReg,
   smiReadPhyReg
};


/**
 * @brief Initialize serial management interface
 * @return Error code
 **/

error_t smiInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   //Configure MDIO as an input
   GPIO_InitStructure.Pin = SMI_MDIO_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
   GPIO_InitStructure.Pull = GPIO_PULLUP;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
   HAL_GPIO_Init(SMI_MDIO_GPIO, &GPIO_InitStructure);

   //Configure MDC as an output
   GPIO_InitStructure.Pin = SMI_MDC_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
   HAL_GPIO_Init(SMI_MDC_GPIO, &GPIO_InitStructure);

   //Deassert MDC
   HAL_GPIO_WritePin(SMI_MDC_GPIO, SMI_MDC_PIN, GPIO_PIN_RESET);

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

void smiWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data)
{
   //Synchronization pattern
   smiWriteData(SMI_SYNC, 32);
   //Start of frame
   smiWriteData(SMI_START, 2);
   //Set up a write operation
   smiWriteData(opcode, 2);
   //Write PHY address
   smiWriteData(phyAddr, 5);
   //Write register address
   smiWriteData(regAddr, 5);
   //Turnaround
   smiWriteData(SMI_TA, 2);
   //Write register value
   smiWriteData(data, 16);
   //Release MDIO
   smiReadData(1);
}


/**
 * @brief Read PHY register
 * @param[in] opcode Access type (2 bits)
 * @param[in] phyAddr PHY address (5 bits)
 * @param[in] regAddr Register address (5 bits)
 * @return Register value
 **/

uint16_t smiReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr)
{
   uint16_t data;

   //Synchronization pattern
   smiWriteData(SMI_SYNC, 32);
   //Start of frame
   smiWriteData(SMI_START, 2);
   //Set up a read operation
   smiWriteData(opcode, 2);
   //Write PHY address
   smiWriteData(phyAddr, 5);
   //Write register address
   smiWriteData(regAddr, 5);
   //Turnaround to avoid contention
   smiReadData(1);
   //Read register value
   data = smiReadData(16);
   //Force the PHY to release the MDIO pin
   smiReadData(1);

   //Return the value of the PHY register
   return data;
}


/**
 * @brief SMI write operation
 * @param[in] data Raw data to be written
 * @param[in] length Number of bits to be written
 **/

void smiWriteData(uint32_t data, uint_t length)
{
   uint_t i;
   GPIO_InitTypeDef GPIO_InitStructure;

   //Skip the most significant bits since they are meaningless
   data <<= 32 - length;

   //Configure MDIO as an output
   GPIO_InitStructure.Pin = SMI_MDIO_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
   HAL_GPIO_Init(SMI_MDIO_GPIO, &GPIO_InitStructure);

   //Write the specified number of bits
   for(i = 0; i < length; i++)
   {
      //Write MDIO
      if(data & 0x80000000)
      {
         HAL_GPIO_WritePin(SMI_MDIO_GPIO, SMI_MDIO_PIN, GPIO_PIN_SET);
      }
      else
      {
         HAL_GPIO_WritePin(SMI_MDIO_GPIO, SMI_MDIO_PIN, GPIO_PIN_RESET);
      }

      //Delay
      usleep(1);
      //Assert MDC
      HAL_GPIO_WritePin(SMI_MDC_GPIO, SMI_MDC_PIN, GPIO_PIN_SET);
      //Delay
      usleep(1);
      //Deassert MDC
      HAL_GPIO_WritePin(SMI_MDC_GPIO, SMI_MDC_PIN, GPIO_PIN_RESET);

      //Rotate data
      data <<= 1;
   }
}


/**
 * @brief SMI read operation
 * @param[in] length Number of bits to be read
 * @return Data resulting from the MDIO read operation
 **/

uint32_t smiReadData(uint_t length)
{
   uint_t i;
   uint32_t data;
   GPIO_InitTypeDef GPIO_InitStructure;

   //Configure MDIO as an input
   GPIO_InitStructure.Pin = SMI_MDIO_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
   GPIO_InitStructure.Pull = GPIO_PULLUP;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
   HAL_GPIO_Init(SMI_MDIO_GPIO, &GPIO_InitStructure);

   //Initialize data
   data = 0;

   //Read the specified number of bits
   for(i = 0; i < length; i++)
   {
      //Rotate data
      data <<= 1;

      //Assert MDC
      HAL_GPIO_WritePin(SMI_MDC_GPIO, SMI_MDC_PIN, GPIO_PIN_SET);
      //Delay
      usleep(1);
      //Deassert MDC
      HAL_GPIO_WritePin(SMI_MDC_GPIO, SMI_MDC_PIN, GPIO_PIN_RESET);
      //Delay
      usleep(1);

      //Check MDIO state
      if(HAL_GPIO_ReadPin(SMI_MDIO_GPIO, SMI_MDIO_PIN))
      {
         data |= 0x00000001;
      }
   }

   //Return the received data
   return data;
}
