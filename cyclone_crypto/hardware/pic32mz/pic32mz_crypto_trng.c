/**
 * @file pic32mz_crypto_trng.c
 * @brief PIC32MZ true random number generator
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
#include <p32xxxx.h>
#include "core/crypto.h"
#include "hardware/pic32mz/pic32mz_crypto.h"
#include "hardware/pic32mz/pic32mz_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (PIC32MZ_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief TRNG module initialization
 * @return Error code
 **/

error_t trngInit(void)
{
   //Enable TRNG
   RNGCON |= _RNGCON_TRNGMODE_MASK | _RNGCON_TRNGEN_MASK;

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
   uint32_t value1;
   uint32_t value2;

   //Acquire exclusive access to the RNG module
   osAcquireMutex(&pic32mzCryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 64-bit random value when necessary
      if((i % 8) == 0)
      {
         //Wait for the RNG to contain a valid data
         while(RNGCNT < 64)
         {
         }

         //Get the 64-bit random value
         value2 = RNGSEED2;
         value1 = RNGSEED1;
      }

      //Extract a random byte
      if((i % 8) < 4)
      {
         //Copy random byte
         data[i] = value1 & 0xFF;
         //Shift the 32-bit random value
         value1 >>= 8;
      }
      else
      {
         //Copy random byte
         data[i] = value2 & 0xFF;
         //Shift the 32-bit random value
         value2 >>= 8;
      }
   }

   //Release exclusive access to the RNG module
   osReleaseMutex(&pic32mzCryptoMutex);

   //Successful processing
   return NO_ERROR;
}

#endif
