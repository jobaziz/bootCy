/**
 * @file coap_server_request.h
 * @brief CoAP request handling
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

#ifndef _COAP_SERVER_REQUEST_H
#define _COAP_SERVER_REQUEST_H

//Dependencies
#include "core/net.h"
#include "coap/coap_server.h"
#include "coap/coap_option.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

error_t coapServerGetMethodCode(CoapServerContext *context, CoapCode *code);

error_t coapServerGetUriPath(CoapServerContext *context, char_t *path,
   size_t maxLen);

error_t coapServerGetUriQuery(CoapServerContext *context, char_t *queryString,
   size_t maxLen);

error_t coapServerGetOpaqueOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const uint8_t **optionValue, size_t *optionLen);

error_t coapServerGetStringOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const char_t **optionValue, size_t *optionLen);

error_t coapServerGetUintOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, uint32_t *optionValue);

error_t coapServerGetPayload(CoapServerContext *context, const uint8_t **payload,
   size_t *payloadLen);

error_t coapServerReadPayload(CoapServerContext *context, void *data, size_t size,
   size_t *length);

error_t coapServerSetResponseCode(CoapServerContext *context, CoapCode code);

error_t coapServerSetLocationPath(CoapServerContext *context,
   const char_t *path);

error_t coapServerSetLocationQuery(CoapServerContext *context,
   const char_t *queryString);

error_t coapServerSetOpaqueOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const uint8_t *optionValue, size_t optionLen);

error_t coapServerSetStringOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const char_t *optionValue);

error_t coapServerSetUintOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, uint32_t optionValue);

error_t coapServerDeleteOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex);

error_t coapServerSetPayload(CoapServerContext *context, const void *payload,
   size_t payloadLen);

error_t coapServerWritePayload(CoapServerContext *context, const void *data,
   size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
