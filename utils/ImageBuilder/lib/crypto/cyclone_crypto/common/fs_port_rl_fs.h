/**
 * @file fs_port_rl_fs.h
 * @brief File system abstraction layer (RL-FlashFS)
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
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
 * @version 3.0.0
 **/

#ifndef _FS_PORT_RL_FS_H
#define _FS_PORT_RL_FS_H

//Dependencies
#include "os_port.h"
#include "rl_fs.h"

//Maximum path length
#ifndef FS_MAX_PATH_LEN
   #define FS_MAX_PATH_LEN 260
#elif (FS_MAX_PATH_LEN < 1)
   #error FS_MAX_PATH_LEN parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief File descriptor
 **/

typedef void FsFile;


/**
 * @brief Directory descriptor
 **/

typedef struct
{
   char_t pattern[FS_MAX_PATH_LEN + 1];
   fsFileInfo fileInfo;
} FsDir;


//File system abstraction layer
error_t fsInit(void);

bool_t fsFileExists(const char_t *path);
error_t fsGetFileSize(const char_t *path, uint32_t *size);
error_t fsGetFileStat(const char_t *path, FsFileStat *fileStat);
error_t fsRenameFile(const char_t *oldPath, const char_t *newPath);
error_t fsDeleteFile(const char_t *path);

FsFile *fsOpenFile(const char_t *path, uint_t mode);
error_t fsSeekFile(FsFile *file, int_t offset, uint_t origin);
error_t fsWriteFile(FsFile *file, void *data, size_t length);
error_t fsReadFile(FsFile *file, void *data, size_t size, size_t *length);
void fsCloseFile(FsFile *file);

bool_t fsDirExists(const char_t *path);
error_t fsCreateDir(const char_t *path);
error_t fsRemoveDir(const char_t *path);

FsDir *fsOpenDir(const char_t *path);
error_t fsReadDir(FsDir *dir, FsDirEntry *dirEntry);
void fsCloseDir(FsDir *dir);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
