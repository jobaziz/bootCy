/**
 * @file fs_port.h
 * @brief File system abstraction layer
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

#ifndef _FS_PORT_H
#define _FS_PORT_H

//Dependencies
#include "fs_port_config.h"
#include "os_port.h"
#include "date_time.h"
#include "error.h"

//Maximum filename length
#ifndef FS_MAX_NAME_LEN
   #define FS_MAX_NAME_LEN 127
#elif (FS_MAX_NAME_LEN < 11)
   #error FS_MAX_NAME_LEN parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief File attributes
 **/

typedef enum
{
   FS_FILE_ATTR_READ_ONLY   = 0x01,
   FS_FILE_ATTR_HIDDEN      = 0x02,
   FS_FILE_ATTR_SYSTEM      = 0x04,
   FS_FILE_ATTR_VOLUME_NAME = 0x08,
   FS_FILE_ATTR_DIRECTORY   = 0x10,
   FS_FILE_ATTR_ARCHIVE     = 0x20
} FsFileAttributes;


/**
 * @brief File access mode
 **/

typedef enum
{
   FS_FILE_MODE_READ   = 1,
   FS_FILE_MODE_WRITE  = 2,
   FS_FILE_MODE_CREATE = 4,
   FS_FILE_MODE_TRUNC  = 8
} FsFileMode;


/**
 * @brief File seek origin
 **/

typedef enum
{
   FS_SEEK_SET = 0,
   FS_SEEK_CUR = 1,
   FS_SEEK_END = 2
} FsSeekOrigin;


/**
 * @brief File status
 **/

typedef struct
{
   uint32_t attributes;
   uint32_t size;
   DateTime modified;
} FsFileStat;


/**
 * @brief Directory entry
 **/

typedef struct
{
   uint32_t attributes;
   uint32_t size;
   DateTime modified;
   char_t name[FS_MAX_NAME_LEN + 1];
} FsDirEntry;


//FatFs port?
#if defined(USE_FATFS)
   #include "fs_port_fatfs.h"
//RL-FlashFS port?
#elif defined(USE_RL_FS)
   #include "fs_port_rl_fs.h"
//SPIFFS port?
#elif defined(USE_SPIFFS)
   #include "fs_port_spiffs.h"
//Windows port?
#elif defined(_WIN32)
   #include "fs_port_posix.h"
//POSIX port?
#elif defined(__linux__) || defined(__FreeBSD__)
   #include "fs_port_posix.h"
//Custom port?
#elif defined(USE_CUSTOM_FS)
   #include "fs_port_custom.h"
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
