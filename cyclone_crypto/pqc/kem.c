/**
 * @file kem.c
 * @brief Key encapsulation mechanism (KEM)
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
#include "pqc/kem.h"

//Check crypto library configuration
#if (KEM_SUPPORT == ENABLED)


/**
 * @brief Initialize KEM context
 * @param[in] context Pointer to the KEM context
 * @param[in] kemAlgo Key encapsulation mechanism
 **/

void kemInit(KemContext *context, const KemAlgo *kemAlgo)
{
   //Initialize KEM context
   context->kemAlgo = kemAlgo;
   context->sk = NULL;
   context->pk = NULL;
}


/**
 * @brief Release KEM context
 * @param[in] context Pointer to the KEM context
 **/

void kemFree(KemContext *context)
{
   //Valid key encapsulation mechanism?
   if(context->kemAlgo != NULL)
   {
      //Check whether the secret key is valid
      if(context->sk != NULL)
      {
         //Clear secret key
         osMemset(context->sk, 0, context->kemAlgo->secretKeySize);

         //Release secret key
         cryptoFreeMem(context->sk);
         context->sk = NULL;
      }

      //Check whether the public key is valid
      if(context->pk != NULL)
      {
         //Clear public key
         osMemset(context->pk, 0, context->kemAlgo->publicKeySize);

         //Release public key
         cryptoFreeMem(context->pk);
         context->pk = NULL;
      }
   }
}


/**
 * @brief Key pair generation
 * @param[in] context Pointer to the KEM context
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @return Error code
 **/

error_t kemGenerateKeyPair(KemContext *context, const PrngAlgo *prngAlgo,
   void *prngContext)
{
   error_t error;

   //Valid key encapsulation mechanism?
   if(context->kemAlgo != NULL)
   {
      //Allocate a memory buffer to hold the secret key
      if(context->sk == NULL)
      {
         context->sk = cryptoAllocMem(context->kemAlgo->secretKeySize);
      }

      //Successful memory allocation?
      if(context->sk != NULL)
      {
         //Allocate a memory buffer to hold the public key
         if(context->pk == NULL)
         {
            context->pk = cryptoAllocMem(context->kemAlgo->publicKeySize);
         }

         //Successful memory allocation?
         if(context->pk != NULL)
         {
            //Key pair generation
            error = context->kemAlgo->generateKeyPair(prngAlgo, prngContext,
               context->pk, context->sk);
         }
         else
         {
            //Failed to allocate memory
            error = ERROR_OUT_OF_MEMORY;
         }
      }
      else
      {
         //Failed to allocate memory
         error = ERROR_OUT_OF_MEMORY;
      }
   }
   else
   {
      //Invalid key encapsulation mechanism
      error = ERROR_INVALID_PARAMETER;
   }

   //Return status code
   return error;
}


/**
 * @brief Load public key
 * @param[in] context Pointer to the KEM context
 * @param[in] pk Public key
 * @return Error code
 **/

error_t kemLoadPublicKey(KemContext *context, const uint8_t *pk)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Valid key encapsulation mechanism?
   if(context->kemAlgo != NULL)
   {
      //Allocate a memory buffer to hold the public key
      if(context->pk == NULL)
      {
         context->pk = cryptoAllocMem(context->kemAlgo->publicKeySize);
      }

      //Successful memory allocation?
      if(context->pk != NULL)
      {
         //Copy the public key
         osMemcpy(context->pk, pk, context->kemAlgo->publicKeySize);
      }
      else
      {
         //Failed to allocate memory
         error = ERROR_OUT_OF_MEMORY;
      }
   }
   else
   {
      //Invalid key encapsulation mechanism
      error = ERROR_INVALID_PARAMETER;
   }

   //Return status code
   return error;
}


/**
 * @brief Encapsulation algorithm
 * @param[in] context Pointer to the KEM context
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[out] ct Ciphertext
 * @param[out] ss Shared secret
 * @return Error code
 **/

error_t kemEncapsulate(KemContext *context, const PrngAlgo *prngAlgo,
   void *prngContext, uint8_t *ct, uint8_t *ss)
{
   error_t error;

   //Valid parameters?
   if(context->kemAlgo != NULL && context->pk != NULL)
   {
      //Encapsulation algorithm
      error = context->kemAlgo->encapsulate(prngAlgo, prngContext, ct, ss,
         context->pk);
   }
   else
   {
      //Invalid parameters
      error = ERROR_INVALID_PARAMETER;
   }

   //Return status code
   return error;
}


/**
 * @brief Decapsulation algorithm
 * @param[in] context Pointer to the KEM context
 * @param[in] ct Ciphertext
 * @param[out] ss Shared secret
 * @return Error code
 **/

error_t kemDecapsulate(KemContext *context, const uint8_t *ct, uint8_t *ss)
{
   error_t error;

   //Valid parameters?
   if(context->kemAlgo != NULL && context->sk != NULL)
   {
      //Decapsulation algorithm
      error = context->kemAlgo->decapsulate(ss, ct, context->sk);
   }
   else
   {
      //Invalid key encapsulation mechanism
      error = ERROR_INVALID_PARAMETER;
   }

   //Return status code
   return error;
}

#endif
