/**
 * @file concat_kdf.c
 * @brief Concat KDF (Concatenation Key Derivation Function)
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
#include "kdf/pbkdf.h"
#include "mac/hmac.h"

//Check crypto library configuration
#if (CONCAT_KDF_SUPPORT == ENABLED)


/**
 * @brief Concat KDF key derivation function
 * @param[in] hash Underlying hash function
 * @param[in] z Shared secret Z
 * @param[in] zLen Length in octets of the shared secret Z
 * @param[in] otherInfo Context-specific information (optional parameter)
 * @param[in] otherInfoLen Length in octets of the context-specific information
 * @param[out] dk Derived keying material
 * @param[in] dkLen Length in octets of the keying material to be generated
 * @return Error code
 **/

error_t concatKdf(const HashAlgo *hash, const uint8_t *z, size_t zLen,
   const uint8_t *otherInfo, size_t otherInfoLen, uint8_t *dk, size_t dkLen)
{
   size_t n;
   uint32_t i;
   uint8_t counter[4];
#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   HashContext *hashContext;
#else
   HashContext hashContext[1];
#endif

   //Check parameters
   if(hash == NULL || z == NULL || dk == NULL)
      return ERROR_INVALID_PARAMETER;

   //The OtherInfo parameter is optional
   if(otherInfo == NULL && otherInfoLen != 0)
      return ERROR_INVALID_PARAMETER;

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Allocate a memory buffer to hold the hash context
   hashContext = cryptoAllocMem(hash->contextSize);
   //Failed to allocate memory?
   if(hashContext == NULL)
      return ERROR_OUT_OF_MEMORY;
#endif

   //Derive the keying material
   for(i = 1; dkLen > 0; i++)
   {
      //Encode the counter as a 32-bit big-endian string
      STORE32BE(i, counter);

      //Compute H(counter || Z || OtherInfo)
      hash->init(hashContext);
      hash->update(hashContext, counter, sizeof(uint32_t));
      hash->update(hashContext, z, zLen);

      //The OtherInfo parameter is optional
      if(otherInfoLen > 0)
      {
         hash->update(hashContext, otherInfo, otherInfoLen);
      }

      //Finalize hash calculation
      hash->final(hashContext, NULL);

      //Number of octets in the current block
      n = MIN(dkLen, hash->digestSize);
      //Save the resulting block
      osMemcpy(dk, hashContext->digest, n);

      //Point to the next block
      dk += n;
      dkLen -= n;
   }

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Free previously allocated memory
   cryptoFreeMem(hashContext);
#endif

   //Successful processing
   return NO_ERROR;
}

#endif
