/**
 * @file x509_key_format.h
 * @brief Formatting of ASN.1 encoded keys
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

#ifndef _X509_KEY_FORMAT_H
#define _X509_KEY_FORMAT_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Key formatting functions
error_t x509FormatSubjectPublicKeyInfo(const X509SubjectPublicKeyInfo *publicKeyInfo,
   const void *publicKey, uint8_t *keyId, uint8_t *output, size_t *written);

error_t x509FormatAlgorithmIdentifier(const X509SubjectPublicKeyInfo *publicKeyInfo,
   const void *params, uint8_t *output, size_t *written);

error_t x509FormatRsaPublicKey(const X509RsaPublicKey *rsaPublicKey,
   uint8_t *output, size_t *written);

error_t x509FormatRsaPssParameters(const X509RsaPssParameters *rsaPssParams,
   uint8_t *output, size_t *written);

error_t x509FormatRsaPssHashAlgo(const X509RsaPssParameters *rsaPssParams,
   uint8_t *output, size_t *written);

error_t x509FormatRsaPssMaskGenAlgo(const X509RsaPssParameters *rsaPssParams,
   uint8_t *output, size_t *written);

error_t x509FormatRsaPssMaskGenHashAlgo(const X509RsaPssParameters *rsaPssParams,
   uint8_t *output, size_t *written);

error_t x509FormatRsaPssSaltLength(const X509RsaPssParameters *rsaPssParams,
   uint8_t *output, size_t *written);

error_t x509FormatDsaPublicKey(const X509DsaPublicKey *dsaPublicKey,
   uint8_t *output, size_t *written);

error_t x509FormatDsaParameters(const X509DsaParameters *dsaParams,
   uint8_t *output, size_t *written);

error_t x509FormatEcPublicKey(const X509EcPublicKey *ecPublicKey,
   uint8_t *output, size_t *written);

error_t x509FormatEcParameters(const X509EcParameters *ecParams,
   uint8_t *output, size_t *written);

error_t x509ExportRsaPublicKey(const RsaPublicKey *publicKey,
   uint8_t *output, size_t *written);

error_t x509ExportRsaPrivateKey(const RsaPrivateKey *privateKey,
   uint8_t *output, size_t *written);

error_t x509ExportDsaPublicKey(const DsaPublicKey *publicKey,
   uint8_t *output, size_t *written);

error_t x509ExportDsaPrivateKey(const DsaPrivateKey *privateKey,
   uint8_t *output, size_t *written);

error_t x509ExportDsaParameters(const DsaDomainParameters *params,
   uint8_t *output, size_t *written);

error_t x509ExportEcPublicKey(const X509SubjectPublicKeyInfo *publicKeyInfo,
   const EcPoint *publicKey, uint8_t *output, size_t *written);

error_t x509ExportEddsaPublicKey(const EddsaPublicKey *publicKey,
   size_t publicKeyLen, uint8_t *output, size_t *written);

error_t x509ExportEddsaPrivateKey(const EddsaPrivateKey *privateKey,
   size_t privateKeyLen, uint8_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
