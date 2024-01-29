/**
 * @file verify.c
 * @brief CycloneBOOT image data verification module
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

// Dependencies
#include "security/verify.h"
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
#include "security/verify_auth.h"
#endif
#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)
#include "security/verify_sign.h"
#endif


/**
 * @brief Initialize image verification context
 * @param[in] context Pointer to the image verification context to initialize
 * @param[in] settings Image Verification specific settings
 * @return
 **/

cboot_error_t verifyInit(VerifyContext *context, VerifySettings *settings)
{
   //Check parameters
   if (context == NULL || settings == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Save verify settings
   memcpy(&context->verifySettings, settings, sizeof(VerifySettings));

   //Save verification module user settings
   memcpy(&context->verifySettings, settings, sizeof(VerifySettings));

   //Initialize verification context for integrity method
   if(context->verifySettings.verifyMethod == VERIFY_METHOD_INTEGRITY)
   {
#if (VERIFY_INTEGRITY_SUPPORT == ENABLED)
      //Is user require integrity check?
      if (context->verifySettings.integrityAlgo != NULL)
      {
         //Initialize integrity algo context
         context->verifySettings.integrityAlgo->init(context->checkContext);

         //Set image check digest size
         context->imageCheckDigestSize = context->verifySettings.integrityAlgo->digestSize;

         //Set check data (integrity tag) size
         context->checkDataSize = context->imageCheckDigestSize;

         //Successful process
         return CBOOT_NO_ERROR;
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   //Initialize verification context for authentication method
   else if(context->verifySettings.verifyMethod == VERIFY_METHOD_AUTHENTICATION)
   {
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
      //Initialize authentication algorithm
      return authInit(context);
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   //Initialize verification context for signature method
   else if(context->verifySettings.verifyMethod == VERIFY_METHOD_SIGNATURE)
   {
#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)
      //Initialize signature algorithm
      return signInit(context);
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      // Debug message
      TRACE_ERROR("Verification mode not supported!\r\n");
      return CBOOT_ERROR_NOT_IMPLEMENTED;
   }
}


/**
 * @brief Process image data verification by updating integrity,
 * authentication or signature tag with a portion of the image data.
 * It basically constist to update hash tag on image data block.
 * Once the image data fully processed the final hash tag will be used to check
 * image validy according the verification method.
 * @param[in] context Pointer to the image verification context
 * @param[in] data Pointer to the image data block to process
 * @param[in] length Length og the image data block
 * @return Error code
 **/

cboot_error_t verifyProcess(VerifyContext *context, uint8_t *data, size_t length)
{
   //Check parameters
   if (context == NULL || (data == NULL || length == 0))
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Process image data block integrity hash tag calculation
   if(context->verifySettings.verifyMethod == VERIFY_METHOD_INTEGRITY)
   {
#if (VERIFY_INTEGRITY_SUPPORT == ENABLED)
      //Is user require integrity check?
      if (context->verifySettings.integrityAlgo != NULL)
      {
         //Update integrity hash tag
         context->verifySettings.integrityAlgo->update(context->checkContext, data, length);
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   //Process image data block authentication hash tag calculation
   else if(context->verifySettings.verifyMethod == VERIFY_METHOD_AUTHENTICATION)
   {
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
      //Update authentication tag
      return authUpdateTag(context, data, length);
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   //Process image data block signature hash tag calculation
   else if(context->verifySettings.verifyMethod == VERIFY_METHOD_SIGNATURE)
   {
#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)
      //Is user require signature check?
      if (context->verifySettings.signAlgo != VERIFY_SIGN_NONE)
      {
         //Update signature hash tag
         context->verifySettings.signHashAlgo->update(context->checkContext, data, length);
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      // Debug message
      TRACE_ERROR("Verification mode not supported!\r\n");
      return CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   //Successful process
   return CBOOT_NO_ERROR;
}


/**
 * @brief Validate image check data tag using the given verification method.
 * For integrity check in consist on a comparison between image check integrity
 * hash tag and the computed image integrity hash tag.
 * For integrity check in consist on a comparison between image check authentication
 * tag and the computed image authentication tag.
 * For the signature method is use both teh public signature key and the computed hash tag
 * to check the image check signature tag.
 * @param[in,out] context Pointer to the image verification context
 * @param[in] verifyData Image check data tag to be verified
 * @param[in] verifyDataLength Length of the image check data
 * @return Error code
 **/

cboot_error_t verifyConfirm(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength)
{
   cboot_error_t cerror;

   //Initialize error code
   cerror = CBOOT_NO_ERROR;

   //Check parameters
   if (context == NULL || verifyData == NULL || verifyDataLength == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check image check data using integrity method
   if(context->verifySettings.verifyMethod == VERIFY_METHOD_INTEGRITY)
   {
#if (VERIFY_INTEGRITY_SUPPORT == ENABLED)
      //Is user require integrity check?
      if (context->verifySettings.integrityAlgo != NULL)
      {
         //Finalize interity tag computation
         context->verifySettings.integrityAlgo->final(context->checkContext, context->imageCheckDigest);

         //Debug message
         TRACE_DEBUG("\r\n");
         TRACE_DEBUG("Checking firmware integrity...\r\n");
         TRACE_DEBUG("Received integrity tag (%d bytes):\r\n", verifyDataLength);
         TRACE_DEBUG_ARRAY("  ", verifyData, verifyDataLength);
         TRACE_DEBUG("Computed integrity tag (%d bytes)\r\n", context->imageCheckDigestSize);
         TRACE_DEBUG_ARRAY("  ", context->imageCheckDigest, context->imageCheckDigestSize);
         TRACE_DEBUG("\r\n");

         //Compare computed integrity digest with the one received in the image
         if (osMemcmp(verifyData, context->imageCheckDigest, context->imageCheckDigestSize) != 0)
         {
            //Debug message
            TRACE_INFO("Firmware integrity tag is not valid!\r\n");
            cerror = CBOOT_ERROR_ABORTED;
         }
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   //Check image check data using authentification method
   else if(context->verifySettings.verifyMethod == VERIFY_METHOD_AUTHENTICATION)
   {
#if (VERIFY_AUTHENTICATION_SUPPORT == ENABLED)
      //Is user require authentification check?
      if(context->verifySettings.authAlgo != VERIFY_AUTH_NONE)
      {
         //Compute final authentification tag
         cerror = authFinalizeTag(context);
         //Is any error?
         if (cerror)
         {
            return CBOOT_ERROR_FAILURE;
         }

         //Debug message
         TRACE_DEBUG("\r\n");
         TRACE_DEBUG("Checking firmware authentification...\r\n");
         TRACE_DEBUG("Received authentification tag (%d bytes):\r\n", verifyDataLength);
         TRACE_DEBUG_ARRAY("  ", verifyData, verifyDataLength);
         TRACE_DEBUG("Computed authentification tag (%d bytes)\r\n", context->imageCheckDigestSize);
         TRACE_DEBUG_ARRAY("  ", context->imageCheckDigest, context->imageCheckDigestSize);
         TRACE_DEBUG("\r\n");

         //Compare computed integrity digest with the one received in the image
         if (osMemcmp(verifyData, context->imageCheckDigest, context->imageCheckDigestSize) != 0)
         {
            //Debug message
            TRACE_INFO("Firmware authentification tag is not valid!\r\n");
            cerror = CBOOT_ERROR_ABORTED;
         }
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   //Check image check data using signature method
   else if(context->verifySettings.verifyMethod == VERIFY_METHOD_SIGNATURE)
   {
#if (VERIFY_SIGNATURE_SUPPORT == ENABLED)
      //Is user require signature check?
      if (context->verifySettings.signAlgo != VERIFY_SIGN_NONE)
      {
         //Compute final integrity hash tag needed to check signature
         context->verifySettings.signHashAlgo->final(context->checkContext, context->imageCheckDigest);

         //Debug message
         TRACE_DEBUG("\r\n");
         TRACE_DEBUG("Checking firmware signature...\r\n");
         TRACE_DEBUG("Received signature (%d bytes):\r\n", verifyDataLength);
         TRACE_DEBUG_ARRAY("  ", verifyData, verifyDataLength);
         TRACE_DEBUG("Computed signature hash tag (%d bytes)\r\n", context->imageCheckDigestSize);
         TRACE_DEBUG_ARRAY("  ", context->imageCheckDigest, context->imageCheckDigestSize);
         TRACE_DEBUG("\r\n");

         //Verify receive signature
         cerror = signVerify(context, verifyData, verifyDataLength);
         //Is any error?
         if (cerror)
         {
            //Debug message
            TRACE_INFO("Firmware signature is not valid!\r\n");
         }
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      // Debug message
      TRACE_ERROR("Verification mode not supported!\r\n");
      return CBOOT_ERROR_NOT_IMPLEMENTED;
   }

   //Successful process
   return cerror;
}


/**
 * @brief Generate image check data according to given verification method.
 * @param[in,out] context Pointer to the image verification context
 * @param[in] checkData Buffer that will hold image check data
 * @param[in] checkDataSize Size of the image check data buffer
 * @param[out] checkDataLength Length of the image check data buffer tag
 * @return Error code
 **/

cboot_error_t verifyGenerateCheckData(VerifyContext *context, uint8_t *checkData, size_t checkDataSize, size_t *checkDataLength)
{
   VerifySettings *settings;

   //Check parameters
   if (context == NULL || checkData == NULL || checkDataLength == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Point to the verify settings
   settings = (VerifySettings *)&context->verifySettings;

   //Generate image check integrity tag
   if(context->verifySettings.verifyMethod == VERIFY_METHOD_INTEGRITY)
   {
#if (VERIFY_INTEGRITY_SUPPORT == ENABLED)
      //Is user require integrity check?
      if (context->verifySettings.integrityAlgo != NULL)
      {
         //Finalize interity tag check computation
         settings->integrityAlgo->final(context->checkContext, context->imageCheckDigest);

         if(checkDataSize >= context->imageCheckDigestSize)
         {
            //Save interity tag check data length
            *checkDataLength = context->imageCheckDigestSize;
            //Save interity tag check data
            memcpy(checkData, context->imageCheckDigest, context->imageCheckDigestSize);
         }
         else
         {
            return CBOOT_ERROR_BUFFER_OVERFLOW;
         }
      }
      else
      {
         return CBOOT_ERROR_INVALID_PARAMETERS;
      }
#else
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      //Debug message
      TRACE_ERROR("Check data generation method missing or unsupported!!!\r\n");
      return CBOOT_ERROR_ABORTED;
   }

   //Successfull process
   return CBOOT_NO_ERROR;
}
