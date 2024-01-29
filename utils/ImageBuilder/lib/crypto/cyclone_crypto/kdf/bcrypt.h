/**
 * @file bcrypt.h
 * @brief bcrypt password hashing function
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

#ifndef _BCRYPT_H
#define _BCRYPT_H

//Dependencies
#include "core/crypto.h"
#include "cipher/blowfish.h"

//Minimum acceptable value for cost parameter
#ifndef BCRYPT_MIN_COST
   #define BCRYPT_MIN_COST 3
#elif (BCRYPT_MIN_COST < 3)
   #error BCRYPT_MIN_COST parameter is not valid
#endif

//Maximum acceptable value for cost parameter
#ifndef BCRYPT_MAX_COST
   #define BCRYPT_MAX_COST 31
#elif (BCRYPT_MAX_COST < BCRYPT_MIN_COST || BCRYPT_MAX_COST > 31)
   #error BCRYPT_MAX_COST parameter is not valid
#endif

//Length of bcrypt hash string
#define BCRYPT_HASH_STRING_LEN 60

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//bcrypt related functions
error_t bcryptHashPassword(const PrngAlgo *prngAlgo, void *prngContext,
   uint_t cost, const char_t *password, char_t *hash, size_t *hashLen);

error_t bcryptVerifyPassword(const char_t *password, const char_t *hash);

error_t bcrypt(uint_t cost, const uint8_t *salt, const char_t *password,
   char_t *hash, size_t *hashLen);

error_t eksBlowfishSetup(BlowfishContext *context, uint_t cost,
   const uint8_t *salt, size_t saltLen, const char_t *password,
   size_t passwordLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
