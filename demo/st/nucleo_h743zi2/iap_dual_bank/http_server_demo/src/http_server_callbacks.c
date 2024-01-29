/**
 * @file http_server_callbacks.c
 * @brief HTTP server callbacks handling
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

//Dependencies
#include "core/net.h"
#include "http_server_callbacks.h"
#include "update/update.h"
#include "security/verify.h"
#include "drivers/memory/flash/internal/stm32h7xx_flash_driver.h"
#include "version.h"
#include "debug.h"


//Gloabal variables
UpdateSettings updateSettings;
UpdateContext updateContext;

//Extern variables
extern OsMutex updateMutex;
extern const char_t* appVersion;

//Extern update signature public key declaration
extern const uint8_t* pemUpdtSignPublicKey;
extern size_t pemUpdtSignPublicKeyLen;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief HTTP request callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/

error_t httpServerRequestCallback(HttpConnection *connection,
   const char_t *uri)
{
   error_t error;
   cboot_error_t cerror;
   uint8_t buffer[128];
   size_t received;
   size_t n;

   //Initialize variables
   received = 0;
   error = NO_ERROR;
   cerror = CBOOT_NO_ERROR;

   //Manage application data request
   if(!strcasecmp(uri, "/data.json"))
   {
      //Format json response
      n = sprintf((char_t*) buffer,
         "{" \
            "\"data\": {" \
               "\"version\": \"%s\"," \
               "\"color\": \"%s\"," \
               "\"scenario\": \"%d\"" \
            "}" \
         "}",
         APP_VERSION_STRING,
         APP_VERSION_COLOR,
         3
      );

      //Format HTTP response header
      connection->response.version = connection->request.version;
      connection->response.statusCode = 200;
      connection->response.keepAlive = connection->request.keepAlive;
      connection->response.noCache = TRUE;
      connection->response.contentLength = n;

      //Send the header to the client
      error = httpWriteHeader(connection);
      //Any error to report?
      if(error)
         return error;

      //Send the response data to the client
      error = httpWriteStream(connection, buffer, n);
      //Any error to report?
      if(error)
         return error;

      //Properly close output stream
      error = httpCloseStream(connection);
      //Any error to report?
      if(error)
         return error;
   }
   //Manage new firmware request
   else if(!strcasecmp(uri, "/upload/binary"))
   {
      //Get exclusive access
      osAcquireMutex(&updateMutex);

      //Debug message
      TRACE_INFO("\r\n");
      TRACE_INFO("Starting firmware update...\r\n");

      //Start of exception handling block
      do
      {
         //Start of exception handling block
         do
         {
            //User update settings configuration
            //----------------------------------
#if (UPDATE_ANTI_ROLLBACK_SUPPORT == ENABLED)
            updateSettings.appVersion = APP_VERSION;
#endif
            //User update settings security configuration
            updateSettings.imageInCrypto.verifySettings.verifyMethod    = VERIFY_METHOD_SIGNATURE;
            updateSettings.imageInCrypto.verifySettings.signAlgo        = VERIFY_SIGN_RSA;
            updateSettings.imageInCrypto.verifySettings.signHashAlgo    = SHA256_HASH_ALGO;
            updateSettings.imageInCrypto.verifySettings.signKey         = (const char_t*) pemUpdtSignPublicKey;
            updateSettings.imageInCrypto.verifySettings.signKeyLen      = pemUpdtSignPublicKeyLen;
            updateSettings.imageInCrypto.cipherAlgo                     = AES_CIPHER_ALGO;
            updateSettings.imageInCrypto.cipherMode                     = CIPHER_MODE_CBC;
            updateSettings.imageInCrypto.cipherKey                      = "aa3ff7d43cc015682c7dfd00de9379e7";
            updateSettings.imageInCrypto.cipherKeyLen                   = 32;

            //User update settings primary memory configuration
            updateSettings.memories[0].memoryRole           = MEMORY_ROLE_PRIMARY;
            updateSettings.memories[0].memoryType           = MEMORY_TYPE_FLASH;
            updateSettings.memories[0].driver               = &stm32h7xxFlashDriver;
            updateSettings.memories[0].nbSlots              = 2;
            //User update settings primary memory slot 0 configuration
            updateSettings.memories[0].slots[0].type        = SLOT_TYPE_DIRECT;
            updateSettings.memories[0].slots[0].cType       = SLOT_CONTENT_APP | SLOT_CONTENT_BACKUP;
            updateSettings.memories[0].slots[0].memParent   = &updateSettings.memories[0];
            updateSettings.memories[0].slots[0].addr        = 0x08000000;
            updateSettings.memories[0].slots[0].size        = 0x100000;
            //User update settings primary memory slot 1 configuration
            updateSettings.memories[0].slots[1].type        = SLOT_TYPE_DIRECT;
            updateSettings.memories[0].slots[1].cType       = SLOT_CONTENT_APP | SLOT_CONTENT_BACKUP;
            updateSettings.memories[0].slots[1].memParent   = &updateSettings.memories[0];
            updateSettings.memories[0].slots[1].addr        = 0x08100000;
            updateSettings.memories[0].slots[1].size        = 0x100000;

            //Initialize IAP update context
            cerror = updateInit(&updateContext, &updateSettings);
            //Is any error?
            if(cerror)
            {
               //Debug message
               TRACE_ERROR("Failed to initialize IAP!\r\n");
               break;
            }

            //Debug message
            TRACE_INFO("Receiving firmware update image...\r\n");

            //Process HTTP request body
            while(1)
            {
               //Read the HTTP request body until buffer is full or end of stream
               error = httpReadStream(connection, buffer, sizeof(buffer),
                  &received, HTTP_FLAG_WAIT_ALL);
               //End of stream detected?
               if(error)
                  break;

               //Write received bytes in flash
               cerror = updateProcess(&updateContext, (uint8_t*) buffer, received);
               //Is any error?
               if(cerror != CBOOT_NO_ERROR)
               {
                  //Debug message
                  TRACE_ERROR("Failed to update firmware!\r\n");
                  break;
               }
            }

            //Propagate exception if necessary
            if(error != ERROR_END_OF_STREAM || cerror != CBOOT_NO_ERROR)
            {
               //Debug message
               TRACE_ERROR("Firmware image reception failed!\r\n");
               break;
            }
            else
               error = NO_ERROR;

            //Check application firmware validity
            cerror = updateFinalize(&updateContext);
            //Is application is invalid?
            if(cerror != CBOOT_NO_ERROR)
            {
               //Debug message
               TRACE_ERROR("Failed to finalize firmware update!\r\n");
               break;
            }

            //Start of exception handling block
            do
            {
               //Format HTTP response header
               connection->response.version = connection->request.version;
               connection->response.statusCode = 200;
               connection->response.keepAlive = connection->request.keepAlive;
               connection->response.noCache = TRUE;
               connection->response.contentLength = 0;

               //Send the header to the client
               error = httpWriteHeader(connection);
               //Any error to report?
               if(error)
               {
                  //Debug message
                  TRACE_ERROR("Sending HTTP response response failed!\r\n");
                  break;
               }

               //Properly close output stream
               error = httpCloseStream(connection);
               //Any error to report?
               if(error)
               {
                  //Debug message
                  TRACE_ERROR("Closing HTTP output stream failed!\r\n");
                  break;
               }
               //End of exception handling block
            }while(0);

            //Perform flash swap bank then reboot
            cerror = updateReboot(&updateContext);
            //Is any error?
            if(cerror != CBOOT_NO_ERROR)
            {
               //Debug message
               TRACE_ERROR("Failed to reboot!\r\n");
               break;
            }

            //End of exception handling block
         }while(0);

         //Is any error?
         if(error || cerror != CBOOT_NO_ERROR)
         {
            //Format HTTP response header
            connection->response.version = connection->request.version;
            connection->response.statusCode = 400;
            connection->response.keepAlive = connection->request.keepAlive;
            connection->response.noCache = TRUE;
            connection->response.contentLength = 0;

            //Send the header to the client
            error = httpWriteHeader(connection);
            //Any error to report?
            if(error)
            {
               //Debug message
               TRACE_ERROR("Sending HTTP response response failed!\r\n");
               break;
            }

            //Properly close output stream
            error = httpCloseStream(connection);
            //Any error to report?
            if(error)
            {
               //Debug message
               TRACE_ERROR("Closing HTTP output stream failed!\r\n");
               break;
            }
         }

         //End of exception handling block
      }while(0);

      //Release exclusive access
      osReleaseMutex(&updateMutex);

      //Return status code
      return error;
   }
   else
   {
      //For sanity
   }

   //Not implemented
   return ERROR_NOT_FOUND;
}


/**
 * @brief URI not found callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/

error_t httpServerUriNotFoundCallback(HttpConnection *connection,
   const char_t *uri)
{
   //Not implemented
   return ERROR_NOT_FOUND;
}
