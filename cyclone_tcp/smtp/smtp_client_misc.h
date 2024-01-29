/**
 * @file smtp_client_misc.h
 * @brief Helper functions for SMTP client
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

#ifndef _SMTP_CLIENT_MISC_H
#define _SMTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "smtp/smtp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SMTP client related functions
void smtpClientChangeState(SmtpClientContext *context,
   SmtpClientState newState);

error_t smtpClientSendCommand(SmtpClientContext *context,
   SmtpClientReplyCallback callback);

error_t smtpClientFormatCommand(SmtpClientContext *context,
   const char_t *command, const char_t *argument);

error_t smtpClientParseEhloReply(SmtpClientContext *context,
   char_t *replyLine);

error_t smtpClientFormatMailHeader(SmtpClientContext *context,
   const SmtpMailAddr *from, const SmtpMailAddr *recipients,
   uint_t numRecipients, const char_t *subject);

error_t smtpClientFormatMultipartHeader(SmtpClientContext *context,
   const char_t *filename, const char_t *contentType,
   const char_t *contentTransferEncoding, bool_t last);

error_t smtpClientCheckTimeout(SmtpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
