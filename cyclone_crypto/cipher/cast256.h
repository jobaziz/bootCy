/**
 * @file cast256.h
 * @brief CAST-256 encryption algorithm
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

#ifndef _CAST256_H
#define _CAST256_H

//Dependencies
#include "core/crypto.h"

//CAST-256 block size
#define CAST256_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define CAST256_CIPHER_ALGO (&cast256CipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief CAST-256 algorithm context
 **/

typedef struct
{
   uint32_t km[12][4];
   uint8_t kr[12][4];
} Cast256Context;


//CAST-256 related constants
extern const CipherAlgo cast256CipherAlgo;

//CAST-256 related functions
error_t cast256Init(Cast256Context *context, const uint8_t *key, size_t keyLen);

void cast256EncryptBlock(Cast256Context *context, const uint8_t *input,
   uint8_t *output);

void cast256DecryptBlock(Cast256Context *context, const uint8_t *input,
   uint8_t *output);

void cast256Deinit(Cast256Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
