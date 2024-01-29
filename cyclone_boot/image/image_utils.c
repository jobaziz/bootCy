/**
 * @file image_process.c
 * @brief CycloneBOOT Image Utils routines
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
#include "debug.h"
#include "image/image.h"
#include "memory/memory.h"
#include "image_utils.h"
#include "image_process.h"

//Image utils private function prototypes definition
bool_t imageAcceptUpdate(ImageProcessContext *context, uint32_t version);
#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_INPUT_ENCRYPTED == ENABLED))
cboot_error_t imageProcessAppCipherIv(ImageProcessContext *context);
#endif


/**
 * @brief Process image application header.
 * Once fully received it will parse the image header,
 * check its integrity and retrieve needed data.
 * @param[in,out] context Pointer to the ImageProcess context
 * @return Error code.
 **/

cboot_error_t imageProcessAppHeader(ImageProcessContext *context)
{
    cboot_error_t cerror;
    ImageHeader *imgHeader;
    Image *imageOut;
    Image *imageIn;
    size_t n;
    size_t outputSize;

    //Check parameter validity
    if (context == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Point to output image context
    imageIn = &context->inputImage;
    imageOut = &context->outputImage;

    //Check current input image process state
    if(imageIn->state != IMAGE_STATE_RECV_APP_HEADER)
        return CBOOT_ERROR_INVALID_STATE;

    //Initialize variable
    n = 0;
    imgHeader = NULL;
    outputSize = 0;

    //Is buffer full enough to contain an image header?
    if (imageIn->bufferLen >= sizeof(ImageHeader))
    {
        //Debug message
        TRACE_DEBUG("Processing firmware image header...\r\n");

        //Get input image header
        cerror = imageGetHeader(imageIn->buffer, imageIn->bufferLen, &imgHeader);
        //Is any error?
        if(cerror)
        {
            //Debug message
            TRACE_INFO("Input image header is invalid!\r\n");
            //Forward error
            return cerror;
        }

        //Check anti-rollback callback
        if(context->imgAntiRollbackCallback != NULL)
        {
            //Decide whether to perform the update or not in regards of the
            //  firmware application version inside the update image.
            if(!context->imgAntiRollbackCallback(context->currentAppVersion, imgHeader->dataVers))
            {
                //Debug message
                TRACE_INFO("Update Aborted! Incorrect update image application version.\r\n");
                //Forward error
                return CBOOT_ERROR_INCORRECT_IMAGE_APP_VERSION;
            }
        }

        //Check the header image type
        if(imgHeader->imgType != IMAGE_TYPE_APP)
        {
            //Debug message
            TRACE_ERROR("Invalid header image type!\r\n");
            return CBOOT_ERROR_INVALID_HEADER_APP_TYPE;
        }

        //Check output type
        if(imageOut->activeSlot->cType & SLOT_CONTENT_BINARY)
        {
            //Compute output binary size
            outputSize = imgHeader->dataSize;

            //Would output firmware overcome the memory slot holding it?
            if (outputSize > imageOut->activeSlot->size)
            {
                //Debug message
                TRACE_ERROR("Output binary would be bigger the memory slot holding it\r\n");
                //Forward error
                return CBOOT_ERROR_BUFFER_OVERFLOW;
            }
        }
        else
        {
            //Compute output image size
#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_OUTPUT_ENCRYPTED == ENABLED))
            outputSize = imgHeader->dataSize + sizeof(ImageHeader) +
                imageOut->cipherEngine.ivLen +
                imageOut->verifyContext.verifySettings.integrityAlgo->digestSize;
#else
            outputSize = imgHeader->dataSize + sizeof(ImageHeader) +
                imageOut->verifyContext.verifySettings.integrityAlgo->digestSize;
#endif
            //Would output image overcome the memory slot holding it?
            if (outputSize > imageOut->activeSlot->size)
            {
                //Debug message
                TRACE_ERROR("Output image would be bigger than the memory slot holding it!\r\n");
                //Forward error
                return CBOOT_ERROR_BUFFER_OVERFLOW;
            }
        }

        //Save application firmware length
        imageOut->firmwareLength = imgHeader->dataSize;
        imageIn->firmwareLength = imgHeader->dataSize;

        //Check output type
        if(!(imageOut->activeSlot->cType & SLOT_CONTENT_BINARY))
        {
            //Process parsed image input header for later output image generation
            cerror = imageProcessOutput(context, (uint8_t*)imgHeader, sizeof(ImageHeader));
            if(cerror)
                return cerror;
        }

        //Check image header integrity
        cerror = verifyProcess(&imageIn->verifyContext, (uint8_t*)&imgHeader->headCrc, CRC32_DIGEST_SIZE);
        //Is any error?
        if(cerror)
            return cerror;

        //Remove header from buffer
        n = imageIn->bufferLen - sizeof(ImageHeader);
        memcpy(imageIn->buffer, imageIn->buffer + sizeof(ImageHeader), n);
        imageIn->bufferPos -= sizeof(ImageHeader);
        imageIn->bufferLen -= sizeof(ImageHeader);

        //Change image state
        imageChangeState(imageIn, IMAGE_STATE_RECV_APP_DATA);
    }

    // Successful process
    return CBOOT_NO_ERROR;
}

/**
 * @brief Process receiving of the firmware data bloc by bloc.
 * If firmware is encrypted the data will first be deciphered then depending
 * of the user settings integrity or authentication or signature hash
 * computation will be done one each data blocs.
 * Finally each data bloc will write into the flash
 * bank.
 * @param[in,out] context Pointer to the ImageProcess context
 * @return Error code.
 **/

cboot_error_t imageProcessAppData(ImageProcessContext *context)
{
    cboot_error_t cerror;
    uint_t dataLength;
    Image *imageIn;

    //Check parameter validity
    if (context == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Point to image input context
    imageIn = &context->inputImage;

    //Check current input image process state
    if(imageIn->state != IMAGE_STATE_RECV_APP_DATA)
        return CBOOT_ERROR_INVALID_STATE;

#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_INPUT_ENCRYPTED == ENABLED))
    //Receiving image application cipher iv?
    //if(context->cipherEngine.algo != NULL && !context->ivRetrieved)
    if((imageIn->cipherEngine.algo != NULL) && !imageIn->ivRetrieved)
    {
        //Process application cipher initialisation vector here
        cerror = imageProcessAppCipherIv(context);
        //Is any error?
        if (cerror)
            return cerror;
    }
        //Receiving image firmware data?
    else
    {
#else
    //Receiving image firmware data?
    if(1)
    {
#endif
        //Is buffer full or full enough to contain last application data?
        if ((imageIn->bufferLen == sizeof(imageIn->buffer)) ||
            (imageIn->written + imageIn->bufferLen >= imageIn->firmwareLength))
        {
            //We must not process more data than the firmware length
            dataLength = MIN(imageIn->bufferLen,
                             imageIn->firmwareLength - imageIn->written);

            //Update application check computation tag (could be integrity tag or
            //authentication tag or hash signature tag)
            cerror = verifyProcess(&imageIn->verifyContext,
                                  imageIn->buffer, dataLength);

            //Is any error?
            if (cerror)
                return cerror;

#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_INPUT_ENCRYPTED == ENABLED))
            //Is application is encrypted?
            if (imageIn->cipherEngine.algo != NULL)
            {
                //Decrypt application data
                cerror = cipherDecryptData(&imageIn->cipherEngine, imageIn->buffer, dataLength);

                //Is any error?
                if (cerror)
                    return cerror;
            }
#endif

            //Process/format output data
            cerror = imageProcessOutput(context,imageIn->buffer, dataLength);
            //Is any error?
            if(cerror)
                return cerror;

            //imgOutput->written += dataLength;
            imageIn->written += dataLength;

            //Does the buffer contain remaining data?
            if (imageIn->bufferLen != dataLength)
            {
               //Discard already processed data
               memset(imageIn->buffer, 0, dataLength);

               //Put remaining data at buffer start
               memcpy(imageIn->buffer, imageIn->buffer + dataLength,
                      imageIn->bufferLen - dataLength);
               //Update buffer position and length
               imageIn->bufferPos = imageIn->buffer +
                       (imageIn->bufferLen - dataLength);
               imageIn->bufferLen = imageIn->bufferLen - dataLength;
            }
            else
            {
                //Reset buffer
                memset(imageIn->buffer, 0, sizeof(imageIn->buffer));
                imageIn->bufferPos = imageIn->buffer;
                imageIn->bufferLen = 0;
            }

            //Is application data all received?
            if (imageIn->written == imageIn->firmwareLength)
            {
                //Change Image process state
                imageChangeState(imageIn, IMAGE_STATE_RECV_APP_CHECK);

                //Still data to process?
                if (imageIn->bufferLen > 0)
                {
                    //Process Image check app data
                    cerror = imageProcessAppCheck(context);
                    //Is any error?
                    if (cerror)
                        return cerror;
                }
            }
        }
    }

    //Successful process
    return CBOOT_NO_ERROR;
}

/**
* @brief Process receiving of the image check data. Depending of the user
* settings it could be the integrity or the authentication tag or signature
* of the image firmware data. Once fully received it will saved for further
* firmware validity verification.
* @param[in,out] context Pointer to the ImageProcess context
* @return Error code.
**/

cboot_error_t imageProcessAppCheck(ImageProcessContext *context)
{
    Image *imageIn;

    //Check parameter validity
    if (context == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Point to the input image context
    imageIn = (Image*)&context->inputImage;

    //Check current input image process state
    if(imageIn->state != IMAGE_STATE_RECV_APP_CHECK)
        return CBOOT_ERROR_INVALID_STATE;

    //Is buffer full enough to contain image check data?
    //(could be integrity/authentication tag or signature)
    if (imageIn->checkDataLen + imageIn->bufferLen <= imageIn->checkDataSize)
    {
        //Save image check data block
        memcpy(imageIn->checkDataPos, imageIn->buffer,
               imageIn->bufferLen);
        imageIn->checkDataPos += imageIn->bufferLen;
        imageIn->checkDataLen += imageIn->bufferLen;

        //Reset buffer
        memset(imageIn->buffer, 0, sizeof(imageIn->buffer));
        imageIn->bufferPos = imageIn->buffer;
        imageIn->bufferLen = 0;

        //Is image check data fully received?
        if (imageIn->checkDataLen == imageIn->checkDataSize)
        {
            //Change image process state
            imageChangeState(imageIn, IMAGE_STATE_VALIDATE_APP);
        }
    }
    else
    {
        //Debug message
        TRACE_ERROR("Image check data is bigger than expected!\r\n");
        return CBOOT_ERROR_BUFFER_OVERFLOW;
    }

    //Successful process
    return CBOOT_NO_ERROR;
}

#if ((CIPHER_SUPPORT == ENABLED) && (IMAGE_INPUT_ENCRYPTED == ENABLED))

/**
 * @brief Process receiving of the firmware cipher initialization vector (iv) in
 * case the firmware has been encrypted. Once fully received it will be save for
 * later firmware dencryption.
 * @param[in,out] context Pointer to the Image process context
 * @return Error code.
 **/

cboot_error_t imageProcessAppCipherIv(ImageProcessContext *context)
{
   cboot_error_t cerror;
   Image *imageIn;
   uint_t n;

   //Check parameter validity
   if (context == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

    //Point to the input image inside image proccess context
   imageIn = (Image*)&context->inputImage;

   //Initialize variable
   n = 0;

   //Is buffer full enough to contains IAP image header?
   if (imageIn->bufferLen >= imageIn->cipherEngine.algo->blockSize)
   {
      //Debug message
      TRACE_DEBUG("Processing firmware image cipher initialization vector...\r\n");

      //Save application cipher intialization vector
      cerror = cipherSetIv(&imageIn->cipherEngine, imageIn->buffer, imageIn->cipherEngine.ivLen);
      //Is any error?
      if(cerror)
         return cerror;

      //Set cipher iv as retrieved
      imageIn->ivRetrieved = TRUE;

      //Update application check computation tag (could be integrity tag or
      //authentification tag or hash signature tag)
      cerror = verifyProcess(&imageIn->verifyContext, imageIn->cipherEngine.iv, imageIn->cipherEngine.ivLen);
      //Is any error?
      if(cerror)
         return cerror;

      //Remove processed data (cipher iv) from buffer
      n = imageIn->bufferLen - imageIn->cipherEngine.ivLen;
      memcpy(imageIn->buffer, imageIn->buffer + imageIn->cipherEngine.ivLen, n);
      imageIn->bufferPos -= imageIn->cipherEngine.ivLen;
      imageIn->bufferLen -= imageIn->cipherEngine.ivLen;
   }

   //Successful process
   return CBOOT_NO_ERROR;
}

#endif


/**
 * @brief This function checks the version of firmware application inside the received update image.
 * If the version of the firmware application in the update image is less than or equal to the version
 * of the current firmware application then the update will be discarded. If not the update image is accepted.
 * This function is used only if the anti-rollback feature is activated.
 * @param[in] context Pointer to the Image process context.
 * @param[in] version Version of the firmware application inside the update image.
 * @return TRUE if the update image is accepted, FALSE otherwise.
 **/

bool_t imageAcceptUpdate(ImageProcessContext *context, uint32_t version)
{
    //Check paramter validity
    if(context == NULL)
        return FALSE;

    //The version of the application within the received update image MUST be greater than
    // the version of current running application
    if(version > context->currentAppVersion)
        return TRUE;
    else
        return FALSE;
}


/**
 * @brief Compute image header crc and store it in the image header crc field.
 * The given input header will also be the outout image header.
 * @param[in/out] header Pointer the header on which to calculate the crc.
 * @return Error code
 **/

cboot_error_t imageComputeHeaderCrc(ImageHeader *header)
{
    error_t error;

    //Check parameters validity
    if(header == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Compute image header crc
    error = CRC32_HASH_ALGO->compute((uint8_t*)header, sizeof(ImageHeader) - CRC32_DIGEST_SIZE, (uint8_t*)&header->headCrc);
    //Is any error?
    if(error)
        return CBOOT_ERROR_FAILURE;

    //Successfull process
    return CBOOT_NO_ERROR;
}


/**
 * @brief Update Image state
 * @param[in] context Pointer to the Image context
 * @param[in] newState New state to switch to
 **/

void imageChangeState(Image *image, ImageState newState)
{
    //Update Image state
    image->state = newState;
}
