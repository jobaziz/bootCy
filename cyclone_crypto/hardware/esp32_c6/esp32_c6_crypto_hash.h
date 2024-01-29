/**
 * @file esp32_c6_crypto_hash.h
 * @brief ESP32-C6 hash hardware accelerator
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

#ifndef _ESP32_C6_CRYPTO_HASH_H
#define _ESP32_C6_CRYPTO_HASH_H

//Dependencies
#include "core/crypto.h"

//Hash hardware accelerator
#ifndef ESP32_C6_CRYPTO_HASH_SUPPORT
   #define ESP32_C6_CRYPTO_HASH_SUPPORT DISABLED
#elif (ESP32_C6_CRYPTO_HASH_SUPPORT != ENABLED && ESP32_C6_CRYPTO_HASH_SUPPORT != DISABLED)
   #error ESP32_C6_CRYPTO_HASH_SUPPORT parameter is not valid
#endif

//SHA Mode register
#define SHA_MODE_SHA1   0x00000000
#define SHA_MODE_SHA224 0x00000001
#define SHA_MODE_SHA256 0x00000002

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Hash related functions
void esp32c6ShaInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
