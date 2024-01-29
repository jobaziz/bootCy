/**
 * @file modbus_server_misc.h
 * @brief Helper functions for Modbus/TCP server
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

#ifndef _MODBUS_SERVER_MISC_H
#define _MODBUS_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP server related functions
void modbusServerTick(ModbusServerContext *context);

void modbusServerRegisterConnectionEvents(ModbusClientConnection *connection,
   SocketEventDesc *eventDesc);

void modbusServerProcessConnectionEvents(ModbusClientConnection *connection);

error_t modbusServerParseMbapHeader(ModbusClientConnection *connection);

error_t modbusServerFormatMbapHeader(ModbusClientConnection *connection,
   size_t length);

void *modbusServerGetRequestPdu(ModbusClientConnection *connection,
   size_t *length);

void *modbusServerGetResponsePdu(ModbusClientConnection *connection);

void modbusServerLock(ModbusClientConnection *connection);
void modbusServerUnlock(ModbusClientConnection *connection);

error_t modbusServerReadCoil(ModbusClientConnection *connection,
   uint16_t address, bool_t *state);

error_t modbusServerReadDiscreteInput(ModbusClientConnection *connection,
   uint16_t address, bool_t *state);

error_t modbusServerWriteCoil(ModbusClientConnection *connection,
   uint16_t address, bool_t state, bool_t commit);

error_t modbusServerReadHoldingReg(ModbusClientConnection *connection,
   uint16_t address, uint16_t *value);

error_t modbusServerReadInputReg(ModbusClientConnection *connection,
   uint16_t address, uint16_t *value);

error_t modbusServerWriteReg(ModbusClientConnection *connection,
   uint16_t address, uint16_t value, bool_t commit);

ModbusExceptionCode modbusServerTranslateExceptionCode(error_t status);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
