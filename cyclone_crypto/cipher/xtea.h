/**
 * @file xtea.h
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

#ifndef _XTEA_H
#define _XTEA_H

//Dependencies
#include "core/crypto.h"

//Application specific context
#ifndef XTEA_PRIVATE_CONTEXT
   #define XTEA_PRIVATE_CONTEXT
#endif

//XTEA block size
#define XTEA_BLOCK_SIZE 8
//XTEA number of rounds
#define XTEA_NB_ROUNDS 32
//Common interface for encryption algorithms
#define XTEA_CIPHER_ALGO (&xteaCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief XTEA algorithm context
 **/

typedef struct
{
   uint32_t k[4];
   XTEA_PRIVATE_CONTEXT
} XteaContext;


//XTEA related constants
extern const CipherAlgo xteaCipherAlgo;

//XTEA related functions
error_t xteaInit(XteaContext *context, const uint8_t *key, size_t keyLen);

void xteaEncryptBlock(XteaContext *context, const uint8_t *input,
   uint8_t *output);

void xteaDecryptBlock(XteaContext *context, const uint8_t *input,
   uint8_t *output);

void xteaDeinit(XteaContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
