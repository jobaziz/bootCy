/**
 * @file sm2.h
 * @brief SM2 signature algorithm
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

#ifndef _SM2_H
#define _SM2_H

//Dependencies
#include "core/crypto.h"
#include "ecc/ecdsa.h"

//SM2 identifiers
#define SM2_DEFAULT_ID "1234567812345678"
#define SM2_TLS13_ID "TLSv1.3+GM+Cipher+Suite"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SM2 related constants
extern const uint8_t SM2_WITH_SM3_OID[8];

//SM2 related functions
error_t sm2GenerateSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const EcDomainParameters *params, const EcPrivateKey *privateKey,
   const HashAlgo *hashAlgo, const char_t *id, size_t idLen,
   const void *message, size_t messageLen, EcdsaSignature *signature);

error_t sm2VerifySignature(const EcDomainParameters *params,
   const EcPublicKey *publicKey, const HashAlgo *hashAlgo,
   const char_t *id, size_t idLen, const void *message, size_t messageLen,
   const EcdsaSignature *signature);

error_t sm2ComputeZa(const HashAlgo *hashAlgo, HashContext *hashContext,
   const EcDomainParameters *params, const EcPublicKey *pa, const char_t *ida,
   size_t idaLen, uint8_t *za);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
