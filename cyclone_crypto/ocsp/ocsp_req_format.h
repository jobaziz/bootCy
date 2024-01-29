/**
 * @file ocsp_req_format.h
 * @brief OCSP request formatting
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

#ifndef _OCSP_REQ_FORMAT_H
#define _OCSP_REQ_FORMAT_H

//Dependencies
#include "ocsp/ocsp_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//OCSP related functions
error_t ocspFormatRequest(const OcspRequest *request, uint8_t *output,
   size_t *written);

error_t ocspFormatTbsRequest(const OcspTbsRequest *tbsRequest, uint8_t *output,
   size_t *written);

error_t ocspFormatVersion(OcspVersion version, uint8_t *output,
   size_t *written);

error_t ocspFormatRequestList(const OcspSingleRequest *requestList,
   uint_t numRequests, uint8_t *output, size_t *written);

error_t ocspFormatSingleRequest(const OcspSingleRequest *singleRequest,
   uint8_t *output, size_t *written);

error_t ocspFormatCertId(const OcspCertId *certId, uint8_t *output,
   size_t *written);

error_t ocspFormatHashAlgo(const OcspCertId *certId, uint8_t *output,
   size_t *written);

error_t ocspFormatRequestExtensions(const OcspExtensions *extensions,
   uint8_t *output, size_t *written);

error_t ocspFormatNonceExtension(const X509OctetString *nonce, uint8_t *output,
   size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
