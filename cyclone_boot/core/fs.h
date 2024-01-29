/**
 * @file fs.h
 * @brief CycloneBOOT File System Layer
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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

#ifndef _FS_H
#define _FS_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "error.h"

//File System Driver Major version
#define FS_DRIVER_VERSION_MAJOR 0x01
//File System Driver Minor version
#define FS_DRIVER_VERSION_MINOR 0x00
//File System Driver Revision version
#define FS_DRIVER_VERSION_PATCH 0x00
//File System Driver version
#define FS_DRIVER_VERSION (uint32_t)(((FS_DRIVER_VERSION_MAJOR & 0xFF) << 16) | \
                               ((FS_DRIVER_VERSION_MINOR & 0xFF) << 8) | \
                               (FS_DRIVER_VERSION_PATCH & 0xFF))

/**
 * @brief File System file handler definition
 **/

typedef void FsFileHandler;


/**
 * @brief File System Status definition
 **/

typedef enum
{
    FS_STATUS_OK = 0,
    FS_STATUS_BUSY,
    FS_STATUS_ERR
} FsStatus;


/**
 * @brief File System Driver Information
 **/

typedef struct
{
   uint32_t version;       ///<File System driver version
   char* fsName;           ///<File System memory name
   char* fsPath;           ///<File System memory volume path
   size_t fsSize;          ///<File System memory size
   uint32_t flags;         ///<File System memory flags
} FsInfo;


/**
 * @brief Fs initialization function
 **/

typedef error_t (*FileSystemInit)(void);


/**
 * @brief Fs de-initialization function
 **/

typedef error_t (*FileSystemDeInit)(void);


/**
 * @brief Fs status function
 **/

typedef error_t (*FileSystemGetInfo)(const FsInfo **fsInfo);


/**
 * @brief Fs status function
 **/

typedef error_t (*FileSystemGetStatus)(FsStatus *fsStatus);


/**
 * @brief Fs open function
 **/

typedef FsFileHandler* (*FileSystemOpen)(const char* path, uint8_t mode);


/**
 * @brief Fs close function
 **/

typedef error_t (*FileSystemClose)(FsFileHandler *fp);


/**
 * @brief Write Data into Fs function
 **/

typedef error_t (*FileSystemWrite)(FsFileHandler *fp, uint8_t offset, uint8_t *data, size_t length);


/**
 * @brief Read Data from Fs function
 **/

typedef error_t (*FileSystemRead)(FsFileHandler *fp, uint8_t offset, uint8_t* data, size_t length);


/**
 * @brief Erase Data from Fs function
 **/

typedef error_t (*FileSystemErase)(FsFileHandler *fp, uint8_t offset, size_t length);


/**
 * @brief Fs Driver definition
 **/

typedef struct
{
    FileSystemInit init;                        ///<Fs Driver init callback function
    FileSystemDeInit deInit;                    ///<Fs Driver deinit callback function
    FileSystemGetInfo getInfo;                ///<Fs Driver get info callback function
    FileSystemGetStatus getStatus;            ///<Fs Driver get status callback function
    FileSystemOpen open;                        ///<Fs Driver open file callback function
    FileSystemClose close;                      ///<Fs Driver close file callback function
    FileSystemWrite write;                      ///<Fs Driver write data callback function
    FileSystemRead read;                        ///<Fs Driver read data callback function
    FileSystemErase erase;                      ///<Fs Driver erase data callback function
} FsDriver;

#endif //_FS_H
