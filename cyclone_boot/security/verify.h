/**
 * @file verify.h
 * @brief CycloneBOOT image data verification module
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

#ifndef _VERIFY_H
#define _VERIFY_H

// Dependencies
#include "boot_config.h"
#include "core/crypto.h"
#include "core/cboot_error.h"
#include "hash/hash_algorithms.h"
#include "core/crc32.h"
#include "debug.h"

//Integrity verification support
#ifndef VERIFY_INTEGRITY_SUPPORT
#define VERIFY_INTEGRITY_SUPPORT ENABLED
#elif (VERIFY_INTEGRITY_SUPPORT != ENABLED && VERIFY_INTEGRITY_SUPPORT != DISABLED)
   #error VERIFY_INTEGRITY_SUPPORT parameter is not valid!
#endif

//Authentication verification support
#ifndef VERIFY_AUTHENTICATION_SUPPORT
#define VERIFY_AUTHENTICATION_SUPPORT DISABLED
#elif (VERIFY_AUTHENTICATION_SUPPORT != ENABLED && VERIFY_AUTHENTICATION_SUPPORT != DISABLED)
   #error VERIFY_INTEGRITY_SUPPORT parameter is not valid!
#endif

//Signature verification support
#ifndef VERIFY_SIGNATURE_SUPPORT
#define VERIFY_SIGNATURE_SUPPORT DISABLED
#elif (VERIFY_SIGNATURE_SUPPORT != ENABLED && VERIFY_SIGNATURE_SUPPORT != DISABLED)
   #error VERIFY_INTEGRITY_SUPPORT parameter is not valid!
#endif

//RSA key support
#ifndef VERIFY_RSA_SUPPORT
   #define VERIFY_RSA_SUPPORT DISABLED
#elif (VERIFY_RSA_SUPPORT != ENABLED && VERIFY_RSA_SUPPORT != DISABLED)
   #error VERIFY_RSA_SUPPORT parameter is not valid!
#endif

//ECDSA key support
#ifndef VERIFY_ECDSA_SUPPORT
   #define VERIFY_ECDSA_SUPPORT DISABLED
#elif (VERIFY_ECDSA_SUPPORT != ENABLED && VERIFY_ECDSA_SUPPORT != DISABLED)
   #error VERIFY_ECDSA_SUPPORT parameter is not valid!
#endif

//Add authentication related dependencies
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
#include "cipher/cipher_algorithms.h"
#include "mac/mac_algorithms.h"
#endif

//Add signature related dependencies
#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)
   #if VERIFY_RSA_SUPPORT == ENABLED
      #include "pkc/rsa.h"
   #endif
   #if VERIFY_ECDSA_SUPPORT == ENABLED
      #include "ecc/ecdsa.h"
   #endif
   #include "pkix/pem_import.h"
#endif

//Maximum check data context size
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
#define MAX_CHECK_CONTEXT_SIZE sizeof(HmacContext)
#else
#define MAX_CHECK_CONTEXT_SIZE sizeof(HashContext)
#endif

//Make sure MAX_HASH_DIGEST_SIZE is always defined
#ifndef MAX_HASH_DIGEST_SIZE
#define MAX_HASH_DIGEST_SIZE CRC32_DIGEST_SIZE
#endif

//Maximum digest size
#define VERIFY_MAX_HASH_DIGEST_SIZE MAX_HASH_DIGEST_SIZE


/**
 * @brief Verification method
 **/

typedef enum
{
   VERIFY_METHOD_INTEGRITY,
   VERIFY_METHOD_AUTHENTICATION,
   VERIFY_METHOD_SIGNATURE
} VerifyMethod;


/**
 * @brief Verification authentication algorithms
 **/

typedef enum
{
   VERIFY_AUTH_NONE,
   VERIFY_AUTH_HMAC
} VerifyAuthAlgo;


/**
 * @brief Verification signature algorithms
 **/

typedef enum
{
   VERIFY_SIGN_NONE,
   VERIFY_SIGN_RSA,
   VERIFY_SIGN_ECDSA,
} VerifySignAlgo;


/**
 * @brief Verification settings
 **/

typedef struct
{
   VerifyMethod verifyMethod;                      ///<Image verification method
#if (VERIFY_INTEGRITY_SUPPORT == ENABLED)
   const HashAlgo *integrityAlgo;                  ///<Image integrity algorithm
#endif
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
   VerifyAuthAlgo authAlgo;                        ///<Image authentication algorithm
   const HashAlgo *authHashAlgo;                   ///<Image authentication hash algorithm
   const char_t *authKey;                          ///<Image authentication key
   size_t authKeyLen;                              ///<Image authentication key size
#endif
#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)
   VerifySignAlgo signAlgo;                        ///<Image signature algorithm
   const HashAlgo *signHashAlgo;                   ///<Image signature hash algorithm
   const char_t *signKey;                          ///<Image signature public key (PEM format)
   size_t signKeyLen;                              ///<Image signature public key size
#endif
} VerifySettings;


/**
 * @brief Verification context
 **/

typedef struct
{
   VerifySettings verifySettings;                  ///<Image verification user settings

   uint8_t checkContext[MAX_CHECK_CONTEXT_SIZE];   ///<Image verification algorithm context
   size_t checkDataSize;                           ///<Image verification check data size

   uint8_t imageCheckDigest[MAX_HASH_DIGEST_SIZE]; ///<Image verification digest buffer
   size_t imageCheckDigestSize;                    ///<Image verification digest buffer size
} VerifyContext;


//CycloneBOOT verification related functions
cboot_error_t verifyInit(VerifyContext *context, VerifySettings *settings);
cboot_error_t verifyProcess(VerifyContext *context, uint8_t *data, size_t length);
cboot_error_t verifyConfirm(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength);
cboot_error_t verifyGenerateCheckData(VerifyContext *context, uint8_t *checkData, size_t checkDataSize, size_t *checkDataLength);

#endif // !_CIPHER_H
