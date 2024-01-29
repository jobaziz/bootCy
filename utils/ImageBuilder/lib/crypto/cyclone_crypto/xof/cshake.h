/**
 * @file cshake.h
 * @brief cSHAKE128 and cSHAKE256 (customizable SHAKE function)
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

#ifndef _CSHAKE_H
#define _CSHAKE_H

//Dependencies
#include "core/crypto.h"
#include "xof/keccak.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief cSHAKE algorithm context
 **/

typedef struct
{
   size_t nameLen;
   size_t customLen;
   KeccakContext keccakContext;
} CshakeContext;


//cSHAKE related functions
error_t cshakeCompute(uint_t strength, const void *input, size_t inputLen,
   const char_t *name, size_t nameLen, const char_t *custom, size_t customLen,
   uint8_t *output, size_t outputLen);

error_t cshakeInit(CshakeContext *context, uint_t strength, const char_t *name,
   size_t nameLen, const char_t *custom, size_t customLen);

void cshakeAbsorb(CshakeContext *context, const void *input, size_t length);
void cshakeFinal(CshakeContext *context);
void cshakeSqueeze(CshakeContext *context, uint8_t *output, size_t length);

void cshakeLeftEncode(size_t value, uint8_t *buffer, size_t *length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
