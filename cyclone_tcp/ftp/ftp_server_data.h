/**
 * @file ftp_server_data.h
 * @brief FTP data connection
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

#ifndef _FTP_SERVER_DATA_H
#define _FTP_SERVER_DATA_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP server related functions
void ftpServerRegisterDataChannelEvents(FtpClientConnection *connection,
   SocketEventDesc *eventDesc);

void ftpServerProcessDataChannelEvents(FtpClientConnection *connection,
   uint_t eventFlags);

error_t ftpServerOpenDataChannel(FtpClientConnection *connection);
void ftpServerAcceptDataChannel(FtpClientConnection *connection);
void ftpServerWriteDataChannel(FtpClientConnection *connection);
void ftpServerReadDataChannel(FtpClientConnection *connection);
void ftpServerCloseDataChannel(FtpClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
