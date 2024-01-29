/**
 * @file saml11_crypto_hash.c
 * @brief SAML11 hash hardware accelerator
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
#include "sam.h"
#include "core/crypto.h"
#include "hardware/saml11/saml11_crypto.h"
#include "hardware/saml11/saml11_crypto_hash.h"
#include "hash/hash_algorithms.h"
#include "debug.h"

//Check crypto library configuration
#if (SAML11_CRYPTO_HASH_SUPPORT == ENABLED && SHA256_SUPPORT == ENABLED)


/**
 * @brief Process message in 16-word blocks
 * @param[in] context Pointer to the SHA-256 context
 **/

void sha256ProcessBlock(Sha256Context *context)
{
   static uint32_t buffer[64];

   //Acquire exclusive access to the CRYA module
   osAcquireMutex(&saml11CryptoMutex);
   //Accelerate SHA-256 inner compression loop
   crya_sha_process(context->h, context->buffer, buffer);
   //Release exclusive access to the CRYA module
   osReleaseMutex(&saml11CryptoMutex);
}

#endif
