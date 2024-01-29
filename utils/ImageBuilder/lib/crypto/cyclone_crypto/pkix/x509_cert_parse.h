/**
 * @file x509_cert_parse.h
 * @brief X.509 certificate parsing
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

#ifndef _X509_CERT_PARSE_H
#define _X509_CERT_PARSE_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//X.509 related functions
error_t x509ParseCertificate(const uint8_t *data, size_t length,
   X509CertificateInfo *certInfo);

error_t x509ParseCertificateEx(const uint8_t *data, size_t length,
   X509CertificateInfo *certInfo, bool_t ignoreUnknown);

error_t x509ParseTbsCertificate(const uint8_t *data, size_t length,
   size_t *totalLength, X509TbsCertificate *tbsCert, bool_t ignoreUnknown);

error_t x509ParseVersion(const uint8_t *data, size_t length,
   size_t *totalLength, X509Version *version);

error_t x509ParseSerialNumber(const uint8_t *data, size_t length,
   size_t *totalLength, X509SerialNumber *serialNumber);

error_t x509ParseName(const uint8_t *data, size_t length,
   size_t *totalLength, X509Name *name);

error_t x509ParseNameAttribute(const uint8_t *data, size_t length,
   size_t *totalLength, X509NameAttribute *nameAttribute);

error_t x509ParseValidity(const uint8_t *data, size_t length,
   size_t *totalLength, X509Validity *validity);

error_t x509ParseTime(const uint8_t *data, size_t length,
   size_t *totalLength, DateTime *dateTime);

error_t x509ParseIssuerUniqueId(const uint8_t *data, size_t length,
   size_t *totalLength);

error_t x509ParseSubjectUniqueId(const uint8_t *data, size_t length,
   size_t *totalLength);

error_t x509ParseExtensions(const uint8_t *data, size_t length,
   size_t *totalLength, X509Extensions *extensions, bool_t ignoreUnknown);

error_t x509ParseExtension(const uint8_t *data, size_t length,
   size_t *totalLength, X509Extension *extension);

error_t x509ParseBasicConstraints(bool_t critical, const uint8_t *data,
   size_t length, X509BasicConstraints *basicConstraints);

error_t x509ParseNameConstraints(bool_t critical, const uint8_t *data,
   size_t length, X509NameConstraints *nameConstraints);

error_t x509ParsePolicyConstraints(bool_t critical, const uint8_t *data,
   size_t length);

error_t x509ParsePolicyMappings(bool_t critical, const uint8_t *data,
   size_t length);

error_t x509ParseInhibitAnyPolicy(bool_t critical, const uint8_t *data,
   size_t length);

error_t x509ParseKeyUsage(bool_t critical, const uint8_t *data,
   size_t length, X509KeyUsage *keyUsage);

error_t x509ParseExtendedKeyUsage(bool_t critical, const uint8_t *data,
   size_t length, X509ExtendedKeyUsage *extKeyUsage);

error_t x509ParseSubjectAltName(bool_t critical, const uint8_t *data,
   size_t length, X509SubjectAltName *subjectAltName);

error_t x509ParseGeneralSubtrees(const uint8_t *data, size_t length);

error_t x509ParseGeneralSubtree(const uint8_t *data, size_t length,
   size_t *totalLength, X509GeneralName *generalName);

error_t x509ParseGeneralName(const uint8_t *data, size_t length,
   size_t *totalLength, X509GeneralName *generalName);

error_t x509ParseSubjectKeyId(bool_t critical, const uint8_t *data,
   size_t length, X509SubjectKeyId *subjectKeyId);

error_t x509ParseAuthorityKeyId(bool_t critical, const uint8_t *data,
   size_t length, X509AuthorityKeyId *authKeyId);

error_t x509ParseNsCertType(bool_t critical, const uint8_t *data,
   size_t length, X509NsCertType *nsCertType);

error_t x509ParseSignatureAlgo(const uint8_t *data, size_t length,
   size_t *totalLength, X509SignatureAlgoId *signatureAlgo);

error_t x509ParseSignatureValue(const uint8_t *data, size_t length,
   size_t *totalLength, X509SignatureValue *signatureValue);

error_t x509ParseInt(const uint8_t *data, size_t length, uint_t *value);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
