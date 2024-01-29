/**
 * @file apm32f4xx_crypto_hash.h
 * @brief APM32F4 hash hardware accelerator
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

#ifndef _APM32F4XX_CRYPTO_HASH_H
#define _APM32F4XX_CRYPTO_HASH_H

//Dependencies
#include "core/crypto.h"

//Hash hardware accelerator
#ifndef APM32F4XX_CRYPTO_HASH_SUPPORT
   #define APM32F4XX_CRYPTO_HASH_SUPPORT DISABLED
#elif (APM32F4XX_CRYPTO_HASH_SUPPORT != ENABLED && APM32F4XX_CRYPTO_HASH_SUPPORT != DISABLED)
   #error APM32F4XX_CRYPTO_HASH_SUPPORT parameter is not valid
#endif

//HASH Control register
#define HASH_CTRL_LKEYSEL     0x00010000
#define HASH_CTRL_DINNEMPT    0x00001000
#define HASH_CTRL_WNUM        0x00000F00
#define HASH_CTRL_ALGSEL      0x00000080
#define HASH_CTRL_ALGSEL_SHA1 0x00000000
#define HASH_CTRL_ALGSEL_MD5  0x00000080
#define HASH_CTRL_MODESEL     0x00000040
#define HASH_CTRL_DTYPE       0x00000030
#define HASH_CTRL_DTYPE_32B   0x00000000
#define HASH_CTRL_DTYPE_16B   0x00000010
#define HASH_CTRL_DTYPE_8B    0x00000020
#define HASH_CTRL_DTYPE_1B    0x00000030
#define HASH_CTRL_DMAEN       0x00000008
#define HASH_CTRL_INITCAL     0x00000004

//HASH Status register
#define HASH_STS_BUSY         0x00000008
#define HASH_STS_DMA          0x00000004
#define HASH_STS_DCALCINT     0x00000002
#define HASH_STS_INDATAINT    0x00000001

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Hash related functions
error_t hashInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
