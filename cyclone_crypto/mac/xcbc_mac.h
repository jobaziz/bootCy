/**
 * @file xcbc_mac.h
 * @brief XCBC-MAC message authentication code
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

#ifndef _XCBC_MAC_H
#define _XCBC_MAC_H

//Dependencies
#include "core/crypto.h"
#include "cipher/cipher_algorithms.h"

//Application specific context
#ifndef XCBC_MAC_PRIVATE_CONTEXT
   #define XCBC_MAC_PRIVATE_CONTEXT
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief XCBC-MAC algorithm context
 **/

typedef struct
{
   const CipherAlgo *cipher;
   CipherContext cipherContext;
   uint8_t k1[MAX_CIPHER_BLOCK_SIZE];
   uint8_t k2[MAX_CIPHER_BLOCK_SIZE];
   uint8_t k3[MAX_CIPHER_BLOCK_SIZE];
   uint8_t buffer[MAX_CIPHER_BLOCK_SIZE];
   size_t bufferLength;
   uint8_t mac[MAX_CIPHER_BLOCK_SIZE];
   XCBC_MAC_PRIVATE_CONTEXT
} XcbcMacContext;


//XCBC-MAC related functions
error_t xcbcMacCompute(const CipherAlgo *cipher, const void *key, size_t keyLen,
   const void *data, size_t dataLen, uint8_t *mac, size_t macLen);

error_t xcbcMacInit(XcbcMacContext *context, const CipherAlgo *cipher,
   const void *key, size_t keyLen);

void xcbcMacReset(XcbcMacContext *context);
void xcbcMacUpdate(XcbcMacContext *context, const void *data, size_t dataLen);
error_t xcbcMacFinal(XcbcMacContext *context, uint8_t *mac, size_t macLen);
void xcbcMacDeinit(XcbcMacContext *context);

void xcbcMacXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
