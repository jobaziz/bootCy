/**
 * @file sam4c_crypto.c
 * @brief SAM4C hardware cryptographic accelerator
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
#include "hardware/sam4c/sam4c_crypto.h"
#include "hardware/sam4c/sam4c_crypto_trng.h"
#include "hardware/sam4c/sam4c_crypto_hash.h"
#include "hardware/sam4c/sam4c_crypto_cipher.h"
#include "debug.h"

//Global variables
OsMutex sam4cCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t sam4cCryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&sam4cCryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

#if (SAM4C_CRYPTO_TRNG_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize TRNG module
      error = trngInit();
   }
#endif

#if (SAM4C_CRYPTO_HASH_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable ICM peripheral clock
      PMC->PMC_PCER1 = (1U << (ID_ICM - 32));
   }
#endif

#if (SAM4C_CRYPTO_CIPHER_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable AES peripheral clock
      PMC->PMC_PCER1 = (1U << (ID_AES - 32));
   }
#endif

   //Return status code
   return error;
}
