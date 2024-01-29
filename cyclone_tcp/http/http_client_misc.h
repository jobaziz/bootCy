/**
 * @file http_client_misc.h
 * @brief Helper functions for HTTP client
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

#ifndef _HTTP_CLIENT_MISC_H
#define _HTTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "http/http_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//HTTP client related functions
void httpClientChangeState(HttpClientContext *context,
   HttpClientState newState);

void httpClientChangeRequestState(HttpClientContext *context,
   HttpRequestState newState);

error_t httpClientFormatRequestHeader(HttpClientContext *context);

error_t httpClientFormatChunkSize(HttpClientContext *context, size_t length);

error_t httpClientParseStatusLine(HttpClientContext *context, char_t *line,
   size_t length);

error_t httpClientParseHeaderField(HttpClientContext *context, char_t *line,
   size_t length);

error_t httpClientParseConnectionField(HttpClientContext *context,
   const char_t *value);

error_t httpClientParseTransferEncodingField(HttpClientContext *context,
   const char_t *value);

error_t httpClientParseContentLengthField(HttpClientContext *context,
   const char_t *value);

error_t httpClientParseChunkSize(HttpClientContext *context, char_t *line,
   size_t length);

error_t httpClientCheckTimeout(HttpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
