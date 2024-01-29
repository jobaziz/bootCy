/**
 * @file x509_signature_format.h
 * @brief RSA/DSA/ECDSA/EdDSA signature formatting
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

#ifndef _X509_SIGN_FORMAT_H
#define _X509_SIGN_FORMAT_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//X.509 related functions
error_t x509FormatSignatureAlgo(const X509SignAlgoId *signatureAlgo,
   uint8_t *output, size_t *written);

error_t x509FormatSignatureValue(const PrngAlgo *prngAlgo, void *prngContext,
   const X509OctetString *tbsCert, const X509SignAlgoId *signAlgoId,
   const X509SubjectPublicKeyInfo *publicKeyInfo, const void *privateKey,
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

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
