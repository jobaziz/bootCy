/**
 * @file ra8_crypto_pkc.h
 * @brief RA8 public-key hardware accelerator
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

#ifndef _RA8_CRYPTO_PKC_H
#define _RA8_CRYPTO_PKC_H

//Dependencies
#include "hw_sce_private.h"
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef RA8_CRYPTO_PKC_SUPPORT
   #define RA8_CRYPTO_PKC_SUPPORT DISABLED
#elif (RA8_CRYPTO_PKC_SUPPORT != ENABLED && RA8_CRYPTO_PKC_SUPPORT != DISABLED)
   #error RA8_CRYPTO_PKC_SUPPORT parameter is not valid
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
   uint32_t m[128];
   uint32_t c[128];
   uint32_t key[256];
   uint32_t wrappedKey[300];
} Ra8RsaArgs;


/**
 * @brief EC primitive arguments
 **/

typedef struct
{
   uint32_t g[40];
   uint32_t d[20];
   uint32_t q[40];
   uint32_t digest[20];
   uint32_t signature[40];
   uint32_t wrappedKey[100];
} Ra8EcArgs;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
