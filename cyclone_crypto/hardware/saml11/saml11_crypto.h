/**
 * @file saml11_crypto.h
 * @brief SAML11 hardware cryptographic accelerator
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

#ifndef _SAML11_CRYPTO_H
#define _SAML11_CRYPTO_H

//Dependencies
#include "core/crypto.h"

//CRYA function entry points
#define CRYA_SHA_ADDR        0x02001900
#define CRYA_AES_ENC_ADDR    0x02001904
#define CRYA_AES_DEC_ADDR    0x02001908
#define CRYA_GF_MULT128_ADDR 0x0200190C

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CRYA function prototypes
typedef void (*crya_sha_process_t)(uint32_t digest[8],
   const uint8_t data[64], uint32_t buffer[64]);

typedef void (*crya_aes_encrypt_t)(const uint8_t *key, uint32_t keyLen,
   const uint8_t *src, uint8_t *dest);

typedef void (*crya_aes_decrypt_t)(const uint8_t *key, uint32_t keyLen,
   const uint8_t *src, uint8_t *dest);

typedef void (*crya_gf_mult128_t)(const uint32_t *block1,
   const uint32_t *block2, uint32_t *dest);

//CRYA related functions
#define crya_sha_process ((crya_sha_process_t) (CRYA_SHA_ADDR | 1))
#define crya_aes_encrypt ((crya_aes_encrypt_t) (CRYA_AES_ENC_ADDR | 1))
#define crya_aes_decrypt ((crya_aes_decrypt_t) (CRYA_AES_DEC_ADDR | 1))
#define crya_gf_mult128 ((crya_gf_mult128_t) (CRYA_GF_MULT128_ADDR | 1))

//Global variables
extern OsMutex saml11CryptoMutex;

//SAML11 hardware cryptographic accelerator related functions
error_t saml11CryptoInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
