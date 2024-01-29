/**
 * @file trivium.h
 * @brief Trivium stream cipher
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

#ifndef _TRIVIUM_H
#define _TRIVIUM_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Trivium algorithm context
 **/

typedef struct
{
   uint8_t s[36];
} TriviumContext;


//Trivium related functions
error_t triviumInit(TriviumContext *context, const uint8_t *key,
   size_t keyLen, const uint8_t *iv, size_t ivLen);

void triviumCipher(TriviumContext *context, const uint8_t *input,
   uint8_t *output, size_t length);

uint8_t triviumGenerateBit(TriviumContext *context);
uint8_t triviumGenerateByte(TriviumContext *context);

void triviumDeinit(TriviumContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
