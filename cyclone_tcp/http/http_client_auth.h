/**
 * @file http_client_auth.h
 * @brief HTTP authentication
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
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

#ifndef _HTTP_CLIENT_AUTH_H
#define _HTTP_CLIENT_AUTH_H

//Dependencies
#include "core/net.h"
#include "http/http_client.h"

//Maximum digest size
#if (HTTP_CLIENT_SHA512_256_SUPPORT == ENABLED)
   #define HTTP_CLIENT_MAX_HASH_DIGEST_SIZE 32
#elif (HTTP_CLIENT_SHA256_SUPPORT == ENABLED)
   #define HTTP_CLIENT_MAX_HASH_DIGEST_SIZE 32
#else
   #define HTTP_CLIENT_MAX_HASH_DIGEST_SIZE 16
#endif

//Maximum response length
#if (HTTP_CLIENT_SHA512_256_SUPPORT == ENABLED)
   #define HTTP_CLIENT_MAX_RESPONSE_LEN 64
#elif (HTTP_CLIENT_SHA256_SUPPORT == ENABLED)
   #define HTTP_CLIENT_MAX_RESPONSE_LEN 64
#else
   #define HTTP_CLIENT_MAX_RESPONSE_LEN 32
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief WWW-Authenticate header field
 **/

typedef struct
{
   HttpAuthMode mode;         ///<Authentication scheme
   const char_t *realm;       ///<Realm
   size_t realmLen;           ///<Length of the realm
#if (HTTP_CLIENT_DIGEST_AUTH_SUPPORT == ENABLED)
   HttpAuthQop qop;           ///<Quality of protection
   const HashAlgo *algorithm; ///<Digest algorithm
   const char_t *nonce;       ///<Nonce value
   size_t nonceLen;           ///<Length of the nonce value
   const char_t *opaque;      ///<Opaque parameter
   size_t opaqueLen;          ///<Length of the opaque parameter
   bool_t stale;              ///<Stale flag
#endif
} HttpWwwAuthenticateHeader;


//HTTP client related functions
void httpClientInitAuthParams(HttpClientAuthParams *authParams);

error_t httpClientFormatAuthorizationField(HttpClientContext *context);

error_t httpClientParseWwwAuthenticateField(HttpClientContext *context,
   const char_t *value);

void httpClientParseQopParam(const HttpParam *param,
   HttpWwwAuthenticateHeader *authHeader);

void httpClientParseAlgorithmParam(const HttpParam *param,
   HttpWwwAuthenticateHeader *authHeader);

error_t httpClientComputeDigest(HttpClientAuthParams *authParams,
   const char_t *method, size_t methodLen, const char_t *uri,
   size_t uriLen, char_t *response);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
