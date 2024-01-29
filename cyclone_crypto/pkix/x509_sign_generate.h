/**
 * @file x509_sign_generate.h
 * @brief RSA/DSA/ECDSA/EdDSA signature generation
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

#ifndef _X509_SIGN_GENERATE_H
#define _X509_SIGN_GENERATE_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Signature generation callback function
 **/

typedef error_t (*X509SignGenCallback)(const PrngAlgo *prngAlgo,
   void *prngContext, const X509OctetString *tbsData,
   const X509SignAlgoId *signAlgoId,
   const X509SubjectPublicKeyInfo *publicKeyInfo, const void *privateKey,
   uint8_t *output, size_t *written);


//X.509 related functions
error_t x509RegisterSignGenCallback(X509SignGenCallback callback);

error_t x509GenerateSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const X509OctetString *tbsData, const X509SignAlgoId *signAlgoId,
   const X509SubjectPublicKeyInfo *publicKeyInfo, const void *privateKey,
   uint8_t *output, size_t *written);

error_t x509GenerateRsaSignature(const X509OctetString *tbsData,
   const HashAlgo *hashAlgo, const RsaPrivateKey *privateKey, uint8_t *output,
   size_t *written);

error_t x509GenerateRsaPssSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const X509OctetString *tbsData, const HashAlgo *hashAlgo, size_t saltLen,
   const RsaPrivateKey *privateKey, uint8_t *output, size_t *written);

error_t x509GenerateDsaSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const X509OctetString *tbsData, const HashAlgo *hashAlgo,
   const DsaPrivateKey *privateKey, uint8_t *output, size_t *written);

error_t x509GenerateEcdsaSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const X509OctetString *tbsData, const HashAlgo *hashAlgo,
   const X509SubjectPublicKeyInfo *publicKeyInfo,
   const EcPrivateKey *privateKey, uint8_t *output, size_t *written);

error_t x509GenerateSm2Signature(const PrngAlgo *prngAlgo, void *prngContext,
   const X509OctetString *tbsData, const HashAlgo *hashAlgo,
   const EcPrivateKey *privateKey, uint8_t *output, size_t *written);

error_t x509GenerateEd25519Signature(const X509OctetString *tbsData,
   const EddsaPrivateKey *privateKey, uint8_t *output, size_t *written);

error_t x509GenerateEd448Signature(const X509OctetString *tbsData,
   const EddsaPrivateKey *privateKey, uint8_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
