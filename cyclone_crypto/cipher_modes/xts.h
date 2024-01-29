/**
 * @file xts.h
 * @brief XEX-based tweaked-codebook mode with ciphertext stealing (XTS)
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

#ifndef _XTS_H
#define _XTS_H

//Dependencies
#include "core/crypto.h"
#include "cipher/cipher_algorithms.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief XTS context
 **/

typedef struct
{
   const CipherAlgo *cipherAlgo;
   CipherContext cipherContext1;
   CipherContext cipherContext2;
} XtsContext;


//XTS related functions
error_t xtsInit(XtsContext *context, const CipherAlgo *cipherAlgo,
   const void *key, size_t keyLen);

error_t xtsEncrypt(XtsContext *context, const uint8_t *i, const uint8_t *p,
   uint8_t *c, size_t length);

error_t xtsDecrypt(XtsContext *context, const uint8_t *i, const uint8_t *c,
   uint8_t *p, size_t length);

void xtsMul(uint8_t *x, const uint8_t *a);
void xtsXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
