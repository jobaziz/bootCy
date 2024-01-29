/**
 * @file s7g2_crypto_trng.c
 * @brief Synergy S7G2 true random number generator
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
#include "hw_sce_trng_private.h"
#include "core/crypto.h"
#include "hardware/s7g2/s7g2_crypto.h"
#include "hardware/s7g2/s7g2_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (S7G2_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief Get random data from the TRNG module
 * @param[out] data Buffer where to store random data
 * @param[in] length Number of random bytes to generate
 **/

error_t trngGetRandomData(uint8_t *data, size_t length)
{
   size_t i;
   size_t j;
   uint32_t value[4];
   ssp_err_t status;

   //Acquire exclusive access to the SCE7 module
   osAcquireMutex(&s7g2CryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 128-bit random value when necessary
      if((i % 16) == 0)
      {
         //Get 128-bit random value
         status = HW_SCE_RNG_Read(value);
         //Check status code
         if(status != SSP_SUCCESS)
         {
            break;
         }
      }

      //Extract a random byte
      j = (i % 16) / 4;

      //Copy random byte
      data[i] = value[j] & 0xFF;
      //Shift the 32-bit random value
      value[j] >>= 8;
   }

   //Release exclusive access to the SCE7 module
   osReleaseMutex(&s7g2CryptoMutex);

   //Return status code
   return (status == SSP_SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}

#endif
