/**
 * @file gd32f2xx_crypto_hash.c
 * @brief GD32F2 hash hardware accelerator
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
#include "gd32f20x.h"
#include "gd32f20x_hau.h"
#include "core/crypto.h"
#include "hardware/gd32f2xx/gd32f2xx_crypto.h"
#include "hardware/gd32f2xx/gd32f2xx_crypto_hash.h"
#include "hash/hash_algorithms.h"
#include "debug.h"

//Check crypto library configuration
#if (GD32F2XX_CRYPTO_HASH_SUPPORT == ENABLED)


/**
 * @brief HAU module initialization
 * @return Error code
 **/

error_t hauInit(void)
{
   //Enable HAU peripheral clock
   rcu_periph_clock_enable(RCU_HAU);

   //Successful processing
   return NO_ERROR;
}


#if (MD5_SUPPORT == ENABLED)

/**
 * @brief Digest a message using MD5
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t md5Compute(const void *data, size_t length, uint8_t *digest)
{
   ErrStatus status;

   //Acquire exclusive access to the HAU module
   osAcquireMutex(&gd32f2xxCryptoMutex);
   //Digest the message using MD5
   status = hau_hash_md5((uint8_t *) data, length, digest);
   //Release exclusive access to the HAU module
   osReleaseMutex(&gd32f2xxCryptoMutex);

   //Return status code
   return (status == SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}

#endif
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
   ErrStatus status;

   //Acquire exclusive access to the HAU module
   osAcquireMutex(&gd32f2xxCryptoMutex);
   //Digest the message using SHA-1
   status = hau_hash_sha_1((uint8_t *) data, length, digest);
   //Release exclusive access to the HAU module
   osReleaseMutex(&gd32f2xxCryptoMutex);

   //Return status code
   return (status == SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}

#endif
#if (SHA224_SUPPORT == ENABLED)

/**
 * @brief Digest a message using SHA-224
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t sha224Compute(const void *data, size_t length, uint8_t *digest)
{
   ErrStatus status;

   //Acquire exclusive access to the HAU module
   osAcquireMutex(&gd32f2xxCryptoMutex);
   //Digest the message using SHA-224
   status = hau_hash_sha_224((uint8_t *) data, length, digest);
   //Release exclusive access to the HAU module
   osReleaseMutex(&gd32f2xxCryptoMutex);

   //Return status code
   return (status == SUCCESS) ? NO_ERROR : ERROR_FAILURE;
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
   ErrStatus status;

   //Acquire exclusive access to the HAU module
   osAcquireMutex(&gd32f2xxCryptoMutex);
   //Digest the message using SHA-256
   status = hau_hash_sha_256((uint8_t *) data, length, digest);
   //Release exclusive access to the HAU module
   osReleaseMutex(&gd32f2xxCryptoMutex);

   //Return status code
   return (status == SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}

#endif
#endif
