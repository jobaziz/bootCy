/**
 * @file shake.c
 * @brief SHAKE128 and SHAKE256 extendable-output functions
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
#include "xof/shake.h"

//Check crypto library configuration
#if (SHAKE_SUPPORT == ENABLED)

//SHAKE128 object identifier (2.16.840.1.101.3.4.2.11)
const uint8_t shake128Oid[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x0B};
//SHAKE256 object identifier (2.16.840.1.101.3.4.2.12)
const uint8_t shake256Oid[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x0C};


/**
 * @brief Digest a message using SHAKE128 or SHAKE256
 * @param[in] strength Number of bits of security (128 for SHAKE128 and
 *   256 for SHAKE256)
 * @param[in] input Pointer to the input data
 * @param[in] inputLen Length of the input data
 * @param[out] output Pointer to the output data
 * @param[in] outputLen Expected length of the output data
 * @return Error code
 **/

error_t shakeCompute(uint_t strength, const void *input, size_t inputLen,
   uint8_t *output, size_t outputLen)
{
   error_t error;
   ShakeContext *context;

   //Allocate a memory buffer to hold the SHAKE context
   context = cryptoAllocMem(sizeof(ShakeContext));

   //Successful memory allocation?
   if(context != NULL)
   {
      //Initialize the SHAKE context
      error = shakeInit(context, strength);

      //Check status code
      if(!error)
      {
         //Absorb input data
         shakeAbsorb(context, input, inputLen);
         //Finish absorbing phase
         shakeFinal(context);
         //Extract data from the squeezing phase
         shakeSqueeze(context, output, outputLen);
      }

      //Free previously allocated memory
      cryptoFreeMem(context);
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
 * @brief Initialize SHAKE context
 * @param[in] context Pointer to the SHAKE context to initialize
 * @param[in] strength Number of bits of security (128 for SHAKE128 and
 *   256 for SHAKE256)
 * @return Error code
 **/

error_t shakeInit(ShakeContext *context, uint_t strength)
{
   error_t error;

   //SHAKE128 and SHAKE256 provides respectively 128 and 256 bits of security
   if(strength == 128 || strength == 256)
   {
      //Initialize Keccak context
      error = keccakInit(&context->keccakContext, 2 * strength);
   }
   else
   {
      //Report an error
      error = ERROR_INVALID_PARAMETER;
   }

   //Return status code
   return error;
}


/**
 * @brief Absorb data
 * @param[in] context Pointer to the SHAKE context
 * @param[in] input Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void shakeAbsorb(ShakeContext *context, const void *input, size_t length)
{
   //Absorb the input data
   keccakAbsorb(&context->keccakContext, input, length);
}


/**
 * @brief Finish absorbing phase
 * @param[in] context Pointer to the SHAKE context
 **/

void shakeFinal(ShakeContext *context)
{
   //Finish absorbing phase (padding byte is 0x1F for XOFs)
   keccakFinal(&context->keccakContext, KECCAK_SHAKE_PAD);
}


/**
 * @brief Extract data from the squeezing phase
 * @param[in] context Pointer to the SHAKE context
 * @param[out] output Output string
 * @param[in] length Desired output length, in bytes
 **/

void shakeSqueeze(ShakeContext *context, uint8_t *output, size_t length)
{
   //Extract data from the squeezing phase
   keccakSqueeze(&context->keccakContext, output, length);
}

#endif
