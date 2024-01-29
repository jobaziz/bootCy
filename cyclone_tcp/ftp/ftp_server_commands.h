/**
 * @file ftp_server_commands.h
 * @brief FTP server (command processing)
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

#ifndef _FTP_SERVER_COMMANDS_H
#define _FTP_SERVER_COMMANDS_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP server related functions
void ftpServerProcessCommand(FtpClientConnection *connection);

void ftpServerProcessNoop(FtpClientConnection *connection, char_t *param);
void ftpServerProcessSyst(FtpClientConnection *connection, char_t *param);
void ftpServerProcessFeat(FtpClientConnection *connection, char_t *param);
void ftpServerProcessAuth(FtpClientConnection *connection, char_t *param);
void ftpServerProcessPbsz(FtpClientConnection *connection, char_t *param);
void ftpServerProcessProt(FtpClientConnection *connection, char_t *param);
void ftpServerProcessType(FtpClientConnection *connection, char_t *param);
void ftpServerProcessStru(FtpClientConnection *connection, char_t *param);
void ftpServerProcessMode(FtpClientConnection *connection, char_t *param);
void ftpServerProcessUser(FtpClientConnection *connection, char_t *param);
void ftpServerProcessPass(FtpClientConnection *connection, char_t *param);
void ftpServerProcessRein(FtpClientConnection *connection, char_t *param);
void ftpServerProcessQuit(FtpClientConnection *connection, char_t *param);
void ftpServerProcessPort(FtpClientConnection *connection, char_t *param);
void ftpServerProcessEprt(FtpClientConnection *connection, char_t *param);
void ftpServerProcessPasv(FtpClientConnection *connection, char_t *param);
void ftpServerProcessEpsv(FtpClientConnection *connection, char_t *param);
void ftpServerProcessAbor(FtpClientConnection *connection, char_t *param);
void ftpServerProcessPwd(FtpClientConnection *connection, char_t *param);
void ftpServerProcessCwd(FtpClientConnection *connection, char_t *param);
void ftpServerProcessCdup(FtpClientConnection *connection, char_t *param);
void ftpServerProcessList(FtpClientConnection *connection, char_t *param);
void ftpServerProcessNlst(FtpClientConnection *connection, char_t *param);
void ftpServerProcessMkd(FtpClientConnection *connection, char_t *param);
void ftpServerProcessRmd(FtpClientConnection *connection, char_t *param);
void ftpServerProcessSize(FtpClientConnection *connection, char_t *param);
void ftpServerProcessRetr(FtpClientConnection *connection, char_t *param);
void ftpServerProcessStor(FtpClientConnection *connection, char_t *param);
void ftpServerProcessAppe(FtpClientConnection *connection, char_t *param);
void ftpServerProcessRnfr(FtpClientConnection *connection, char_t *param);
void ftpServerProcessRnto(FtpClientConnection *connection, char_t *param);
void ftpServerProcessDele(FtpClientConnection *connection, char_t *param);

void ftpServerProcessUnknownCmd(FtpClientConnection *connection, char_t *param);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
