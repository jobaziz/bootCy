/**
 * @file gmac.h
 * @brief GMAC (Galois Message Authentication Code)
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

#ifndef _GMAC_H
#define _GMAC_H

//Dependencies
#include "core/crypto.h"
#include "cipher/cipher_algorithms.h"

//Precalculated table width, in bits
#ifndef GMAC_TABLE_W
   #define GMAC_TABLE_W 4
#elif (GMAC_TABLE_W != 4 && GMAC_TABLE_W != 8)
   #error GMAC_TABLE_W parameter is not valid
#endif

//4-bit or 8-bit precalculated table?
#if (GMAC_TABLE_W == 4)
   #define GMAC_TABLE_N 16
   #define GMAC_REVERSE_BITS(n) reverseInt4(n)
#else
   #define GMAC_TABLE_N 256
   #define GMAC_REVERSE_BITS(n) reverseInt8(n)
#endif

//Application specific context
#ifndef GMAC_PRIVATE_CONTEXT
   #define GMAC_PRIVATE_CONTEXT
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief GMAC algorithm context
 **/

typedef struct
{
   const CipherAlgo *cipher;
   CipherContext cipherContext;
   uint32_t m[GMAC_TABLE_N][4];
   uint8_t s[16];
   uint8_t buffer[16];
   size_t bufferLength;
   uint64_t totalLength;
   uint8_t mac[16];
   GMAC_PRIVATE_CONTEXT
} GmacContext;


//GMAC related functions
error_t gmacCompute(const CipherAlgo *cipher, const void *key, size_t keyLen,
   const uint8_t *iv, size_t ivLen, const void *data, size_t dataLen,
   uint8_t *mac, size_t macLen);

error_t gmacInit(GmacContext *context, const CipherAlgo *cipher,
   const void *key, size_t keyLen);

error_t gmacReset(GmacContext *context, const uint8_t *iv, size_t ivLen);
void gmacUpdate(GmacContext *context, const void *data, size_t dataLen);
error_t gmacFinal(GmacContext *context, uint8_t *mac, size_t macLen);
void gmacDeinit(GmacContext *context);

void gmacMul(GmacContext *context, uint8_t *x);
void gmacXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n);
void gmacIncCounter(uint8_t *ctr);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
