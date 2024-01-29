/**
 * @file apm32f4xx_crypto_cipher.h
 * @brief APM32F4 cipher hardware accelerator
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

#ifndef _APM32F4XX_CRYPTO_CIPHER_H
#define _APM32F4XX_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef APM32F4XX_CRYPTO_CIPHER_SUPPORT
   #define APM32F4XX_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (APM32F4XX_CRYPTO_CIPHER_SUPPORT != ENABLED && APM32F4XX_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error APM32F4XX_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//CRYP Control register
#define CRYP_CTRL_CRYPEN            0x00008000
#define CRYP_CTRL_FFLUSH            0x00004000
#define CRYP_CTRL_KSIZESEL          0x00000300
#define CRYP_CTRL_KSIZESEL_128B     0x00000000
#define CRYP_CTRL_KSIZESEL_192B     0x00000100
#define CRYP_CTRL_KSIZESEL_256B     0x00000200
#define CRYP_CTRL_DTSEL             0x000000C0
#define CRYP_CTRL_DTSEL_32B         0x00000000
#define CRYP_CTRL_DTSEL_16B         0x00000040
#define CRYP_CTRL_DTSEL_8B          0x00000080
#define CRYP_CTRL_DTSEL_1B          0x000000C0
#define CRYP_CTRL_ALGOMSEL          0x00000038
#define CRYP_CTRL_ALGOMSEL_TDES_ECB 0x00000000
#define CRYP_CTRL_ALGOMSEL_TDES_CBC 0x00000008
#define CRYP_CTRL_ALGOMSEL_DES_ECB  0x00000010
#define CRYP_CTRL_ALGOMSEL_DES_CBC  0x00000018
#define CRYP_CTRL_ALGOMSEL_AES_ECB  0x00000020
#define CRYP_CTRL_ALGOMSEL_AES_CBC  0x00000028
#define CRYP_CTRL_ALGOMSEL_AES_CTR  0x00000030
#define CRYP_CTRL_ALGOMSEL_AES_KEY  0x00000038
#define CRYP_CTRL_ALGODIRSEL        0x00000004

//CRYP Status register
#define CRYP_STS_BUSY               0x00000010
#define CRYP_STS_OFFULL             0x00000008
#define CRYP_STS_OFEMPT             0x00000004
#define CRYP_STS_IFFULL             0x00000002
#define CRYP_STS_IFEMPT             0x00000001

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
