/**
 * @file verify_auth.c
 * @brief CycloneBOOT IAP image data authentication module
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
#include "security/verify_auth.h"

#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)

/**
 * @brief Initialize authentification material for further authentification tag
 * computation on received firmware data and verification against
 * received image authentification tag.
 * @param[in] context Pointer to the verification context
 * @return Error code
 **/

cboot_error_t authInit(VerifyContext *context)
{
   error_t error;
   cboot_error_t cerror;
   VerifySettings *settings;

   // Check parameter validity
   if (context == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Initialize status code
   cerror = CBOOT_NO_ERROR;

   // Check user settings
   if (settings->authHashAlgo == NULL || settings->authAlgo == VERIFY_AUTH_NONE)
      return CBOOT_ERROR_INVALID_IMAGE_VERIFY_METHOD;

   // Is hmac authentification algo?
   if (settings->authAlgo == VERIFY_AUTH_HMAC)
   {
      // Initialize authentification hmac context
      error = hmacInit((HmacContext *)context->checkContext, settings->authHashAlgo,
                        settings->authKey, settings->authKeyLen);
      //Is any error?
      if(error)
         cerror = CBOOT_ERROR_FAILURE;

      // Set digest length
      context->imageCheckDigestSize = ((HmacContext *)context->checkContext)->hash->digestSize;

      // Set check data (authentification tag) size
      context->checkDataSize = context->imageCheckDigestSize;
   }
   else
   {
      // Debug message
      TRACE_ERROR("Authentification algorithm not supported!\r\n");
      cerror = CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   // Return status code
   return cerror;
}

/**
 * @brief Update authentification tag computation on received firmware data bloc.
 * @param[in,out] context Pointer to the verification context
 * @param[in] data Firmware data bloc to process.
 * @param[in] length Length of the firmware data bloc to process.
 * @return Error code
 **/

cboot_error_t authUpdateTag(VerifyContext *context, const uint8_t *data, size_t length)
{
   cboot_error_t cerror;
   VerifySettings *settings;

   // Check parameter validity
   if (context == NULL || data == NULL || length == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Initialize status code
   cerror = CBOOT_NO_ERROR;

   // Check authentification hash algo
   if (settings->authAlgo == VERIFY_AUTH_NONE)
      return CBOOT_ERROR_INVALID_IMAGE_VERIFY_METHOD;

   // Is hmac authentification algo?
   if (settings->authAlgo == VERIFY_AUTH_HMAC)
   {
      // Update authentification hmac tag
      hmacUpdate((HmacContext *)context->checkContext, data, length);
   }
   else
   {
      // Debug message
      TRACE_ERROR("Authentification algorithm not supported!\r\n");
      cerror = CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   // Return status code
   return cerror;
}

/**
 * @brief Finalize authentification tag computation on received firmware data.
 * @param[in,out] context Pointer to the verification context
 * @return Error code
 **/

cboot_error_t authFinalizeTag(VerifyContext *context)
{
   cboot_error_t cerror;
   VerifySettings *settings;

   // Check parameter validity
   if (context == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   // Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   // Initialize status code
   cerror = CBOOT_NO_ERROR;

   // Check authentification hash algo
   if (settings->authAlgo == VERIFY_AUTH_NONE)
      return CBOOT_ERROR_INVALID_IMAGE_VERIFY_METHOD;

   // Is hmac authentification algo?
   if (settings->authAlgo == VERIFY_AUTH_HMAC)
   {
      // Compute final authentification hmac tag
      hmacFinal((HmacContext *)context->checkContext, context->imageCheckDigest);
   }
   else
   {
      // Debug message
      TRACE_ERROR("Authentification algorithm not supported!\r\n");
      cerror = CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   // Return status code
   return cerror;
}

#endif
