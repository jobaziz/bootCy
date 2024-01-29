/**
 * @file efm32gg11_crypto.c
 * @brief EFM32 Giant Gecko 11 hardware cryptographic accelerator
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
#include "em_device.h"
#include "em_cmu.h"
#include "core/crypto.h"
#include "hardware/efm32gg11/efm32gg11_crypto.h"
#include "hardware/efm32gg11/efm32gg11_crypto_trng.h"
#include "hardware/efm32gg11/efm32gg11_crypto_hash.h"
#include "hardware/efm32gg11/efm32gg11_crypto_cipher.h"
#include "hardware/efm32gg11/efm32gg11_crypto_pkc.h"
#include "debug.h"

//Global variables
OsMutex efm32gg11CryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t efm32gg11CryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&efm32gg11CryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

#if (EFM32GG11_CRYPTO_TRNG_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize TRNG module
      error = trngInit();
   }
#endif

#if (EFM32GG11_CRYPTO_HASH_SUPPORT == ENABLED || \
   EFM32GG11_CRYPTO_CIPHER_SUPPORT == ENABLED || \
   EFM32GG11_CRYPTO_PKC_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable CRYPTO clock
      CMU_ClockEnable(cmuClock_CRYPTO0, true);
   }
#endif

   //Return status code
   return error;
}
