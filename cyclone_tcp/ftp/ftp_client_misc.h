/**
 * @file ftp_client_misc.h
 * @brief Helper functions for FTP client
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

#ifndef _FTP_CLIENT_MISC_H
#define _FTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP client related functions
void ftpClientChangeState(FtpClientContext *context, FtpClientState newState);

error_t ftpClientSendCommand(FtpClientContext *context);

error_t ftpClientFormatCommand(FtpClientContext *context,
   const char_t *command, const char_t *argument);

error_t ftpClientFormatPortCommand(FtpClientContext *context,
   const IpAddr *ipAddr, uint16_t port);

error_t ftpClientFormatPasvCommand(FtpClientContext *context);
error_t ftpClientParsePasvReply(FtpClientContext *context, uint16_t *port);

error_t ftpClientParsePwdReply(FtpClientContext *context, char_t *path,
   size_t maxLen);

error_t ftpClientParseDirEntry(char_t *line, FtpDirEntry *dirEntry);

error_t ftpClientInitDataTransfer(FtpClientContext *context, bool_t direction);
error_t ftpClientTerminateDataTransfer(FtpClientContext *context);

error_t ftpClientCheckTimeout(FtpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
