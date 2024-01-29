/**
 * @file sm4.h
 * @brief SM4 encryption algorithm
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

#ifndef _SM4_H
#define _SM4_H

//Dependencies
#include "core/crypto.h"

//Application specific context
#ifndef SM4_PRIVATE_CONTEXT
   #define SM4_PRIVATE_CONTEXT
#endif

//SM4 block size
#define SM4_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define SM4_CIPHER_ALGO (&sm4CipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SM4 algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t rk[32];
   SM4_PRIVATE_CONTEXT
} Sm4Context;


//SM4 related constants
extern const uint8_t SM4_CBC_OID[8];
extern const uint8_t SM4_ECB_OID[8];
extern const uint8_t SM4_CBC_OID[8];
extern const uint8_t SM4_OFB_OID[8];
extern const uint8_t SM4_CFB_OID[8];
extern const uint8_t SM4_CTR_OID[8];
extern const uint8_t SM4_GCM_OID[8];
extern const uint8_t SM4_CCM_OID[8];
extern const uint8_t SM4_XTS_OID[8];
extern const CipherAlgo sm4CipherAlgo;

//SM4 related functions
error_t sm4Init(Sm4Context *context, const uint8_t *key, size_t keyLen);

void sm4EncryptBlock(Sm4Context *context, const uint8_t *input,
   uint8_t *output);

void sm4DecryptBlock(Sm4Context *context, const uint8_t *input,
   uint8_t *output);

void sm4Deinit(Sm4Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
