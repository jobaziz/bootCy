/**
 * @file x509_cert_ext_format.h
 * @brief X.509 extension formatting
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

#ifndef _X509_CERT_EXT_FORMAT_H
#define _X509_CERT_EXT_FORMAT_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//X.509 related functions
error_t x509FormatExtensions(const X509Extensions *extensions,
   const X509SubjectKeyId *subjectKeyId, const X509AuthKeyId *authKeyId,
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

error_t x509FormatAuthorityKeyId(const X509AuthKeyId *authKeyId,
   uint8_t *output, size_t *written);

error_t x509FormatNsCertType(const X509NsCertType *nsCertType,
   uint8_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
