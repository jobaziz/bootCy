/**
 * @file x509_cert_create.h
 * @brief X.509 certificate generation
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

#ifndef _X509_CERT_CREATE_H
#define _X509_CERT_CREATE_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//X.509 related functions
error_t x509CreateCertificate(const PrngAlgo *prngAlgo, void *prngContext,
   const X509CertRequestInfo *certReqInfo, const void *subjectPublicKey,
   const X509CertificateInfo *issuerCertInfo, const X509SerialNumber *serialNumber,
   const X509Validity *validity, const X509SignatureAlgoId *signatureAlgo,
   const void *signerPrivateKey, uint8_t *output, size_t *written);

error_t x509FormatTbsCertificate(const PrngAlgo *prngAlgo, void *prngContext,
   const X509SerialNumber *serialNumber, const X509SignatureAlgoId *signatureAlgo,
   const X509Name *issuer, const X509Validity *validity, const X509Name *subject,
   const X509SubjectPublicKeyInfo *subjectPublicKeyInfo, const void *publicKey,
   const X509Extensions *extensions, const X509AuthorityKeyId *authKeyId,
   uint8_t *output, size_t *written);

error_t x509FormatVersion(X509Version version, uint8_t *output,
   size_t *written);

error_t x509FormatSerialNumber(const PrngAlgo *prngAlgo, void *prngContext,
   const X509SerialNumber *serialNumber, uint8_t *output, size_t *written);

error_t x509FormatName(const X509Name *name, uint8_t *output,
   size_t *written);

error_t x509FormatNameAttribute(uint_t type, const uint8_t *oid, size_t oidLen,
   const char_t *value, size_t valueLen, uint8_t *output, size_t *written);

error_t x509FormatValidity(const X509Validity *validity, uint8_t *output,
   size_t *written);

error_t x509FormatTime(const DateTime *dateTime, uint8_t *output,
   size_t *written);

error_t x509FormatExtensions(const X509Extensions *extensions,
   const X509SubjectKeyId *subjectKeyId, const X509AuthorityKeyId *authKeyId,
   uint8_t *output, size_t *written);

error_t x509FormatExtension(const X509Extension *extension, uint8_t *output,
   size_t *written);

error_t x509FormatBasicConstraints(const X509BasicConstraints *basicConstraints,
   uint8_t *output, size_t *written);

error_t x509FormatKeyUsage(const X509KeyUsage *keyUsage, uint8_t *output,
   size_t *written);

error_t x509FormatSubjectAltName(const X509SubjectAltName *subjectAltName,
   uint8_t *output, size_t *written);

error_t x509FormatSubjectKeyId(const X509SubjectKeyId *subjectKeyId,
   uint8_t *output, size_t *written);

error_t x509FormatAuthorityKeyId(const X509AuthorityKeyId *authKeyId,
   uint8_t *output, size_t *written);

error_t x509FormatNsCertType(const X509NsCertType *nsCertType,
   uint8_t *output, size_t *written);

error_t x509FormatSignatureAlgo(const X509SignatureAlgoId *signatureAlgo,
   uint8_t *output, size_t *written);

error_t x509FormatSignatureValue(const PrngAlgo *prngAlgo, void *prngContext,
   const uint8_t *tbsCert, size_t tbsCertLen, const X509SignatureAlgoId *signatureAlgoId,
   const X509SubjectPublicKeyInfo *publicKeyInfo, const void *privateKey,
   uint8_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
