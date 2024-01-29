/**
 * @file ra6_crypto_pkc.h
 * @brief RA6 public-key hardware accelerator
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

#ifndef _RA6_CRYPTO_PKC_H
#define _RA6_CRYPTO_PKC_H

//Dependencies
#include "hw_sce_private.h"
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef RA6_CRYPTO_PKC_SUPPORT
   #define RA6_CRYPTO_PKC_SUPPORT DISABLED
#elif (RA6_CRYPTO_PKC_SUPPORT != ENABLED && RA6_CRYPTO_PKC_SUPPORT != DISABLED)
   #error RA6_CRYPTO_PKC_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief RSA primitive arguments
 **/

typedef struct
{
#if (BSP_FEATURE_CRYPTO_HAS_SCE9 != 0)
   uint32_t n[128];
   uint32_t d[128];
   uint32_t e[1];
   uint32_t m[128];
   uint32_t c[128];
   uint32_t key[160];
   uint32_t wrappedKey[256];
#else
   uint32_t n[64];
   uint32_t d[64];
   uint32_t e[1];
   uint32_t m[64];
   uint32_t c[64];
   uint32_t key[160];
#endif
} Ra6RsaArgs;


/**
 * @brief EC primitive arguments
 **/

typedef struct
{
   uint32_t params[48];
   uint32_t g[24];
   uint32_t d[12];
   uint32_t q[24];
   uint32_t digest[12];
   uint32_t signature[24];
#if (BSP_FEATURE_CRYPTO_HAS_SCE9 != 0)
   uint32_t wrappedKey[32];
#endif
} Ra6EcArgs;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
