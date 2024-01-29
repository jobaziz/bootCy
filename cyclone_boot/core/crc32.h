/**
 * @file crc32.h
 * @brief CycloneBOOT CRC32 cryptographic integrity tag
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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

#ifndef _CRC32_H
#define _CRC32_H

//Dependencies
#include "core/crypto.h"

//CRC32 block size
#define CRC32_BLOCK_SIZE 64
//CRC32 digest size
#define CRC32_DIGEST_SIZE 4
//Minimum length of the padding string
#define CRC32_MIN_PAD_SIZE 9
//CRC32 algorithm object identifier
#define CRC32_OID crc32Oid
//Common interface for hash algorithms
#define CRC32_HASH_ALGO (&crc32HashAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief CRC32 algorithm context
 **/

typedef struct
{
   uint32_t digest;  ///<CRC32 digest
} Crc32Context;


//CRC32 related constants
extern const HashAlgo crc32HashAlgo;

//CycloneBOOT CRC32 related functions
error_t crc32Compute(const void *data, size_t length, uint8_t *digest);
void crc32Init(Crc32Context *context);
void crc32Update(Crc32Context *context, const void *data, size_t length);
void crc32Final(Crc32Context *context, uint8_t *digest);
void crc32FinalRaw(Crc32Context *context, uint8_t *digest);
void crc32ProcessBlock(Crc32Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
