/**
 * @file cast128.h
 * @brief CAST-128 encryption algorithm
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

#ifndef _CAST128_H
#define _CAST128_H

//Dependencies
#include "core/crypto.h"

//CAST-128 block size
#define CAST128_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define CAST128_CIPHER_ALGO (&cast128CipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief CAST-128 algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t km[16];
   uint32_t kr[16];
} Cast128Context;


//CAST-128 related constants
extern const CipherAlgo cast128CipherAlgo;

//CAST-128 related functions
error_t cast128Init(Cast128Context *context, const uint8_t *key, size_t keyLen);

void cast128EncryptBlock(Cast128Context *context, const uint8_t *input,
   uint8_t *output);

void cast128DecryptBlock(Cast128Context *context, const uint8_t *input,
   uint8_t *output);

void cast128Deinit(Cast128Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
