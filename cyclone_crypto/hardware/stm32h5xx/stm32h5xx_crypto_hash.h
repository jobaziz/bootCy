/**
 * @file stm32h5xx_crypto_hash.h
 * @brief STM32H5 hash hardware accelerator
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

#ifndef _STM32H5XX_CRYPTO_HASH_H
#define _STM32H5XX_CRYPTO_HASH_H

//Dependencies
#include "core/crypto.h"

//Hash hardware accelerator
#ifndef STM32H5XX_CRYPTO_HASH_SUPPORT
   #define STM32H5XX_CRYPTO_HASH_SUPPORT DISABLED
#elif (STM32H5XX_CRYPTO_HASH_SUPPORT != ENABLED && STM32H5XX_CRYPTO_HASH_SUPPORT != DISABLED)
   #error STM32H5XX_CRYPTO_HASH_SUPPORT parameter is not valid
#endif

//ALGO bitfield
#define HASH_CR_ALGO_SHA1       0
#define HASH_CR_ALGO_SHA224     HASH_CR_ALGO_1
#define HASH_CR_ALGO_SHA256     (HASH_CR_ALGO_1 | HASH_CR_ALGO_0)
#define HASH_CR_ALGO_SHA384     (HASH_CR_ALGO_3 | HASH_CR_ALGO_2)
#define HASH_CR_ALGO_SHA512     (HASH_CR_ALGO_3 | HASH_CR_ALGO_2 | HASH_CR_ALGO_1 | HASH_CR_ALGO_0)
#define HASH_CR_ALGO_SHA512_224 (HASH_CR_ALGO_3 | HASH_CR_ALGO_2 | HASH_CR_ALGO_0)
#define HASH_CR_ALGO_SHA512_256 (HASH_CR_ALGO_3 | HASH_CR_ALGO_2 | HASH_CR_ALGO_1)

//DATATYPE bitfield
#define HASH_CR_DATATYPE_32B 0
#define HASH_CR_DATATYPE_16B HASH_CR_DATATYPE_0
#define HASH_CR_DATATYPE_8B  HASH_CR_DATATYPE_1
#define HASH_CR_DATATYPE_1B  (HASH_CR_DATATYPE_1 | HASH_CR_DATATYPE_0)

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
