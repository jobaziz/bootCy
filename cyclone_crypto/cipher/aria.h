/**
 * @file aria.h
 * @brief ARIA encryption algorithm
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

#ifndef _ARIA_H
#define _ARIA_H

//Dependencies
#include "core/crypto.h"

//ARIA block size
#define ARIA_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define ARIA_CIPHER_ALGO (&ariaCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ARIA algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t k[16];
   uint32_t ek[68];
   uint32_t dk[68];
} AriaContext;


//ARIA related constants
extern const uint8_t ARIA128_ECB_OID[9];
extern const uint8_t ARIA128_CBC_OID[9];
extern const uint8_t ARIA128_CFB_OID[9];
extern const uint8_t ARIA128_OFB_OID[9];
extern const uint8_t ARIA128_CTR_OID[9];
extern const uint8_t ARIA192_ECB_OID[9];
extern const uint8_t ARIA192_CBC_OID[9];
extern const uint8_t ARIA192_CFB_OID[9];
extern const uint8_t ARIA192_OFB_OID[9];
extern const uint8_t ARIA192_CTR_OID[9];
extern const uint8_t ARIA256_ECB_OID[9];
extern const uint8_t ARIA256_CBC_OID[9];
extern const uint8_t ARIA256_CFB_OID[9];
extern const uint8_t ARIA256_OFB_OID[9];
extern const uint8_t ARIA256_CTR_OID[9];
extern const uint8_t ARIA128_GCM_OID[9];
extern const uint8_t ARIA192_GCM_OID[9];
extern const uint8_t ARIA256_GCM_OID[9];
extern const uint8_t ARIA128_CCM_OID[9];
extern const uint8_t ARIA192_CCM_OID[9];
extern const uint8_t ARIA256_CCM_OID[9];
extern const CipherAlgo ariaCipherAlgo;

//ARIA related functions
error_t ariaInit(AriaContext *context, const uint8_t *key, size_t keyLen);

void ariaEncryptBlock(AriaContext *context, const uint8_t *input,
   uint8_t *output);

void ariaDecryptBlock(AriaContext *context, const uint8_t *input,
   uint8_t *output);

void ariaDeinit(AriaContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
