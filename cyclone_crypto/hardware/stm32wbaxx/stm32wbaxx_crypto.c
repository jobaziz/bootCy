/**
 * @file stm32wbaxx_crypto.c
 * @brief STM32WBA hardware cryptographic accelerator
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
#include "core/crypto.h"
#include "hardware/stm32wbaxx/stm32wbaxx_crypto.h"
#include "hardware/stm32wbaxx/stm32wbaxx_crypto_trng.h"
#include "hardware/stm32wbaxx/stm32wbaxx_crypto_hash.h"
#include "hardware/stm32wbaxx/stm32wbaxx_crypto_cipher.h"
#include "hardware/stm32wbaxx/stm32wbaxx_crypto_pkc.h"
#include "debug.h"

//Global variables
OsMutex stm32wbaxxCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t stm32wbaxxCryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&stm32wbaxxCryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

#if (STM32WBAXX_CRYPTO_TRNG_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize TRNG module
      error = trngInit();
   }
#endif

#if (STM32WBAXX_CRYPTO_HASH_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize HASH module
      error = hashInit();
   }
#endif

#if (STM32WBAXX_CRYPTO_CIPHER_SUPPORT == ENABLED && AES_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize CRYP module
      error = crypInit();
   }
#endif

#if (STM32WBAXX_CRYPTO_PKC_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize PKA module
      error = pkaInit();
   }
#endif

   //Return status code
   return error;
}
