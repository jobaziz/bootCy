/**
 * @file verify_sign.c
 * @brief CycloneBOOT IAP image data signature module
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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

// Switch to the appropriate trace level
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include "security/verify_sign.h"

#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)

//Private function forward declaration
#if (VERIFY_RSA_SUPPORT == ENABLED)
cboot_error_t signVerifyRsa(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength);
#endif
#if (VERIFY_ECDSA_SUPPORT == ENABLED)
cboot_error_t signVerifyEcdsa(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength);
#endif

/**
 * @brief Initialize signature material for further signature hash computation
 * and signature verification.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code
 **/

cboot_error_t signInit(VerifyContext *context)
{
   error_t error;
   VerifySettings *settings;

#if (VERIFY_RSA_SUPPORT == ENABLED)
   RsaPublicKey publicKey;
#endif
#if (VERIFY_ECDSA_SUPPORT == ENABLED)
   EcDomainParameters ecParams;
#endif

   // Check parameter validity
   if (context == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Initialize status code
   error = NO_ERROR;

   // Check user settings
   if (settings->signAlgo == VERIFY_SIGN_NONE || settings->signHashAlgo == NULL ||
      settings->signKey == NULL || settings->signKeyLen == 0)
      return CBOOT_ERROR_INVALID_VALUE;

   // Is signature RSA algorithm?
   if (settings->signAlgo == VERIFY_SIGN_RSA)
   {
#if (VERIFY_RSA_SUPPORT == ENABLED)
      // Initialize signature algo context
      settings->signHashAlgo->init(context->checkContext);

      // Set digest length
      context->imageCheckDigestSize = settings->signHashAlgo->digestSize;

      // Initialize RSA public key
      rsaInitPublicKey(&publicKey);

      // Decode pem key file into RSA public key
      error = pemImportRsaPublicKey(settings->signKey, settings->signKeyLen, &publicKey);

      // Check status code
      if (!error)
      {
         // Set check data (signature) size
         context->checkDataSize = publicKey.n.size * sizeof(publicKey.n.size);

         // Free RSA public key
         rsaFreePublicKey(&publicKey);
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   // Is signature ECDSA algorithm?
   else if (settings->signAlgo == VERIFY_SIGN_ECDSA)
   {
#if (VERIFY_ECDSA_SUPPORT == ENABLED)
      // Initialize signature algo context
      settings->signHashAlgo->init(context->checkContext);

      // Set digest length
      context->imageCheckDigestSize = settings->signHashAlgo->digestSize;

      // Initialize EC domain parameters
      ecInitDomainParameters(&ecParams);

      // Load EC domain parameters
      error = ecLoadDomainParameters(&ecParams, SECP256K1_CURVE);

      // Check status code
      if (!error)
      {
            // Set check data (signature) size
            context->checkDataSize = mpiGetByteLength(&ecParams.q) * 2;

            // EC domain parameters
            ecFreeDomainParameters(&ecParams);
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      // Debug message
      TRACE_ERROR("Signature algorithm not supported!\r\n");
      return CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   // Return status code
   return CBOOT_NO_ERROR;
}

/**
 * @brief Verify received image firmware signature.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code
 **/

cboot_error_t signVerify(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength)
{
   cboot_error_t cerror;
   VerifySettings *settings;

   // Check parameters validity
   if (context == NULL || verifyData == NULL || verifyDataLength == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Initialize status code
   cerror = CBOOT_NO_ERROR;

   // Check authentification hash algo
   if (settings->signAlgo == VERIFY_SIGN_NONE)
      return CBOOT_ERROR_INVALID_VALUE;
   // Is user require RSA signature?
   if (settings->signAlgo == VERIFY_SIGN_RSA)
   {
#if (VERIFY_RSA_SUPPORT == ENABLED)
      // Verify RSA signature
      cerror = signVerifyRsa(context, verifyData, verifyDataLength);
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   // Is user require ECDSA signature?
   else if(settings->signAlgo == VERIFY_SIGN_ECDSA)
   {
#if (VERIFY_ECDSA_SUPPORT == ENABLED)
      // Verify ECDSA signature
      cerror = signVerifyEcdsa(context, verifyData, verifyDataLength);
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      // Debug message
      TRACE_ERROR("Signature algorithm not supported!\r\n");
      cerror = CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   // Return status code
   return cerror;
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


/**
 * @brief Verify RSA signature.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code
 **/

#if (VERIFY_RSA_SUPPORT == ENABLED)
cboot_error_t signVerifyRsa(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength)
{
   error_t error;
   VerifySettings *settings;

   RsaPublicKey publicKey;

   // Check parameter validity
   if (context == NULL || verifyData == NULL || verifyDataLength == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Check signature user settings
   if (settings->signHashAlgo == NULL || settings->signKey == NULL ||
      settings->signKeyLen == 0)
      return CBOOT_ERROR_INVALID_VALUE;

   // Initialize RSA public key
   rsaInitPublicKey(&publicKey);

   // Import PEM RSA public key
   error = pemImportRsaPublicKey(settings->signKey, settings->signKeyLen, &publicKey);
   // Is any error?
   if (error)
   {
      // Debug message
      TRACE_ERROR("RSA public key import failed!\r\n");
      return CBOOT_ERROR_FAILURE;
   }

   error = rsassaPkcs1v15Verify(&publicKey, settings->signHashAlgo,
                              context->imageCheckDigest, verifyData, verifyDataLength);
   // Is any error?
   if (error)
   {
      // Debug message
      TRACE_ERROR("RSA signature verification failed!\r\n");
      return CBOOT_ERROR_FAILURE;
   }

   // Successful process
   return CBOOT_NO_ERROR;
}
#endif

/**
 * @brief Verify ECDSA signature.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code
 **/

#if (VERIFY_ECDSA_SUPPORT == ENABLED)
cboot_error_t signVerifyEcdsa(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength)
{
   error_t error;
   VerifySettings *settings;

   EcDomainParameters ecParams;
   EcdsaSignature ecdsaSignature;
   EcPoint publicKey;
   uint8_t *p;

   // Intialize status code
   error = NO_ERROR;

   // Check parameter validity
   if (context == NULL || verifyData == NULL || verifyDataLength = 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Check signature user settings
   if (settings->signHashAlgo == NULL || settings->signKey == NULL ||
      settings->signKeyLen == 0)
      return CBOOT_ERROR_INVALID_VALUE;

   // Initialize EC domain parameters
   ecInitDomainParameters(&ecParams);
   // Initialize ECDSA signature
   ecdsaInitSignature(&ecdsaSignature);
   // Initialize EC public keys
   ecInit(&publicKey);

   // Beginning of exceptions handling
   do
   {
      // Point
      p = (uint8_t *)verifyData;

      // Import R value into ECDSA signature structure from receive check data
      error = mpiImport(&ecdsaSignature.r, p, verifyDataLength / 2, MPI_FORMAT_BIG_ENDIAN);
      // Is any error?
      if (error)
         break;

      p += verifyDataLength / 2;

      // Import S value into ECDSA signature structure from receive check data
      error = mpiImport(&ecdsaSignature.s, p, verifyDataLength / 2, MPI_FORMAT_BIG_ENDIAN);
      // Is any error?
      if (error)
         break;

      // Load EC domain parameters
      error = ecLoadDomainParameters(&ecParams, SECP256K1_CURVE);
      // Is any error?
      if (error)
         break;

      // Decode the PEM file that contains the EC public key
      error = pemImportEcPublicKey(settings->signKey, settings->signKeyLen,
                                    &publicKey);
      // Is any error?
      if (error)
         break;

      // Verify EDCSA signature
      error = ecdsaVerifySignature(&ecParams, &publicKey, context->imageCheckDigest,
                                    context->imageCheckDigestLength, &ecdsaSignature);

      // Is any error?
      if (error)
      {
         // Debug message
         TRACE_ERROR("ECDSA signature verification failed!\r\n");
         break;
      }
   } while (0);

   // Release previously allocated resources
   ecFreeDomainParameters(&ecParams);
   ecdsaFreeSignature(&ecdsaSignature);
   ecFree(&publicKey);

   // Return Status code
   if(error)
      return CBOOT_ERROR_FAILURE;
   else
      return CBOOT_NO_ERROR;
}
#endif

#endif
