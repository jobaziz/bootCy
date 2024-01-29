/**
 * @file sha512_224.c
 * @brief SHA-512/224 (Secure Hash Algorithm)
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
#include "core/crypto.h"
#include "hash/sha512_224.h"

//Check crypto library configuration
#if (SHA512_224_SUPPORT == ENABLED)

//SHA-512/224 object identifier (2.16.840.1.101.3.4.2.5)
const uint8_t sha512_224Oid[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x05};

//Common interface for hash algorithms
const HashAlgo sha512_224HashAlgo =
{
   "SHA-512/224",
   sha512_224Oid,
   sizeof(sha512_224Oid),
   sizeof(Sha512_224Context),
   SHA512_224_BLOCK_SIZE,
   SHA512_224_DIGEST_SIZE,
   SHA512_224_MIN_PAD_SIZE,
   TRUE,
   (HashAlgoCompute) sha512_224Compute,
   (HashAlgoInit) sha512_224Init,
   (HashAlgoUpdate) sha512_224Update,
   (HashAlgoFinal) sha512_224Final,
   NULL
};


/**
 * @brief Digest a message using SHA-512/224
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

__weak_func error_t sha512_224Compute(const void *data, size_t length, uint8_t *digest)
{
   error_t error;
   Sha512_224Context *context;

   //Allocate a memory buffer to hold the SHA-512/224 context
   context = cryptoAllocMem(sizeof(Sha512_224Context));

   //Successful memory allocation?
   if(context != NULL)
   {
      //Initialize the SHA-512/224 context
      sha512_224Init(context);
      //Digest the message
      sha512_224Update(context, data, length);
      //Finalize the SHA-512/224 message digest
      sha512_224Final(context, digest);

      //Free previously allocated memory
      cryptoFreeMem(context);

      //Successful processing
      error = NO_ERROR;
   }
   else
   {
      //Failed to allocate memory
      error = ERROR_OUT_OF_MEMORY;
   }

   //Return status code
   return error;
}


/**
 * @brief Initialize SHA-512/224 message digest context
 * @param[in] context Pointer to the SHA-512/224 context to initialize
 **/

__weak_func void sha512_224Init(Sha512_224Context *context)
{
   //Set initial hash value
   context->h[0] = 0x8C3D37C819544DA2;
   context->h[1] = 0x73E1996689DCD4D6;
   context->h[2] = 0x1DFAB7AE32FF9C82;
   context->h[3] = 0x679DD514582F9FCF;
   context->h[4] = 0x0F6D2B697BD44DA8;
   context->h[5] = 0x77E36F7304C48942;
   context->h[6] = 0x3F9D85A86A1D36C8;
   context->h[7] = 0x1112E6AD91D692A1;

   //Number of bytes in the buffer
   context->size = 0;
   //Total length of the message
   context->totalSize = 0;
}


/**
 * @brief Update the SHA-512/224 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA-512/224 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

__weak_func void sha512_224Update(Sha512_224Context *context, const void *data, size_t length)
{
   //The function is defined in the exact same manner as SHA-512
   sha512Update(context, data, length);
}


/**
 * @brief Finish the SHA-512/224 message digest
 * @param[in] context Pointer to the SHA-512/224 context
 * @param[out] digest Calculated digest (optional parameter)
 **/

__weak_func void sha512_224Final(Sha512_224Context *context, uint8_t *digest)
{
   //The function is defined in the exact same manner as SHA-512
   sha512Final(context, NULL);

   //Copy the resulting digest
   if(digest != NULL)
   {
      osMemcpy(digest, context->digest, SHA512_224_DIGEST_SIZE);
   }
}

#endif
