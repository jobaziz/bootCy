/**
 * @file ftp_server_misc.h
 * @brief Helper functions for FTP server
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

#ifndef _FTP_SERVER_MISC_H
#define _FTP_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_server.h"

//Time constant
#define FTP_SERVER_180_DAYS (180 * 86400)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP server related functions
void ftpServerTick(FtpServerContext *context);

uint16_t ftpServerGetPassivePort(FtpServerContext *context);

error_t ftpServerGetPath(FtpClientConnection *connection,
   const char_t *inputPath, char_t *outputPath, size_t maxLen);

uint_t ftpServerGetFilePermissions(FtpClientConnection *connection,
   const char_t *path);

size_t ftpServerFormatDirEntry(const FsDirEntry *dirEntry, uint_t perm,
   char_t *buffer);

const char_t *ftpServerStripRootDir(FtpServerContext *context,
   const char_t *path);

const char_t *ftpServerStripHomeDir(FtpClientConnection *connection,
   const char_t *path);

void ftpServerCloseConnection(FtpClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
