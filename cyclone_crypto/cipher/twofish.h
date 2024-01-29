/**
 * @file twofish.h
 * @brief Twofish encryption algorithm
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

#ifndef _TWOFISH_H
#define _TWOFISH_H

//Dependencies
#include "core/crypto.h"

//Twofish block size
#define TWOFISH_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define TWOFISH_CIPHER_ALGO (&twofishCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Twofish algorithm context
 **/

typedef struct
{
   uint32_t k[40];
   uint32_t s1[256];
   uint32_t s2[256];
   uint32_t s3[256];
   uint32_t s4[256];
} TwofishContext;


//Twofish related constants
extern const CipherAlgo twofishCipherAlgo;

//Twofish related functions
error_t twofishInit(TwofishContext *context, const uint8_t *key, size_t keyLen);

void twofishEncryptBlock(TwofishContext *context, const uint8_t *input,
   uint8_t *output);

void twofishDecryptBlock(TwofishContext *context, const uint8_t *input,
   uint8_t *output);

void twofishDeinit(TwofishContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
