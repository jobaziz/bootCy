/**
 * @file mk6x_crypto_hash.c
 * @brief Kinetis K6x hash hardware accelerator
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
#include "fsl_mmcau.h"
#include "core/crypto.h"
#include "hardware/mk6x/mk6x_crypto.h"
#include "hardware/mk6x/mk6x_crypto_hash.h"
#include "hash/hash_algorithms.h"
#include "debug.h"

//Check crypto library configuration
#if (MK6X_CRYPTO_HASH_SUPPORT == ENABLED)
#if (MD5_SUPPORT == ENABLED)

/**
 * @brief Update the MD5 context with a portion of the message being hashed
 * @param[in] context Pointer to the MD5 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void md5Update(Md5Context *context, const void *data, size_t length)
{
   size_t n;

   //Acquire exclusive access to the MMCAU module
   osAcquireMutex(&mk6xCryptoMutex);

   //Process the incoming data
   while(length > 0)
   {
      //Check whether some data is pending in the buffer
      if(context->size == 0 && length >= 64)
      {
         //Update hash value
         MMCAU_MD5_HashN(data, 1, context->h);

         //Update the MD5 context
         context->totalSize += 64;
         //Advance the data pointer
         data = (uint8_t *) data + 64;
         //Remaining bytes to process
         length -= 64;
      }
      else
      {
         //The buffer can hold at most 64 bytes
         n = MIN(length, 64 - context->size);

         //Copy the data to the buffer
         osMemcpy(context->buffer + context->size, data, n);

         //Update the MD5 context
         context->size += n;
         context->totalSize += n;
         //Advance the data pointer
         data = (uint8_t *) data + n;
         //Remaining bytes to process
         length -= n;

         //Check whether the buffer is full
         if(context->size == 64)
         {
            //Update hash value
            MMCAU_MD5_HashN(context->buffer, 1, context->h);
            //Empty the buffer
            context->size = 0;
         }
      }
   }

   //Release exclusive access to the MMCAU module
   osReleaseMutex(&mk6xCryptoMutex);
}


/**
 * @brief Process message in 16-word blocks
 * @param[in] context Pointer to the MD5 context
 **/

void md5ProcessBlock(Md5Context *context)
{
   //Acquire exclusive access to the MMCAU module
   osAcquireMutex(&mk6xCryptoMutex);
   //Accelerate MD5 inner compression loop
   MMCAU_MD5_HashN(context->buffer, 1, context->h);
   //Release exclusive access to the MMCAU module
   osReleaseMutex(&mk6xCryptoMutex);
}

#endif
#if (SHA1_SUPPORT == ENABLED)

/**
 * @brief Update the SHA-1 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA-1 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void sha1Update(Sha1Context *context, const void *data, size_t length)
{
   size_t n;

   //Acquire exclusive access to the MMCAU module
   osAcquireMutex(&mk6xCryptoMutex);

   //Process the incoming data
   while(length > 0)
   {
      //Check whether some data is pending in the buffer
      if(context->size == 0 && length >= 64)
      {
         //Update hash value
         MMCAU_SHA1_HashN(data, 1, context->h);

         //Update the SHA-1 context
         context->totalSize += 64;
         //Advance the data pointer
         data = (uint8_t *) data + 64;
         //Remaining bytes to process
         length -= 64;
      }
      else
      {
         //The buffer can hold at most 64 bytes
         n = MIN(length, 64 - context->size);

         //Copy the data to the buffer
         osMemcpy(context->buffer + context->size, data, n);

         //Update the SHA-1 context
         context->size += n;
         context->totalSize += n;
         //Advance the data pointer
         data = (uint8_t *) data + n;
         //Remaining bytes to process
         length -= n;

         //Check whether the buffer is full
         if(context->size == 64)
         {
            //Update hash value
            MMCAU_SHA1_HashN(context->buffer, 1, context->h);
            //Empty the buffer
            context->size = 0;
         }
      }
   }

   //Release exclusive access to the MMCAU module
   osReleaseMutex(&mk6xCryptoMutex);
}


/**
 * @brief Process message in 16-word blocks
 * @param[in] context Pointer to the SHA-1 context
 **/

void sha1ProcessBlock(Sha1Context *context)
{
   //Acquire exclusive access to the MMCAU module
   osAcquireMutex(&mk6xCryptoMutex);
   //Accelerate SHA-1 inner compression loop
   MMCAU_SHA1_HashN(context->buffer, 1, context->h);
   //Release exclusive access to the MMCAU module
   osReleaseMutex(&mk6xCryptoMutex);
}

#endif
#if (SHA256_SUPPORT == ENABLED)

/**
 * @brief Update the SHA-256 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA-256 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void sha256Update(Sha256Context *context, const void *data, size_t length)
{
   size_t n;

   //Acquire exclusive access to the MMCAU module
   osAcquireMutex(&mk6xCryptoMutex);

   //Process the incoming data
   while(length > 0)
   {
      //Check whether some data is pending in the buffer
      if(context->size == 0 && length >= 64)
      {
         //Update hash value
         MMCAU_SHA256_HashN(data, 1, context->h);

         //Update the SHA-256 context
         context->totalSize += 64;
         //Advance the data pointer
         data = (uint8_t *) data + 64;
         //Remaining bytes to process
         length -= 64;
      }
      else
      {
         //The buffer can hold at most 64 bytes
         n = MIN(length, 64 - context->size);

         //Copy the data to the buffer
         osMemcpy(context->buffer + context->size, data, n);

         //Update the SHA-256 context
         context->size += n;
         context->totalSize += n;
         //Advance the data pointer
         data = (uint8_t *) data + n;
         //Remaining bytes to process
         length -= n;

         //Check whether the buffer is full
         if(context->size == 64)
         {
            //Update hash value
            MMCAU_SHA256_HashN(context->buffer, 1, context->h);
            //Empty the buffer
            context->size = 0;
         }
      }
   }

   //Release exclusive access to the MMCAU module
   osReleaseMutex(&mk6xCryptoMutex);
}


/**
 * @brief Process message in 16-word blocks
 * @param[in] context Pointer to the SHA-256 context
 **/

void sha256ProcessBlock(Sha256Context *context)
{
   //Acquire exclusive access to the MMCAU module
   osAcquireMutex(&mk6xCryptoMutex);
   //Accelerate SHA-256 inner compression loop
   MMCAU_SHA256_HashN(context->buffer, 1, context->h);
   //Release exclusive access to the MMCAU module
   osReleaseMutex(&mk6xCryptoMutex);
}

#endif
#endif
