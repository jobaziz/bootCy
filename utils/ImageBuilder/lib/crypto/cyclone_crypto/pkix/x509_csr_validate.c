/**
 * @file x509_csr_validate.c
 * @brief CSR validation
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_csr_validate.h"
#include "pkix/x509_signature.h"
#include "debug.h"

//Check crypto library configuration
#if (X509_SUPPORT == ENABLED)


/**
 * @brief CSR validation
 * @param[in] csrInfo Pointer to the CSR to be verified
 * @return Error code
 **/

error_t x509ValidateCsr(const X509CsrInfo *csrInfo)
{
   error_t error;

   //Check parameters
   if(csrInfo == NULL)
      return ERROR_INVALID_PARAMETER;

   //The ASN.1 DER-encoded certificationRequestInfo is used as the input
   //to the signature function
   error = x509VerifySignature(csrInfo->certReqInfo.rawData,
      csrInfo->certReqInfo.rawDataLen, &csrInfo->signatureAlgo,
      &csrInfo->certReqInfo.subjectPublicKeyInfo, &csrInfo->signatureValue);

   //Return status code
   return error;
}

#endif
