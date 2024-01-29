/**
 * @file x509_csr_create.c
 * @brief CSR (Certificate Signing Request) generation
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_csr_create.h"
#include "pkix/x509_csr_format.h"
#include "pkix/x509_sign_format.h"
#include "encoding/asn1.h"
#include "debug.h"

//Check crypto library configuration
#if (X509_SUPPORT == ENABLED)


/**
 * @brief Generate a CSR (Certificate Signing Request)
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[in] certReqInfo Certificate request information
 * @param[in] subjectPublicKey Pointer to the subject's public key
 * @param[in] signatureAlgo Signature algorithm
 * @param[in] signerPrivateKey Pointer to the subject's private key
 * @param[out] output Buffer where to store the CSR
 * @param[out] written Length of the resulting CSR
 * @return Error code
 **/

error_t x509CreateCsr(const PrngAlgo *prngAlgo, void *prngContext,
   const X509CertRequestInfo *certReqInfo, const void *subjectPublicKey,
   const X509SignAlgoId *signatureAlgo, const void *signerPrivateKey,
   uint8_t *output, size_t *written)
{
   error_t error;
   size_t n;
   size_t length;
   uint8_t *p;
   X509OctetString tbsData;
   Asn1Tag tag;

   //Check parameters
   if(certReqInfo == NULL || subjectPublicKey == NULL ||
      signatureAlgo == NULL || signerPrivateKey == NULL || written == NULL)
   {
      return ERROR_INVALID_PARAMETER;
   }

   //Point to the buffer where to write the CSR
   p = output;
   //Length of the CSR
   length = 0;

   //Format CertificationRequestInfo structure
   error = x509FormatCertRequestInfo(certReqInfo, subjectPublicKey, p, &n);
   //Any error to report?
   if(error)
      return error;

   //The ASN.1 DER-encoded CertificationRequestInfo is used as the input to
   //the signature function
   tbsData.value = p;
   tbsData.length = n;

   //Advance data pointer
   p += n;
   length += n;

   //Format SignatureAlgorithm structure
   error = x509FormatSignatureAlgo(signatureAlgo, p, &n);
   //Any error to report?
   if(error)
      return error;

   //Advance data pointer
   p += n;
   length += n;

   //Format Signature structure
   error = x509FormatSignatureValue(prngAlgo, prngContext, &tbsData,
      signatureAlgo, &certReqInfo->subjectPublicKeyInfo, signerPrivateKey,
      p, &n);
   //Any error to report?
   if(error)
      return error;

   //Advance data pointer
   p += n;
   length += n;

   //The CSR is encapsulated within a sequence
   tag.constructed = TRUE;
   tag.objClass = ASN1_CLASS_UNIVERSAL;
   tag.objType = ASN1_TYPE_SEQUENCE;
   tag.length = length;
   tag.value = output;

   //Write the corresponding ASN.1 tag
   error = asn1WriteTag(&tag, FALSE, output, &n);
   //Any error to report?
   if(error)
      return error;

   //Total number of bytes that have been written
   *written = n;

   //Successful processing
   return NO_ERROR;
}

#endif
