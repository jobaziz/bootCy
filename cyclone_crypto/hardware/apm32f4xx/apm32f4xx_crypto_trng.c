/**
 * @file apm32f4xx_crypto_trng.c
 * @brief APM32F4 true random number generator
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCRYPTO Eval.
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
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "apm32f4xx.h"
#include "apm32f4xx_rcm.h"
#include "apm32f4xx_rng.h"
#include "core/crypto.h"
#include "hardware/apm32f4xx/apm32f4xx_crypto.h"
#include "hardware/apm32f4xx/apm32f4xx_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (APM32F4XX_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief TRNG module initialization
 * @return Error code
 **/

error_t trngInit(void)
{
   //Enable RNG peripheral clock
   RCM_EnableAHB2PeriphClock(RCM_AHB2_PERIPH_RNG);

   //Reset RNG module
   RNG_Reset();
   //Enable RNG module
   RNG_Enable();

   //Successful initialization
   return NO_ERROR;
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

   //Acquire exclusive access to the RNG module
   osAcquireMutex(&apm32f4xxCryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 32-bit random value when necessary
      if((i % 4) == 0)
      {
         //Wait for the RNG to contain a valid data
         while(RNG_ReadStatusFlag(RNG_FLAG_DATARDY) == RESET)
         {
         }

         //Get 32-bit random value
         value = RNG_ReadRandomNumber();
      }

      //Copy random byte
      data[i] = value & 0xFF;
      //Shift the 32-bit random value
      value >>= 8;
   }

   //Release exclusive access to the RNG module
   osReleaseMutex(&apm32f4xxCryptoMutex);

   //Successful processing
   return NO_ERROR;
}

#endif
