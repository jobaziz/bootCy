/**
 * @file ocsp_resp_validate.h
 * @brief OCSP response validation
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

#ifndef _OCSP_RESP_VALIDATE_H
#define _OCSP_RESP_VALIDATE_H

//Dependencies
#include "ocsp/ocsp_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//OCSP related functions
error_t ocspValidateResponse(const OcspResponse *response,
   const X509CertInfo *certInfo, const X509CertInfo *issuerCertInfo,
   const uint8_t *nonce, size_t nonceLen);

error_t ocspCheckResponseSignature(const OcspBasicResponse *basicResponse,
   const X509CertInfo *issuerCertInfo);

error_t ocspCheckResponderCert(const OcspResponderId *responderId,
   const X509CertInfo *responderCertInfo, const X509CertInfo *issuerCertInfo);

error_t ocspCheckResponderId(const OcspResponderId *responderId,
   const X509CertInfo *issuerCertInfo);

error_t ocspCheckCertId(const OcspCertId *certId, const X509CertInfo *certInfo,
   const X509CertInfo *issuerCertInfo);

error_t ocspCheckValidity(const OcspSingleResponse *singleResponse);

error_t ocspCheckNonce(const OcspExtensions *extensions, const uint8_t *nonce,
   size_t nonceLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
