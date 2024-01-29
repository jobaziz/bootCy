/**
 * @file lpc54xxx_crypto.c
 * @brief LPC54000 hardware cryptographic accelerator
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
#include "fsl_device_registers.h"
#include "fsl_sha.h"
#include "core/crypto.h"
#include "hardware/lpc54xxx/lpc54xxx_crypto.h"
#include "hardware/lpc54xxx/lpc54xxx_crypto_trng.h"
#include "hardware/lpc54xxx/lpc54xxx_crypto_hash.h"
#include "debug.h"

//Global variables
OsMutex lpc54xxxCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t lpc54xxxCryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&lpc54xxxCryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

#if (LPC54XXX_CRYPTO_TRNG_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize TRNG module
      error = trngInit();
   }
#endif

#if (LPC54XXX_CRYPTO_HASH_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable SHA peripheral clock
      SHA_ClkInit(SHA0);
   }
#endif

   //Return status code
   return error;
}
