/**
 * @file modbus_server_security.h
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

#ifndef _MODBUS_SERVER_SECURITY_H
#define _MODBUS_SERVER_SECURITY_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"

//TLS supported?
#if (MODBUS_SERVER_TLS_SUPPORT == ENABLED)
   #include "core/crypto.h"
   #include "encoding/asn1.h"
   #include "encoding/oid.h"
   #include "pkix/x509_cert_parse.h"
   #include "pkix/x509_cert_ext_parse.h"
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP server related functions
error_t modbusServerParseRoleOid(ModbusClientConnection *connection,
   const uint8_t *data, size_t length);

error_t modbusServerOpenSecureConnection(ModbusServerContext *context,
   ModbusClientConnection *connection);

error_t modbusServerEstablishSecureConnection(ModbusClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
