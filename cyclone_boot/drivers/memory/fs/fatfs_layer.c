/**
 * @file fatfs_layer.c
 * @brief CycloneBOOT FATFS file system layer
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include "core/fs.h"
#include "fatfs_layer.h"

/**
 * @brief FatFS driver
 **/

error_t FatFsDriverInit(void);
FsFileHandler* FatFsOpen(const char* path, uint8_t mode);
error_t FatFsDriverWrite(FsFileHandler *fp, uint8_t offset, uint8_t *data, size_t length);
error_t FatFsDriverRead(FsFileHandler *fp, uint8_t offset, uint8_t* data, size_t length);
error_t FatFsClose(FsFileHandler *fp);
error_t FatFsDriverErase(FsFileHandler *fp, uint8_t offset, size_t length);
error_t FatFsGetInfo(const FsInfo **fsInfo);
error_t FatFsGetStatus(FsStatus *fsStatus);
error_t FatFsDriverDeInit(void);


/**
 * @brief Memory Information
 **/

const FsInfo fileSystemDriverInfo =
{
   FS_DRIVER_VERSION,
   FAT_FS_NAME,
   FAT_FS_PATH,
   FAT_FS_SIZE,
   0
};


const FsDriver fat_fs_driver =
{
    FatFsDriverInit,
    FatFsDriverDeInit,
    FatFsGetInfo,
    FatFsGetStatus,
    FatFsOpen,
    FatFsClose,
    FatFsDriverWrite,
    FatFsDriverRead,
    FatFsDriverErase,
};

/**
 * @brief Fs initialization function
 **/

error_t FatFsDriverInit(void) {
    return ERROR_NOT_IMPLEMENTED;
};

/**
 * @brief Fs file open function
 **/

FsFileHandler* FatFsOpen(const char* path, uint8_t mode) {
    return (FsFileHandler*)NULL;
};


error_t FatFsGetInfo(const FsInfo **fsInfo)
{
    //Set File System information pointeur
    *fsInfo = (const FsInfo*) &fileSystemDriverInfo;

    //Successfull process
    return NO_ERROR;
}


/**
 * @brief Fs status function
 **/

error_t FatFsGetStatus(FsStatus *fsStatus) {
    return ERROR_NOT_IMPLEMENTED;
};

/**
 * @brief Write Data into Fs function
 **/

error_t FatFsDriverWrite(FsFileHandler *fp, uint8_t offset, uint8_t *data, size_t length) {
    return ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief Read Data from Fs function
 **/

error_t FatFsDriverRead(FsFileHandler *fp, uint8_t offset, uint8_t* data, size_t length) {
    return ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief Close file function
 **/

error_t FatFsClose(FsFileHandler *fp) {
    return ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief Erase Data from Fs function
 **/

error_t FatFsDriverErase(FsFileHandler *fp, uint8_t offset, size_t length) {
    return ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief Fs initialization function
 **/

error_t FatFsDriverDeInit(void) {
    return ERROR_NOT_IMPLEMENTED;
};
