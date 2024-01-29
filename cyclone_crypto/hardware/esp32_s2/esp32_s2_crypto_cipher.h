/**
 * @file esp32_s2_crypto_cipher.h
 * @brief ESP32-S2 cipher hardware accelerator
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

#ifndef _ESP32_S2_CRYPTO_CIPHER_H
#define _ESP32_S2_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef ESP32_S2_CRYPTO_CIPHER_SUPPORT
   #define ESP32_S2_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (ESP32_S2_CRYPTO_CIPHER_SUPPORT != ENABLED && ESP32_S2_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error ESP32_S2_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//AES Operation Mode register
#define AES_MODE_ENC          0x00000000
#define AES_MODE_DEC          0x00000004
#define AES_MODE_128_BITS     0x00000000
#define AES_MODE_192_BITS     0x00000001
#define AES_MODE_256_BITS     0x00000002

//AES Block Mode register
#define AES_BLOCK_MODE_ECB    0x00000000
#define AES_BLOCK_MODE_CBC    0x00000001
#define AES_BLOCK_MODE_OFB    0x00000002
#define AES_BLOCK_MODE_CTR    0x00000003
#define AES_BLOCK_MODE_CFB8   0x00000004
#define AES_BLOCK_MODE_CFB128 0x00000005
#define AES_BLOCK_MODE_GCM    0x00000006

//Endianness Configuration register
#define AES_ENDIAN_DEFAULT    0x0000003F

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Cipher related functions
void esp32s2AesInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
