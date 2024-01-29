/**
 * @file esp32_c3_crypto.c
 * @brief ESP32-C3 hardware cryptographic accelerator
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
#include "esp_private/periph_ctrl.h"
#include "core/crypto.h"
#include "hardware/esp32_c3/esp32_c3_crypto.h"
#include "hardware/esp32_c3/esp32_c3_crypto_trng.h"
#include "hardware/esp32_c3/esp32_c3_crypto_hash.h"
#include "hardware/esp32_c3/esp32_c3_crypto_cipher.h"
#include "hardware/esp32_c3/esp32_c3_crypto_pkc.h"
#include "debug.h"

//Global variables
OsMutex esp32c3CryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t esp32c3CryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&esp32c3CryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

   //Check status code
   if(!error)
   {
#if (ESP32_C3_CRYPTO_TRNG_SUPPORT == ENABLED)
      //Initialize RNG module
      esp32c3RngInit();
#endif

#if (ESP32_C3_CRYPTO_HASH_SUPPORT == ENABLED)
      //Initialize SHA module
      esp32c3ShaInit();
#endif

#if (ESP32_C3_CRYPTO_CIPHER_SUPPORT == ENABLED && AES_SUPPORT == ENABLED)
      //Initialize AES module
      esp32c3AesInit();
#endif

#if (ESP32_C3_CRYPTO_PKC_SUPPORT == ENABLED)
      //Initialize RSA module
      esp32c3RsaInit();
#endif
   }

   //Return status code
   return error;
}
