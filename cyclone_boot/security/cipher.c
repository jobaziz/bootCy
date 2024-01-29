/**
 * @file cipher.c
 * @brief CycloneBOOT Cipher image decoding handling
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include "update/update.h"
#include "cipher.h"
#include "cipher_modes/cbc.h"
#include "debug.h"

#if (CIPHER_SUPPORT == ENABLED)

/**
 * @brief Initialize cipher engine context.
 * @param[in] engine Pointer to the cipher Engine context to initialize
 * @param[in] algo Cipher algorithm to be used for encryption/decryption.
 * @param[in] mode Cipher mode to be used with cipher algorithm
 * @param[in] key Pointer to the cipher key
 * @param[in] keyLen Length of the cipher key
 * @return Error code
 **/

cboot_error_t cipherInit(CipherEngine *engine, const CipherAlgo *algo,
   CipherMode mode, const char_t *key, size_t keyLen)
{
   error_t error;

   //Check parameter validity
   if(engine == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check cipher engine fields
   if(algo == NULL || mode == CIPHER_MODE_NULL || key == NULL || keyLen == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Reset cipher engine contents
   memset(engine, 0, sizeof(CipherEngine));

   //Set cipher engine algorithm
   engine->algo = algo;
   //Set cipher engine mode
   engine->mode = mode;
   //Set cipher engine key
   engine->key = key;

   //Initialize cipher engine context
   error = engine->algo->init((void *) &engine->context,
      (const uint8_t *) engine->key, strlen(engine->key));
   //Is any error?
   if(error)
      return CBOOT_ERROR_FAILURE;

   //Set cipher iv length
   engine->ivLen = engine->algo->blockSize;

   //Return status code
   return CBOOT_NO_ERROR;
}


/**
 * @brief Set cipher initialization vector.
 * @param[in] engine Pointer to the cipher Engine context
 * @param[in] iv Initialization vector to use for encryption
 * @param[in] ivLen Length of the cipher initialization vector
 * @return Error code
 **/

cboot_error_t cipherSetIv(CipherEngine *engine, uint8_t* iv, size_t ivLen)
{
   //Check parameters
   if(engine == NULL || iv == NULL || ivLen == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Save cipher engine iv
   memcpy(engine->iv, iv, ivLen);

   //Successfull process
   return CBOOT_NO_ERROR;
}


/**
 * @brief Encrypt given data using cipher engine.
 * @param[in] engine Pointer to the cipher Engine context
 * @param[in,out] data Buffer that initially contains plaintext data and then will
 * hold ciphertext data resulting from encryption
 * @param[in] length Length of the plaintext data buffer
 * @return Error code
 **/

cboot_error_t cipherEncryptData(CipherEngine *engine, uint8_t *data, size_t length)
{
   error_t error;

   //Check parameters validity
   if(engine == NULL || data == NULL || length == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check cipher engine iv
   if(engine->ivLen == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check cipher engine mode
   if(engine->mode == CIPHER_MODE_NULL)
   {
      return CBOOT_ERROR_INVALID_PARAMETERS;
   }
   else if(engine->mode == CIPHER_MODE_CBC)
   {
      //Encrypt plaintext data using CBC mode
      error = cbcEncrypt(engine->algo, (void *) &engine->context,
         engine->iv, data, data, length);
      //Is any error?
      if(error)
         return CBOOT_ERROR_FAILURE;
      else
         return CBOOT_NO_ERROR;
   }
   else
   {
      //Debug message
      TRACE_ERROR("Cipher mode not supported!\r\n");
      return CBOOT_ERROR_NOT_IMPLEMENTED;
   }
}


/**
 * @brief Decrypt given encrypted data using cipher engine.
 * @param[in] engine Pointer to the cipher Engine context
 * @param[in,out] data Buffer that initially contains encrypted data and then will
 * hold plaintext data resulting for decryption
 * @param[in] length Length of the plaintext data buffer
 * @return Error code
 **/

cboot_error_t cipherDecryptData(CipherEngine *engine, uint8_t *data, size_t length)
{
   error_t error;

   //Check parameters validity
   if(engine == NULL || data == NULL || length == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check cipher engine iv
   if(engine->ivLen == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check cipher engine mode
   if(engine->mode == CIPHER_MODE_NULL)
   {
      return CBOOT_ERROR_INVALID_PARAMETERS;
   }
   else if(engine->mode == CIPHER_MODE_CBC)
   {
      //Decrypt ciphertext data using CBC mode
      error = cbcDecrypt(engine->algo, (void *) &engine->context,
         engine->iv, data, data, length);
      //Is any error?
      if(error)
         return CBOOT_ERROR_FAILURE;
      else
         return CBOOT_NO_ERROR;
   }
   else
   {
      //Debug message
      TRACE_ERROR("Cipher mode not supported!\r\n");
      //Forward error;
      return CBOOT_ERROR_NOT_IMPLEMENTED;
   }
}

#endif
