/**
 * @file stm32l4xx_crypto_trng.c
 * @brief STM32L4 true random number generator
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "core/crypto.h"
#include "hardware/stm32l4xx/stm32l4xx_crypto.h"
#include "hardware/stm32l4xx/stm32l4xx_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (STM32L4XX_CRYPTO_TRNG_SUPPORT == ENABLED)

//Global variable
static RNG_HandleTypeDef RNG_Handle;


/**
 * @brief TRNG module initialization
 * @return Error code
 **/

error_t trngInit(void)
{
   HAL_StatusTypeDef status;

   //Enable RNG peripheral clock
   __HAL_RCC_RNG_CLK_ENABLE();

   //Set instance
   RNG_Handle.Instance = RNG;

   //Reset RNG module
   status = HAL_RNG_DeInit(&RNG_Handle);

   //Check status code
   if(status == HAL_OK)
   {
      //Initialize RNG module
      status = HAL_RNG_Init(&RNG_Handle);
   }

   //Return status code
   return (status == HAL_OK) ? NO_ERROR : ERROR_FAILURE;
}


/**
 * @brief Get random data from the TRNG module
 * @param[out] data Buffer where to store random data
 * @param[in] length Number of random bytes to generate
 **/

error_t trngGetRandomData(uint8_t *data, size_t length)
{
   size_t i;
   uint32_t value;
   HAL_StatusTypeDef status;

   //Initialize status code
   status = HAL_OK;

   //Acquire exclusive access to the RNG module
   osAcquireMutex(&stm32l4xxCryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 32-bit random value when necessary
      if((i % 4) == 0)
      {
         //Get 32-bit random value
         status = HAL_RNG_GenerateRandomNumber(&RNG_Handle, &value);
         //Check status code
         if(status != HAL_OK)
         {
            break;
         }
      }

      //Copy random byte
      data[i] = value & 0xFF;
      //Shift the 32-bit random value
      value >>= 8;
   }

   //Release exclusive access to the RNG module
   osReleaseMutex(&stm32l4xxCryptoMutex);

   //Return status code
   return (status == HAL_OK) ? NO_ERROR : ERROR_FAILURE;
}

#endif
