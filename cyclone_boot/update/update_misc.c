/**
 * @file update_misc.c
 * @brief CycloneBOOT IAP Miscellaneous Functions
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include <stdlib.h>
//#include "boot_config.h"
#include "core/flash.h"
#include "memory/memory.h"
#include "update/update.h"
#if ((CIPHER_SUPPORT == ENABLED) && ((IMAGE_INPUT_ENCRYPTED == ENABLED) || (IMAGE_OUTPUT_ENCRYPTED == ENABLED)))
#include "security/cipher.h"
#endif
#include "update/update_misc.h"
#include "core/crc32.h"
#include "debug.h"


/**
 * @brief Initialize input (update) image settings.
 * It will setup all settings needed to process the input image,
 * especially the cryptographic settings.
 * @param[in,out] settings Pointer to the user IAP settings.
 * @param[in,out] context Pointer to the IAP context.
 * @return Error code
 **/

cboot_error_t updateInitInputImage(UpdateSettings *settings, UpdateContext *context)
{
   cboot_error_t cerror;
   Image *imageIn;

   //Initialize status code
   cerror = CBOOT_NO_ERROR;

   //Check parameters validity
   if(context == NULL || settings == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Check image verification method settings
   if(settings->imageInCrypto.verifySettings.verifyMethod == VERIFY_METHOD_INTEGRITY)
   {
#if VERIFY_INTEGRITY_SUPPORT == ENABLED
      //Check image integrity verification settings
      if(settings->imageInCrypto.verifySettings.integrityAlgo == NULL)
         return CBOOT_ERROR_INVALID_PARAMETERS;
#else
      //Image integrity verification support is not activated
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else if(settings->imageInCrypto.verifySettings.verifyMethod == VERIFY_METHOD_AUTHENTICATION)
   {
#if VERIFY_AUTHENTICATION_SUPPORT == ENABLED
      //Check image authentication verification settings
      if(settings->imageInCrypto.verifySettings.authAlgo == VERIFY_AUTH_NONE ||
         settings->imageInCrypto.verifySettings.authHashAlgo == NULL ||
         settings->imageInCrypto.verifySettings.authKey == NULL ||
         settings->imageInCrypto.verifySettings.authKeyLen == 0)
         return CBOOT_ERROR_INVALID_PARAMETERS;

      //Force authentication algorythm to HMAC
      if(settings->imageInCrypto.verifySettings.authAlgo != VERIFY_AUTH_HMAC)
         return CBOOT_ERROR_UNSUPPORTED_AUTH_ALGO;
#else
      //Image authentication verification support is not activated
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else if(settings->imageInCrypto.verifySettings.verifyMethod == VERIFY_METHOD_SIGNATURE)
   {
#if VERIFY_SIGNATURE_SUPPORT == ENABLED
      //Check image signature verification settings
      if(settings->imageInCrypto.verifySettings.signAlgo == VERIFY_SIGN_NONE ||
         settings->imageInCrypto.verifySettings.signHashAlgo == NULL ||
         settings->imageInCrypto.verifySettings.signKey == NULL ||
         settings->imageInCrypto.verifySettings.signKeyLen == 0)
         return CBOOT_ERROR_INVALID_PARAMETERS;

      //Check signature algorithm is supported
      if(settings->imageInCrypto.verifySettings.signAlgo != VERIFY_SIGN_RSA &&
         settings->imageInCrypto.verifySettings.signAlgo != VERIFY_SIGN_ECDSA)
         return CBOOT_ERROR_UNSUPPORTED_SIGNATURE_ALGO;
#else
      //Image signature verification support is not activated
      return CBOOT_ERROR_INVALID_PARAMETERS;
#endif
   }
   else
   {
      //Debug message
      TRACE_ERROR("Image verification mode not supported!\r\n");
      return CBOOT_ERROR_INVALID_PARAMETERS;
   }

#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_INPUT_ENCRYPTED == ENABLED))
   //Check encryption settings
   if(settings->imageInCrypto.cipherAlgo == NULL || settings->imageInCrypto.cipherMode == CIPHER_MODE_NULL ||
       settings->imageInCrypto.cipherKey == NULL || settings->imageInCrypto.cipherKeyLen == 0)
      return CBOOT_ERROR_INVALID_PARAMETERS;
   //Force cipher algo to AES
   if(settings->imageInCrypto.cipherAlgo != AES_CIPHER_ALGO)
      return CBOOT_ERROR_UNSUPPORTED_CIPHER_ALGO;
   //Force cipher mode to CBC
   if(settings->imageInCrypto.cipherMode != CIPHER_MODE_CBC)
      return CBOOT_ERROR_UNSUPPORTED_CIPHER_MODE;
#endif

   //Point to the input image context
   imageIn = &context->imageProcessCtx.inputImage;

   //Clear input image context
   memset(imageIn, 0x00, sizeof(Image));

   //Initialize buffer position to buffer start address
   imageIn->bufferPos = imageIn->buffer;
   //Initialize check data position to check data start address
   imageIn->checkDataPos = imageIn->checkData;

   //Initialize image input process state
   imageIn->state = IMAGE_STATE_RECV_APP_HEADER;

#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_INPUT_ENCRYPTED == ENABLED))
   //Is application encrypted?
   if (settings->imageInCrypto.cipherAlgo != NULL)
   {
      //Initialize cipher engine
      cerror = cipherInit(&imageIn->cipherEngine, settings->imageInCrypto.cipherAlgo,
         settings->imageInCrypto.cipherMode, settings->imageInCrypto.cipherKey,
         settings->imageInCrypto.cipherKeyLen);

      //Is any error?
      if (cerror)
         return cerror;
   }
#endif

   //Initialize verification module
   cerror = verifyInit(&imageIn->verifyContext, &settings->imageInCrypto.verifySettings);
   //Is any error?
   if (cerror)
      return cerror;

   //Get expecting image check data size
   imageIn->checkDataSize = imageIn->verifyContext.checkDataSize;

   //Successful process
   return CBOOT_NO_ERROR;
}


/**
 * @brief Initialize Output Image settings.
 * It will setup all settings needed to generate the output image.
 * - If CycloneBOOT IAP is configured in DUal bank mode then the output message
 *   will be the new application firmware binary that was contained inside the input (update) image.
 * - Else the output image will be:
 *    - an image with encrypted application binary and simple crc32 check (if encryption on external memory is activated)
 *    - or a simple image with non encrypted application binary and simple crc32
 * @param[in,out] settings Pointer to the user IAP settings.
 * @param[in,out] context Pointer to the IAP context.
 * @return Error code
 **/

cboot_error_t updateInitOutputImage(UpdateSettings *settings, UpdateContext *context)
{
   cboot_error_t cerror;
   Image *imageOut;
   VerifySettings verifySettings;

   //Initialize status code
   cerror = CBOOT_NO_ERROR;

   //Initialize verify settings structure
   memset(&verifySettings, 0, sizeof(VerifySettings));

   //Check parameters validly
   if(context == NULL || settings == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Point to the output image context
   imageOut = &context->imageProcessCtx.outputImage;

   //Initialize buffer position to buffer start address
   imageOut->bufferPos = imageOut->buffer;

   //Initialize image output process state
#if (UPDATE_SINGLE_BANK_SUPPORT == ENABLED)
   imageOut->state = IMAGE_STATE_WRITE_APP_HEADER;
#else
   imageOut->state = IMAGE_STATE_WRITE_APP_INIT;
#endif

#if (UPDATE_SINGLE_BANK_SUPPORT == ENABLED)
#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_OUTPUT_ENCRYPTED == ENABLED))
   //Set image output data cipher key (PSK)
   if(context->settings.pskSize == 0 || context->settings.psk == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Initialize cipher engine
   cerror = cipherInit(&imageOut->cipherEngine, AES_CIPHER_ALGO,
      CIPHER_MODE_CBC, context->settings.psk,
      context->settings.pskSize);
   //Is any error?
   if (cerror)
      return cerror;

   //Set cipher engine iv
   cerror = cipherSetIv(&imageOut->cipherEngine, (uint8_t*)"1122334455667788",
      imageOut->cipherEngine.ivLen);
   //Is any error?
   if (cerror)
      return cerror;
#endif

   //Initialize check data position to check data start address
   imageOut->checkDataPos = imageOut->checkData;

   //Set verification module settings
   verifySettings.verifyMethod = VERIFY_METHOD_INTEGRITY;
   verifySettings.integrityAlgo = CRC32_HASH_ALGO;

   //Initialize verification module
   cerror = verifyInit(&imageOut->verifyContext, &verifySettings);
   //Is any error ?
   if (cerror)
      return cerror;
#endif

   //Return status code
   return cerror;
}


/**
 * @brief This function retrieves the header of the image contained in the given flash memory slot.
 * The given slot can be an primary (internal) or secondary (external) flash slot.
 * @param[in] slot Pointer to given slot that contained the image header to be extracted.
 * @param[out] header Pointer to store the extracted image header.
 * @return Error code.
 **/

cboot_error_t updateGetImageHeaderFromSlot(Slot *slot, ImageHeader *header)
{

   cboot_error_t cerror;
   ImageHeader *imgHeader;
   uint8_t buffer[sizeof(ImageHeader)];

   //Check parameter validity
   if(slot == NULL || header == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   ////Point to the slot memory driver
   ////Read first slot 64 bytes
   cerror = memoryReadSlot(slot, 0, buffer, sizeof(ImageHeader));
   //Is any error?
   if(cerror)
      return cerror;

   //Get image header from above buffer
   cerror = imageGetHeader(buffer, sizeof(ImageHeader), &imgHeader);
   //Is any error?
   if(cerror)
      return CBOOT_ERROR_INVALID_IMAGE_HEADER;

   //Save image header
   memcpy(header, imgHeader, sizeof(ImageHeader));

   //Successful process
   return CBOOT_NO_ERROR;
}
