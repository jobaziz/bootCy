/**
 * @file ocsp_resp_parse.h
 * @brief OCSP response parsing
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

#ifndef _OCSP_RESP_PARSE_H
#define _OCSP_RESP_PARSE_H

//Dependencies
#include "ocsp/ocsp_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//OCSP related functions
error_t ocspParseResponse(const uint8_t *data, size_t length,
   OcspResponse *response);

error_t ocspParseResponseStatus(const uint8_t *data, size_t length,
   size_t *totalLength, OcspResponseStatus *status);

error_t ocspParseResponseBytes(const uint8_t *data, size_t length,
   OcspResponse *response);

error_t ocspParseBasicResponse(const uint8_t *data, size_t length,
   OcspBasicResponse *basicResponse);

error_t ocspParseTbsResponseData(const uint8_t *data, size_t length,
   size_t *totalLength, OcspTbsResponseData *tbsResponseData);

error_t ocspParseVersion(const uint8_t *data, size_t length,
   size_t *totalLength, OcspVersion *version);

error_t ocspParseResponderId(const uint8_t *data, size_t length,
   size_t *totalLength, OcspResponderId *responderId);

error_t ocspParseResponses(const uint8_t *data, size_t length,
   size_t *totalLength, OcspTbsResponseData *tbsResponseData);

error_t ocspParseSingleResponse(const uint8_t *data, size_t length,
   size_t *totalLength, OcspSingleResponse *singleResponse);

error_t ocspParseCertId(const uint8_t *data, size_t length,
   size_t *totalLength, OcspCertId *certId);

error_t ocspParseHashAlgo(const uint8_t *data, size_t length,
   size_t *totalLength, OcspCertId *certId);

error_t ocspParseCertStatus(const uint8_t *data, size_t length,
   size_t *totalLength, OcspSingleResponse *singleResponse);

error_t ocspParseRevokedInfo(const uint8_t *data, size_t length,
   OcspRevokedInfo *revokedInfo);

error_t ocspParseRevocationReason(const uint8_t *data, size_t length,
   X509CrlReasons *revocationReason);

error_t ocspParseCerts(const uint8_t *data, size_t length,
   OcspCerts *certs);

error_t ocspParseResponseExtensions(const uint8_t *data, size_t length,
   OcspExtensions *responseExtensions);

error_t ocspParseSingleExtensions(const uint8_t *data, size_t length,
   OcspSingleExtensions *singleExtensions);

error_t ocspParseNonceExtension(bool_t critical, const uint8_t *data,
   size_t length, X509OctetString *nonce);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
