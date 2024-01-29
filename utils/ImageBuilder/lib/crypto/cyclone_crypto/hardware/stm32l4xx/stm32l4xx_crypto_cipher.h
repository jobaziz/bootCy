/**
 * @file stm32l4xx_crypto_cipher.h
 * @brief STM32L4 cipher hardware accelerator
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

#ifndef _STM32L4XX_CRYPTO_CIPHER_H
#define _STM32L4XX_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef STM32L4XX_CRYPTO_CIPHER_SUPPORT
   #define STM32L4XX_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (STM32L4XX_CRYPTO_CIPHER_SUPPORT != ENABLED && STM32L4XX_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error STM32L4XX_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//KEYSIZE bitfield
#define AES_CR_KEYSIZE_128B        0
#define AES_CR_KEYSIZE_256B        AES_CR_KEYSIZE

//GCMPH bitfield
#define AES_CR_GCMPH_INIT          0
#define AES_CR_GCMPH_HEADER        AES_CR_GCMPH_0
#define AES_CR_GCMPH_PAYLOAD       AES_CR_GCMPH_1
#define AES_CR_GCMPH_FINAL         (AES_CR_GCMPH_1 | AES_CR_GCMPH_0)

//CHMOD bitfield
#define AES_CR_CHMOD_ECB           0
#define AES_CR_CHMOD_CBC           AES_CR_CHMOD_0
#define AES_CR_CHMOD_CTR           AES_CR_CHMOD_1
#define AES_CR_CHMOD_GCM_GMAC      (AES_CR_CHMOD_0 | AES_CR_CHMOD_1)
#define AES_CR_CHMOD_CCM           AES_CR_CHMOD_2

//MODE bitfield
#define AES_CR_MODE_ENCRYPTION     0
#define AES_CR_MODE_KEY_DERIVATION AES_CR_MODE_0
#define AES_CR_MODE_DECRYPTION     AES_CR_MODE_1

//DATATYPE bitfield
#define AES_CR_DATATYPE_32B        0
#define AES_CR_DATATYPE_16B        AES_CR_DATATYPE_0
#define AES_CR_DATATYPE_8B         AES_CR_DATATYPE_1
#define AES_CR_DATATYPE_1B         (AES_CR_DATATYPE_1 | AES_CR_DATATYPE_0)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Cipher related functions
error_t crypInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
