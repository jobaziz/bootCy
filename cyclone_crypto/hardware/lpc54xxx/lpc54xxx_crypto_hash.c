/**
 * @file lpc54xxx_crypto_hash.c
 * @brief LPC54000 hash hardware accelerator
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
#include "hardware/lpc54xxx/lpc54xxx_crypto_hash.h"
#include "hash/hash_algorithms.h"
#include "debug.h"

//Check crypto library configuration
#if (LPC54XXX_CRYPTO_HASH_SUPPORT == ENABLED)
#if (SHA1_SUPPORT == ENABLED)

/**
 * @brief Digest a message using SHA-1
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t sha1Compute(const void *data, size_t length, uint8_t *digest)
{
   size_t n;
   status_t status;
   sha_ctx_t context;

   //Size of the digest value
   n = SHA1_DIGEST_SIZE;

   //Acquire exclusive access to the SHA module
   osAcquireMutex(&lpc54xxxCryptoMutex);

   //Initialize hash computation
   status = SHA_Init(SHA0, &context, kSHA_Sha1);

   //Check status code
   if(status == kStatus_Success)
   {
      //Digest the message
      SHA_Update(SHA0, &context, data, length, true);
   }

   //Check status code
   if(status == kStatus_Success)
   {
      //Get the resulting digest value
      SHA_Finish(SHA0, &context, digest, &n);
   }

   //Release exclusive access to the SHA module
   osReleaseMutex(&lpc54xxxCryptoMutex);

   //Return status code
   return (status == kStatus_Success) ? NO_ERROR : ERROR_FAILURE;
}

#endif
#if (SHA256_SUPPORT == ENABLED)

/**
 * @brief Digest a message using SHA-256
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t sha256Compute(const void *data, size_t length, uint8_t *digest)
{
   size_t n;
   status_t status;
   sha_ctx_t context;

   //Size of the digest value
   n = SHA256_DIGEST_SIZE;

   //Acquire exclusive access to the SHA module
   osAcquireMutex(&lpc54xxxCryptoMutex);

   //Initialize hash computation
   status = SHA_Init(SHA0, &context, kSHA_Sha256);

   //Check status code
   if(status == kStatus_Success)
   {
      //Digest the message
      SHA_Update(SHA0, &context, data, length, true);
   }

   //Check status code
   if(status == kStatus_Success)
   {
      //Get the resulting digest value
      SHA_Finish(SHA0, &context, digest, &n);
   }

   //Release exclusive access to the SHA module
   osReleaseMutex(&lpc54xxxCryptoMutex);

   //Return status code
   return (status == kStatus_Success) ? NO_ERROR : ERROR_FAILURE;
}

#endif
#endif
