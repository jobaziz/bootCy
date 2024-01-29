/**
 * @file modbus_server_security.c
 * @brief Modbus/TCP security layer
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
#define TRACE_LEVEL MODBUS_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"
#include "modbus/modbus_server_security.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (MODBUS_SERVER_SUPPORT == ENABLED && MODBUS_SERVER_TLS_SUPPORT == ENABLED)

//Modbus Role OID (1.3.6.1.4.1.50316.802.1)
const uint8_t MODBUS_ROLE_OID[11] = {0x2B, 0x06, 0x01, 0x04, 0x01, 0x83, 0x89, 0x0C, 0x86, 0x22, 0x01};


/**
 * @brief Parse client's certificate
 * @param[in] tlsContext Pointer to the TLS context
 * @param[in] certInfo Pointer to the X.509 certificate
 * @param[in] pathLen Certificate path length
 * @param[in] param Handle referencing a Modbus/TCP client connection
 * @return Error code
 **/

error_t modbusServerParseCertificate(TlsContext *tlsContext,
   const X509CertInfo *certInfo, uint_t pathLen, void *param)
{
   error_t error;
   size_t n;
   size_t length;
   const uint8_t *data;
   ModbusClientConnection *connection;
   X509Extension extension;

   //Point to the client connection
   connection = (ModbusClientConnection *) param;

   //End-user certificate?
   if(pathLen == 0)
   {
      //The X.509 v3 certificate format also allows communities to define
      //private extensions to carry information unique to those communities
      data = certInfo->tbsCert.extensions.raw.value;
      length = certInfo->tbsCert.extensions.raw.length;

      //Loop through the extensions
      while(length > 0)
      {
         //Each extension includes an OID and a value
         error = x509ParseExtension(data, length, &n, &extension);
         //Any error to report?
         if(error)
            return error;

         //Role OID extension found?
         if(!oidComp(extension.oid.value, extension.oid.length, MODBUS_ROLE_OID,
            sizeof(MODBUS_ROLE_OID)))
         {
            //Extract the client role OID from the certificate
            error = modbusServerParseRoleOid(connection, extension.data.value,
               extension.data.length);
            //Any error to report?
            if(error)
               return error;
         }

         //Next extension
         data += n;
         length -= n;
      }
   }

   //Upon receipt of a certificate chain from the remote peer, the TLS end point
   //will verify each certificate signature using the next CA certificate in the
   //chain until it can verify the root of the chain
   return ERROR_UNKNOWN_CA;
}


/**
 * @brief Parse client role OID
 * @param[in] connection Pointer to the client connection
 * @param[in] data Pointer to the ASN.1 structure to parse
 * @param[in] length Length of the ASN.1 structure
 * @return Error code
 **/

error_t modbusServerParseRoleOid(ModbusClientConnection *connection,
   const uint8_t *data, size_t length)
{
   error_t error;
   Asn1Tag tag;

   //The Role extension must be a valid UTF-8 string
   error = asn1ReadTag(data, length, &tag);
   //Failed to decode ASN.1 tag?
   if(error)
      return error;

   //Enforce encoding, class and type
   error = asn1CheckTag(&tag, FALSE, ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_UTF8_STRING);
   //Invalid tag?
   if(error)
      return error;

   //Extract the client role OID from the certificate
   if(tag.length <= MODBUS_SERVER_MAX_ROLE_LEN)
   {
      //Copy client role
      osMemcpy(connection->role, tag.value, tag.length);
      //Properly terminate the string with a NULL character
      connection->role[tag.length] = '\0';
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Open secure connection
 * @param[in] context Pointer to the Modbus/TCP server context
 * @param[in] connection Pointer to the client connection
 * @return Error code
 **/

error_t modbusServerOpenSecureConnection(ModbusServerContext *context,
   ModbusClientConnection *connection)
{
   error_t error;

   //Allocate TLS context
   connection->tlsContext = tlsInit();
   //Failed to allocate TLS context?
   if(connection->tlsContext == NULL)
      return ERROR_OPEN_FAILED;

   //Select server operation mode
   error = tlsSetConnectionEnd(connection->tlsContext,
      TLS_CONNECTION_END_SERVER);
   //Any error to report?
   if(error)
      return error;

   //Bind TLS to the relevant socket
   error = tlsSetSocket(connection->tlsContext, connection->socket);
   //Any error to report?
   if(error)
      return error;

   //Set TX and RX buffer size
   error = tlsSetBufferSize(connection->tlsContext,
      MODBUS_SERVER_TLS_TX_BUFFER_SIZE, MODBUS_SERVER_TLS_RX_BUFFER_SIZE);
   //Any error to report?
   if(error)
      return error;

   //Register certificate verification callback
   error = tlsSetCertificateVerifyCallback(connection->tlsContext,
      modbusServerParseCertificate, connection);
   //Any error to report?
   if(error)
      return error;

#if (TLS_TICKET_SUPPORT == ENABLED)
   //Enable session ticket mechanism
   error = tlsEnableSessionTickets(connection->tlsContext, TRUE);
   //Any error to report?
   if(error)
      return error;

   //Register ticket encryption/decryption callbacks
   error = tlsSetTicketCallbacks(connection->tlsContext, tlsEncryptTicket,
      tlsDecryptTicket, &context->tlsTicketContext);
   //Any error to report?
   if(error)
      return error;
#endif

   //Invoke user-defined callback, if any
   if(context->settings.tlsInitCallback != NULL)
   {
      //Perform TLS related initialization
      error = context->settings.tlsInitCallback(connection,
         connection->tlsContext);
      //Any error to report?
      if(error)
         return error;
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Establish secure connection
 * @param[in] connection Pointer to the client connection
 * @return Error code
 **/

error_t modbusServerEstablishSecureConnection(ModbusClientConnection *connection)
{
   //Establish a TLS connection
   return tlsConnect(connection->tlsContext);
}

#endif
