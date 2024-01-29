/**
 * @file stm32f4xx_crypto_cipher.h
 * @brief STM32F4 cipher hardware accelerator
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

#ifndef _STM32F4XX_CRYPTO_CIPHER_H
#define _STM32F4XX_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef STM32F4XX_CRYPTO_CIPHER_SUPPORT
   #define STM32F4XX_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (STM32F4XX_CRYPTO_CIPHER_SUPPORT != ENABLED && STM32F4XX_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error STM32F4XX_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//GCM_CCMPH  bitfield
#define CRYP_CR_GCM_CCMPH_INIT    0
#define CRYP_CR_GCM_CCMPH_HEADER  CRYP_CR_GCM_CCMPH_0
#define CRYP_CR_GCM_CCMPH_PAYLOAD CRYP_CR_GCM_CCMPH_1
#define CRYP_CR_GCM_CCMPH_FINAL   (CRYP_CR_GCM_CCMPH_1 | CRYP_CR_GCM_CCMPH_0)

//KEYSIZE bitfield
#define CRYP_CR_KEYSIZE_128B      0
#define CRYP_CR_KEYSIZE_192B      CRYP_CR_KEYSIZE_0
#define CRYP_CR_KEYSIZE_256B      CRYP_CR_KEYSIZE_1

//DATATYPE bitfield
#define CRYP_CR_DATATYPE_32B      0
#define CRYP_CR_DATATYPE_16B      CRYP_CR_DATATYPE_0
#define CRYP_CR_DATATYPE_8B       CRYP_CR_DATATYPE_1
#define CRYP_CR_DATATYPE_1B       (CRYP_CR_DATATYPE_1 | CRYP_CR_DATATYPE_0)

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
