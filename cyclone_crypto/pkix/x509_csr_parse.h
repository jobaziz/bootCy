/**
 * @file x509_csr_parse.h
 * @brief CSR (Certificate Signing Request) parsing
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

#ifndef _X509_CSR_PARSE_H
#define _X509_CSR_PARSE_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CSR related functions
error_t x509ParseCsr(const uint8_t *data, size_t length,
   X509CsrInfo *csrInfo);

error_t x509ParseCertRequestInfo(const uint8_t *data, size_t length,
   size_t *totalLength, X509CertRequestInfo *certReqInfo);

error_t x509ParseAttributes(const uint8_t *data, size_t length,
   size_t *totalLength, X509Attributes *attributes);

error_t x509ParseAttribute(const uint8_t *data, size_t length,
   size_t *totalLength, X509Attribute *attribute);

error_t x509ParseChallengePassword(const uint8_t *data, size_t length,
   X509ChallengePassword *challengePwd);

error_t x509ParseExtensionRequest(const uint8_t *data, size_t length,
   X509Extensions *extensionReq);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
