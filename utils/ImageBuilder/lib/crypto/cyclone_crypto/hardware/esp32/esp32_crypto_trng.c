/**
 * @file esp32_crypto_trng.c
 * @brief ESP32 true random number generator
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
#include "esp_system.h"
#include "driver/periph_ctrl.h"
#include "core/crypto.h"
#include "hardware/esp32/esp32_crypto.h"
#include "hardware/esp32/esp32_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (ESP32_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief RNG module initialization
 **/

void esp32RngInit(void)
{
   //Enable RNG module
   periph_module_enable(PERIPH_RNG_MODULE);
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

   //Initialize variable
   value = 0;

   //Acquire exclusive access to the RNG module
   osAcquireMutex(&esp32CryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 32-bit random value when necessary
      if((i % 4) == 0)
      {
         //Get the 32-bit random value
         value = esp_random();
      }

      //Copy random byte
      data[i] = value & 0xFF;
      //Shift the 32-bit random value
      value >>= 8;
   }

   //Release exclusive access to the RNG module
   osReleaseMutex(&esp32CryptoMutex);

   //Successful processing
   return NO_ERROR;
}

#endif
