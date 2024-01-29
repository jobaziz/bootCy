/**
 * @file sama5d2_crypto_trng.c
 * @brief SAMA5D2 true random number generator
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
#include "chip.h"
#include "core/crypto.h"
#include "hardware/sama5d2/sama5d2_crypto.h"
#include "hardware/sama5d2/sama5d2_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (SAMA5D2_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief TRNG module initialization
 * @return Error code
 **/

error_t trngInit(void)
{
   //Enable TRNG peripheral clock
   PMC->PMC_PCER1 = (1 << (ID_TRNG - 32));
   //Enable TRNG
   TRNG->TRNG_CR = TRNG_CR_KEY(0x524E47) | TRNG_CR_ENABLE;

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

   //Acquire exclusive access to the TRNG module
   osAcquireMutex(&sama5d2CryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 32-bit random value when necessary
      if((i % 4) == 0)
      {
         //Wait for the TRNG to contain a valid data
         while((TRNG->TRNG_ISR & TRNG_ISR_DATRDY) == 0)
         {
         }

         //Get the 32-bit random value
         value = TRNG->TRNG_ODATA;
      }

      //Copy random byte
      data[i] = value & 0xFF;
      //Shift the 32-bit random value
      value >>= 8;
   }

   //Release exclusive access to the TRNG module
   osReleaseMutex(&sama5d2CryptoMutex);

   //Successful processing
   return NO_ERROR;
}

#endif
