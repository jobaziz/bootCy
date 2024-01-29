/**
 * @file modbus_debug.h
 * @brief Data logging functions for debugging purpose (Modbus/TCP)
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

#ifndef _MODBUS_DEBUG_H
#define _MODBUS_DEBUG_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP related functions
error_t modbusDumpRequestPdu(const void *pdu, size_t length);
error_t modbusDumpResponsePdu(const void *pdu, size_t length);

error_t modbusDumpReadCoilsReq(const ModbusReadCoilsReq *request,
   size_t length);

error_t modbusDumpReadCoilsResp(const ModbusReadCoilsResp *response,
   size_t length);

error_t modbusDumpReadDiscreteInputsReq(const ModbusReadDiscreteInputsReq *request,
   size_t length);

error_t modbusDumpReadDiscreteInputsResp(const ModbusReadDiscreteInputsResp *response,
   size_t length);

error_t modbusDumpReadHoldingRegsReq(const ModbusReadHoldingRegsReq *request,
   size_t length);

error_t modbusDumpReadHoldingRegsResp(const ModbusReadHoldingRegsResp *response,
   size_t length);

error_t modbusDumpReadInputRegsReq(const ModbusReadInputRegsReq *request,
   size_t length);

error_t modbusDumpReadInputRegsResp(const ModbusReadInputRegsResp *response,
   size_t length);

error_t modbusDumpWriteSingleCoilReq(const ModbusWriteSingleCoilReq *request,
   size_t length);

error_t modbusDumpWriteSingleCoilResp(const ModbusWriteSingleCoilResp *response,
   size_t length);

error_t modbusDumpWriteSingleRegReq(const ModbusWriteSingleRegReq *request,
   size_t length);

error_t modbusDumpWriteSingleRegResp(const ModbusWriteSingleRegResp *response,
   size_t length);

error_t modbusDumpWriteMultipleCoilsReq(const ModbusWriteMultipleCoilsReq *request,
   size_t length);

error_t modbusDumpWriteMultipleCoilsResp(const ModbusWriteMultipleCoilsResp *response,
   size_t length);

error_t modbusDumpWriteMultipleRegsReq(const ModbusWriteMultipleRegsReq *request,
   size_t length);

error_t modbusDumpWriteMultipleRegsResp(const ModbusWriteMultipleRegsResp *response,
   size_t length);

error_t modbusDumpMaskWriteRegReq(const ModbusMaskWriteRegReq *request,
   size_t length);

error_t modbusDumpMaskWriteRegResp(const ModbusMaskWriteRegResp *response,
   size_t length);

error_t modbusDumpReadWriteMultipleRegsReq(const ModbusReadWriteMultipleRegsReq *request,
   size_t length);

error_t modbusDumpReadWriteMultipleRegsResp(const ModbusReadWriteMultipleRegsResp *response,
   size_t length);

error_t modbusDumpExceptionResp(const ModbusExceptionResp *response,
   size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
