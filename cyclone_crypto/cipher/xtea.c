/**
 * @file xtea.c
 * @brief XTEA (eXtended TEA)
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
#include "cipher/xtea.h"

//XTEA magic constant
#define DELTA 0x9E3779B9

//Check crypto library configuration
#if (XTEA_SUPPORT == ENABLED)


//Common interface for encryption algorithms
const CipherAlgo xteaCipherAlgo =
{
   "XTEA",
   sizeof(XteaContext),
   CIPHER_ALGO_TYPE_BLOCK,
   XTEA_BLOCK_SIZE,
   (CipherAlgoInit) xteaInit,
   NULL,
   NULL,
   (CipherAlgoEncryptBlock) xteaEncryptBlock,
   (CipherAlgoDecryptBlock) xteaDecryptBlock,
   (CipherAlgoDeinit) xteaDeinit
};


/**
 * @brief Key expansion
 * @param[in] context Pointer to the XTEA context to initialize
 * @param[in] key Pointer to the key
 * @param[in] keyLen Length of the key
 * @return Error code
 **/

__weak_func error_t xteaInit(XteaContext *context, const uint8_t *key,
   size_t keyLen)
{
   //Check parameters
   if(context == NULL || key == NULL)
      return ERROR_INVALID_PARAMETER;

   //Invalid key length?
   if(keyLen != 16)
      return ERROR_INVALID_KEY_LENGTH;

   //Copy the 128-bit key
   context->k[0] = LOAD32BE(key + 0);
   context->k[1] = LOAD32BE(key + 4);
   context->k[2] = LOAD32BE(key + 8);
   context->k[3] = LOAD32BE(key + 12);

   //No error to report
   return NO_ERROR;
}


/**
 * @brief Encrypt a 16-byte block using XTEA algorithm
 * @param[in] context Pointer to the XTEA context
 * @param[in] input Plaintext block to encrypt
 * @param[out] output Ciphertext block resulting from encryption
 **/

__weak_func void xteaEncryptBlock(XteaContext *context, const uint8_t *input,
   uint8_t *output)
{
   uint_t i;
   uint32_t y;
   uint32_t z;
   uint32_t sum;

   //Initialize variable
   sum = 0;

   //The 8 bytes of plaintext are split into 2 words
   y = LOAD32BE(input + 0);
   z = LOAD32BE(input + 4);

   //Apply 32 rounds
   for(i = 0; i < XTEA_NB_ROUNDS; i++)
   {
      y += (((z << 4) ^ (z >> 5)) + z) ^ (sum + context->k[sum & 0x03]);
      sum += DELTA;
      z += (((y << 4) ^ (y >> 5)) + y) ^ (sum + context->k[(sum >> 11) & 0x03]);
   }

   //The 2 words of ciphertext are then written as 8 bytes
   STORE32BE(y, output + 0);
   STORE32BE(z, output + 4);
}


/**
 * @brief Decrypt a 16-byte block using XTEA algorithm
 * @param[in] context Pointer to the XTEA context
 * @param[in] input Ciphertext block to decrypt
 * @param[out] output Plaintext block resulting from decryption
 **/

__weak_func void xteaDecryptBlock(XteaContext *context, const uint8_t *input,
   uint8_t *output)
{
   uint_t i;
   uint32_t y;
   uint32_t z;
   uint32_t sum;

   //Initialize variable
   sum = (DELTA & 0x07FFFFFF) << 5;

   //The 8 bytes of ciphertext are split into 2 words
   y = LOAD32BE(input + 0);
   z = LOAD32BE(input + 4);

   //Apply 32 rounds
   for(i = 0; i < XTEA_NB_ROUNDS; i++)
   {
      z -= (((y << 4) ^ (y >> 5)) + y) ^ (sum + context->k[(sum >> 11) & 0x03]);
      sum -= DELTA;
      y -= (((z << 4) ^ (z >> 5)) + z) ^ (sum + context->k[sum & 0x03]);
   }

   //The 2 words of plaintext are then written as 8 bytes
   STORE32BE(y, output + 0);
   STORE32BE(z, output + 4);
}


/**
 * @brief Release XTEA context
 * @param[in] context Pointer to the XTEA context
 **/

__weak_func void xteaDeinit(XteaContext *context)
{
   //Clear XTEA context
   osMemset(context, 0, sizeof(XteaContext));
}

#endif
