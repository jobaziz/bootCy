/**
 * @file blowfish.h
 * @brief Blowfish encryption algorithm
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

#ifndef _BLOWFISH_H
#define _BLOWFISH_H

//Dependencies
#include "core/crypto.h"

//Blowfish block size
#define BLOWFISH_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define BLOWFISH_CIPHER_ALGO (&blowfishCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Blowfish algorithm context
 **/

typedef struct
{
   uint32_t p[18];
   uint32_t s1[256];
   uint32_t s2[256];
   uint32_t s3[256];
   uint32_t s4[256];
} BlowfishContext;


//Blowfish related constants
extern const CipherAlgo blowfishCipherAlgo;

//Blowfish related functions
error_t blowfishInit(BlowfishContext *context, const uint8_t *key,
   size_t keyLen);

error_t blowfishInitState(BlowfishContext *context);

error_t blowfishExpandKey(BlowfishContext *context, const uint8_t *salt,
   size_t saltLen, const uint8_t *key, size_t keyLen);

void blowfishEncryptBlock(BlowfishContext *context, const uint8_t *input,
   uint8_t *output);

void blowfishDecryptBlock(BlowfishContext *context, const uint8_t *input,
   uint8_t *output);

void blowfishXorBlock(uint8_t *data, const uint8_t *salt, size_t saltLen,
   size_t *saltIndex);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
