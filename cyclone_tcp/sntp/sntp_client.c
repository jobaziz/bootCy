/**
 * @file sntp_client.c
 * @brief SNTP client (Simple Network Time Protocol)
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
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
#define TRACE_LEVEL SNTP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "sntp/sntp_client.h"
#include "sntp/sntp_client_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNTP_CLIENT_SUPPORT == ENABLED)


/**
 * @brief Initialize SNTP client context
 * @param[in] context Pointer to the SNTP client context
 * @return Error code
 **/

error_t sntpClientInit(SntpClientContext *context)
{
   //Make sure the SNTP client context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Clear SNTP client context
   osMemset(context, 0, sizeof(SntpClientContext));

   //Initialize SNTP client state
   context->state = SNTP_CLIENT_STATE_INIT;

   //Default timeout
   context->timeout = SNTP_CLIENT_DEFAULT_TIMEOUT;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Set communication timeout
 * @param[in] context Pointer to the SNTP client context
 * @param[in] timeout Timeout value, in milliseconds
 * @return Error code
 **/

error_t sntpClientSetTimeout(SntpClientContext *context, systime_t timeout)
{
   //Make sure the SNTP client context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Save timeout value
   context->timeout = timeout;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Bind the SNTP client to a particular network interface
 * @param[in] context Pointer to the SNTP client context
 * @param[in] interface Network interface to be used
 * @return Error code
 **/

error_t sntpClientBindToInterface(SntpClientContext *context,
   NetInterface *interface)
{
   //Make sure the SNTP client context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Explicitly associate the SNTP client with the specified interface
   context->interface = interface;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Specify the IP address of the NTP server
 * @param[in] context Pointer to the SNTP client context
 * @param[in] serverIpAddr IP address of the NTP server
 * @param[in] serverPort Port number
 * @return Error code
 **/

error_t sntpClientSetServerAddr(SntpClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort)
{
   //Check parameters
   if(context == NULL || serverIpAddr == NULL)
      return ERROR_INVALID_PARAMETER;

   //Save the IP address and the port number of the NTP server
   context->serverIpAddr = *serverIpAddr;
   context->serverPort = serverPort;

   //Close UDP socket
   sntpClientCloseConnection(context);
   //Revert to default state
   context->state = SNTP_CLIENT_STATE_INIT;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Retrieve current time from NTP server
 * @param[in] context Pointer to the SNTP client context
 * @param[out] timestamp Pointer to the NTP timestamp
 * @return Error code
 **/

error_t sntpClientGetTimestamp(SntpClientContext *context,
   NtpTimestamp *timestamp)
{
   error_t error;

   //Check parameters
   if(context == NULL || timestamp == NULL)
      return ERROR_INVALID_PARAMETER;

   //Initialize status code
   error = NO_ERROR;

   //Send NTP request and wait for server's response
   while(!error)
   {
      //Check current state
      if(context->state == SNTP_CLIENT_STATE_INIT)
      {
         //Open UDP socket
         error = sntpClientOpenConnection(context);

         //Check status code
         if(!error)
         {
            //Save current time
            context->startTime = osGetSystemTime();
            //Initialize retransmission timeout
            context->retransmitTimeout = SNTP_CLIENT_INIT_RETRANSMIT_TIMEOUT;

            //Send the request to the designated NTP server
            context->state = SNTP_CLIENT_STATE_SENDING;
         }
      }
      else if(context->state == SNTP_CLIENT_STATE_SENDING)
      {
         //Send the request to the designated NTP server
         error = sntpClientSendRequest(context);
      }
      else if(context->state == SNTP_CLIENT_STATE_RECEIVING)
      {
         //Wait for server's response
         error = sntpClientReceiveResponse(context);
      }
      else if(context->state == SNTP_CLIENT_STATE_COMPLETE)
      {
         //Extract NTP timestamp from server's response
         error = sntpClientParseResponse(context, timestamp);

         //Revert to default state
         context->state = SNTP_CLIENT_STATE_INIT;
         break;
      }
      else
      {
         //Invalid state
         error = ERROR_WRONG_STATE;
      }
   }

   //Check status code
   if(error != ERROR_WOULD_BLOCK)
   {
      //Close UDP socket
      sntpClientCloseConnection(context);
      //Revert to default state
      context->state = SNTP_CLIENT_STATE_INIT;
   }

   //Return status code
   return error;
}


/**
 * @brief Retrieve the kiss code from a Kiss-of-Death message
 * @param[in] context Pointer to the SNTP client context
 * @return Kiss code
 **/

uint32_t sntpClientGetKissCode(SntpClientContext *context)
{
   uint32_t kissCode;

   //Make sure the SNTP client context is valid
   if(context != NULL)
   {
      //Get kiss code
      kissCode = context->kissCode;
   }
   else
   {
      //The SNTP client context is not valid
      kissCode = 0;
   }

   //Return kiss code
   return kissCode;
}


/**
 * @brief Release SNTP client context
 * @param[in] context Pointer to the SNTP client context
 **/

void sntpClientDeinit(SntpClientContext *context)
{
   //Make sure the SNTP client context is valid
   if(context != NULL)
   {
      //Close UDP socket
      sntpClientCloseConnection(context);

      //Clear SNTP client context
      osMemset(context, 0, sizeof(SntpClientContext));
   }
}

#endif
