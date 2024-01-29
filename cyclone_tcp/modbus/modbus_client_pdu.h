/**
 * @file modbus_client_pdu.h
 * @brief Modbus PDU formatting and parsing
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

#ifndef _MODBUS_CLIENT_PDU_H
#define _MODBUS_CLIENT_PDU_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP client related functions
error_t modbusClientFormatReadCoilsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatReadDiscreteInputsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatReadHoldingRegsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatReadInputRegsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatWriteSingleCoilReq(ModbusClientContext *context,
   uint16_t address, bool_t value);

error_t modbusClientFormatWriteSingleRegReq(ModbusClientContext *context,
   uint16_t address, uint16_t value);

error_t modbusClientFormatWriteMultipleCoilsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity, const uint8_t *value);

error_t modbusClientFormatWriteMultipleRegsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity, const uint16_t *value);

error_t modbusClientFormatMaskWriteRegReq(ModbusClientContext *context,
   uint16_t address, uint16_t andMask, uint16_t orMask);

error_t modbusClientFormatReadWriteMultipleRegsReq(ModbusClientContext *context,
   uint16_t readAddress, uint16_t readQuantity, uint16_t writeAddress,
   uint16_t writeQuantity, const uint16_t *writeValue);

error_t modbusClientParseReadCoilsResp(ModbusClientContext *context,
   uint_t quantity, uint8_t *value);

error_t modbusClientParseReadDiscreteInputsResp(ModbusClientContext *context,
   uint_t quantity, uint8_t *value);

error_t modbusClientParseReadHoldingRegsResp(ModbusClientContext *context,
   uint_t quantity, uint16_t *value);

error_t modbusClientParseReadInputRegsResp(ModbusClientContext *context,
   uint_t quantity, uint16_t *value);

error_t modbusClientParseWriteSingleCoilResp(ModbusClientContext *context,
   uint16_t address, bool_t value);

error_t modbusClientParseWriteSingleRegResp(ModbusClientContext *context,
   uint16_t address, uint16_t value);

error_t modbusClientParseWriteMultipleCoilsResp(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientParseWriteMultipleRegsResp(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientParseMaskWriteRegResp(ModbusClientContext *context,
   uint16_t address, uint16_t andMask, uint16_t orMask);

error_t modbusClientParseReadWriteMultipleRegsResp(ModbusClientContext *context,
   uint_t readQuantity, uint16_t *readValue);

error_t modbusClientParseExceptionResp(ModbusClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
