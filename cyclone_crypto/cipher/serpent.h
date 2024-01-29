/**
 * @file serpent.h
 * @brief Serpent encryption algorithm
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

#ifndef _SERPENT_H
#define _SERPENT_H

//Dependencies
#include "core/crypto.h"

//Serpent block size
#define SERPENT_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define SERPENT_CIPHER_ALGO (&serpentCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Serpent algorithm context
 **/

typedef struct
{
   uint32_t k[33][4];
} SerpentContext;


//Serpent related constants
extern const CipherAlgo serpentCipherAlgo;

//Serpent related functions
error_t serpentInit(SerpentContext *context, const uint8_t *key, size_t keyLen);

void serpentEncryptBlock(SerpentContext *context, const uint8_t *input,
   uint8_t *output);

void serpentDecryptBlock(SerpentContext *context, const uint8_t *input,
   uint8_t *output);

void serpentDeinit(SerpentContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
