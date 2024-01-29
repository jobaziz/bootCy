/**
 * @file cmac.h
 * @brief CMAC (Cipher-based Message Authentication Code)
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

#ifndef _CMAC_H
#define _CMAC_H

//Dependencies
#include "core/crypto.h"
#include "cipher/cipher_algorithms.h"

//Application specific context
#ifndef CMAC_PRIVATE_CONTEXT
   #define CMAC_PRIVATE_CONTEXT
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief CMAC algorithm context
 **/

typedef struct
{
   const CipherAlgo *cipher;
   CipherContext cipherContext;
   uint8_t k1[MAX_CIPHER_BLOCK_SIZE];
   uint8_t k2[MAX_CIPHER_BLOCK_SIZE];
   uint8_t buffer[MAX_CIPHER_BLOCK_SIZE];
   size_t bufferLength;
   uint8_t mac[MAX_CIPHER_BLOCK_SIZE];
   CMAC_PRIVATE_CONTEXT
} CmacContext;


//CMAC related functions
error_t cmacCompute(const CipherAlgo *cipher, const void *key, size_t keyLen,
   const void *data, size_t dataLen, uint8_t *mac, size_t macLen);

error_t cmacInit(CmacContext *context, const CipherAlgo *cipher,
   const void *key, size_t keyLen);

void cmacReset(CmacContext *context);
void cmacUpdate(CmacContext *context, const void *data, size_t dataLen);
error_t cmacFinal(CmacContext *context, uint8_t *mac, size_t macLen);
void cmacDeinit(CmacContext *context);

void cmacMul(uint8_t *x, const uint8_t *a, size_t n, uint8_t rb);
void cmacXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
