/**
 * @file zuc.h
 * @brief ZUC stream cipher (ZUC-128 and ZUC-256)
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

#ifndef _ZUC_H
#define _ZUC_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ZUC algorithm context
 **/

typedef struct
{
   uint32_t r1;
   uint32_t r2;
   uint32_t s[16];
   uint32_t ks;
   size_t n;
} ZucContext;


//ZUC related functions
error_t zucInit(ZucContext *context, const uint8_t *key, size_t keyLen,
   const uint8_t *iv, size_t ivLen);

void zucGenerateKeyStream(ZucContext *context, uint32_t *output,
   size_t length);

void zucCipher(ZucContext *context, const uint8_t *input, uint8_t *output,
   size_t length);

void zucDeinit(ZucContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
