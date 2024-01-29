/**
 * @file kmac.h
 * @brief KMAC (Keccak Message Authentication Code)
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

#ifndef _KMAC_H
#define _KMAC_H

//Dependencies
#include "core/crypto.h"
#include "xof/cshake.h"

//Application specific context
#ifndef KMAC_PRIVATE_CONTEXT
   #define KMAC_PRIVATE_CONTEXT
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief KMAC algorithm context
 **/

typedef struct
{
   CshakeContext cshakeContext;
   KMAC_PRIVATE_CONTEXT
} KmacContext;


//KMAC related constants
extern const uint8_t kmac128Oid[9];
extern const uint8_t kmac256Oid[9];

//KMAC related functions
error_t kmacCompute(uint_t strength, const void *key, size_t keyLen,
   const void *data, size_t dataLen, const char_t *custom, size_t customLen,
   uint8_t *mac, size_t macLen);

error_t kmacInit(KmacContext *context, uint_t strength, const void *key,
   size_t keyLen, const char_t *custom, size_t customLen);

void kmacUpdate(KmacContext *context, const void *data, size_t dataLen);
error_t kmacFinal(KmacContext *context, uint8_t *mac, size_t macLen);
void kmacDeinit(KmacContext *context);

void kmacRightEncode(size_t value, uint8_t *buffer, size_t *length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
