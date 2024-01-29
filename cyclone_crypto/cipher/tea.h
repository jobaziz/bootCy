/**
 * @file tea.h
 * @brief TEA (Tiny Encryption Algorithm)
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

#ifndef _TEA_H
#define _TEA_H

//Dependencies
#include "core/crypto.h"

//Application specific context
#ifndef TEA_PRIVATE_CONTEXT
   #define TEA_PRIVATE_CONTEXT
#endif

//TEA block size
#define TEA_BLOCK_SIZE 8
//TEA number of rounds
#define TEA_NB_ROUNDS 32
//Common interface for encryption algorithms
#define TEA_CIPHER_ALGO (&teaCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief TEA algorithm context
 **/

typedef struct
{
   uint32_t k[4];
   TEA_PRIVATE_CONTEXT
} TeaContext;


//TEA related constants
extern const CipherAlgo teaCipherAlgo;

//TEA related functions
error_t teaInit(TeaContext *context, const uint8_t *key, size_t keyLen);

void teaEncryptBlock(TeaContext *context, const uint8_t *input,
   uint8_t *output);

void teaDecryptBlock(TeaContext *context, const uint8_t *input,
   uint8_t *output);

void teaDeinit(TeaContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
