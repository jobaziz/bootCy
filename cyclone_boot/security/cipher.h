/**
 * @file cipher.h
 * @brief CycloneBOOT Encryption & Decryption handling
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

#ifndef _CIPHER_H
#define _CIPHER_H

//Dependencies
#include "core/cboot_error.h"
#include "cipher/cipher_algorithms.h"

#ifndef CIPHER_SUPPORT
#define CIPHER_SUPPORT DISABLED
#elif ((CIPHER_SUPPORT != ENABLED) && (CIPHER_SUPPORT != DISABLED))
   #error CIPHER_SUPPORT parameter is not valid!
#endif

// Cipher initialization vector maximum size
#define MAX_CIPHER_IV_SIZE MAX_CIPHER_BLOCK_SIZE

/**
 * @brief Cipher engine structure definition
 **/

typedef struct
{
   CipherContext context;
   const CipherAlgo *algo;
   CipherMode mode;
   const char_t *key;
   uint8_t iv[MAX_CIPHER_IV_SIZE];
   size_t ivLen;
} CipherEngine;


//CycloneBOOT cipher engine related functions
cboot_error_t cipherInit(CipherEngine *engine, const CipherAlgo *algo,
   CipherMode mode, const char_t *key, size_t keyLen);
cboot_error_t cipherSetIv(CipherEngine *engine, uint8_t* iv, size_t ivLen);
cboot_error_t cipherEncryptData(CipherEngine *cipherEngine, uint8_t *data, size_t length);
cboot_error_t cipherDecryptData(CipherEngine *cipherEngine, uint8_t *data, size_t length);

#endif // !_CIPHER_H
