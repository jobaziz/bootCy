/**
 * @file sm3.h
 * @brief SM3 hash function
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

#ifndef _SM3_H
#define _SM3_H

//Dependencies
#include "core/crypto.h"

//Application specific context
#ifndef SM3_PRIVATE_CONTEXT
   #define SM3_PRIVATE_CONTEXT
#endif

//SM3 block size
#define SM3_BLOCK_SIZE 64
//SM3 digest size
#define SM3_DIGEST_SIZE 32
//Minimum length of the padding string
#define SM3_MIN_PAD_SIZE 9
//Common interface for hash algorithms
#define SM3_HASH_ALGO (&sm3HashAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SM3 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[8];
      uint8_t digest[32];
   };
   union
   {
      uint32_t w[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
   SM3_PRIVATE_CONTEXT
} Sm3Context;


//SM3 related constants
extern const uint8_t SM3_OID[6];
extern const HashAlgo sm3HashAlgo;

//SM3 related functions
error_t sm3Compute(const void *data, size_t length, uint8_t *digest);
void sm3Init(Sm3Context *context);
void sm3Update(Sm3Context *context, const void *data, size_t length);
void sm3Final(Sm3Context *context, uint8_t *digest);
void sm3FinalRaw(Sm3Context *context, uint8_t *digest);
void sm3ProcessBlock(Sm3Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
