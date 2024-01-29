/**
 * @file fs_driver.c
 * @brief CycloneBOOT File System Driver
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
#include "fs_port.h"
#include "fs_driver.h"
#include "debug.h"

//Memory driver private related functions
error_t fileSystemDriverInit(void);
error_t fileSystemDriverDeInit(void);
error_t fileSystemDriverGetInfo(const FsInfo **info);
error_t fileSystemDriverGetStatus(FsStatus *status);
FsFile* fileSystemDriverOpen(const char_t* path, uint_t flags);
void fileSystemDriverClose(FsFile *file);
error_t fileSystemDriverWrite(FsFile *file, uint32_t offset, uint8_t* data, size_t length);
error_t fileSystemDriverRead(FsFile *file, uint32_t offset, uint8_t* data, size_t length);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Memory Information
 **/

const FsInfo fileSystemDriverInfo =
{
   FS_DRIVER_VERSION,
   FILE_SYSTEM_NAME,
   FILE_SYSTEM_PATH,
   FILE_SYSTEM_SIZE,
   0
};

/**
 * @brief Custom File System driver
 **/

const FsDriver fileSystemDriver =
{
   fileSystemDriverInit,
   fileSystemDriverDeInit,
   fileSystemDriverGetInfo,
   fileSystemDriverGetStatus,
   fileSystemDriverOpen,
   fileSystemDriverClose,
   fileSystemDriverWrite,
   fileSystemDriverRead
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief Initialize File System Memory.
 * @return Error code
 **/

error_t fileSystemDriverInit(void)
{
   error_t error;

   //TODO:  Maybe use define avoid initialize here if already done in application
   //       #if !defined(FS_INIT_FROM_APP)

   //Initializing file system
   error = fsInit();
   //Is any error?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialized file system!\r\n");
      //Return error
      return error;
   }

   //TODO:  Maybe use define avoid initialize here if already done in application
   //       #endif

   //Successfull process
   return NO_ERROR;
}


/**
 * @brief De-Initialize File System Memory.
 * @return Error code
 **/

error_t fileSystemDriverDeInit(void)
{
   error_t error;

   //De-Initializing file system
   error = fsDeInit();
   //Is any error?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to de-initialized file system!\r\n");
      //Return error
      return error;
   }

   //Successful process
   return NO_ERROR;
}


/**
 * @brief Get File System Memory information.
 * @param[in,out] info Pointeur to the Memory information structure to be returned
 * @return Error code
 **/

error_t fileSystemDriverGetInfo(const FsInfo **info)
{
   //Set File System information pointeur
   *info = (const FsInfo*) &fileSystemDriverInfo;

   //Successfull process
   return NO_ERROR;
}


/**
 * @brief Get File System Memory status.
 * @param[in,out] status Pointeur to the Memory status to be returned
 * @return Error code
 **/

error_t fileSystemDriverGetStatus(FsStatus *status)
{
   //Check parameter vailidity
   if(status == NULL)
      return ERROR_INVALID_PARAMETER;

   //TODO: Check how to file system get status

   //Set File System memory status
   *status = FS_STATUS_OK;

   //Successfull process
   return NO_ERROR;
}


/**
 * @brief Open the specified file for reading or writing.
 * @param[in] path NULL-terminated string specifying the filename
 * @param[in] mode Type of access permitted (FS_FILE_MODE_READ,
 *   FS_FILE_MODE_WRITE or FS_FILE_MODE_CREATE)
 * @return File handle
 **/

FsFile* fileSystemDriverOpen(const char_t* path, uint_t flags)
{
   //Check parameter vailidity
   if(path == NULL)
      return ERROR_INVALID_PARAMETER;

   //File already exists?
   if(fsFileExists(path))
   {
      //Creating and opening the specified file in read/write mode
      return fsOpenFile(path, FS_FILE_MODE_CREATE | FS_FILE_MODE_WRITE |
         FS_FILE_MODE_READ);
   }
   else
   {
      //Opening the specified file in read/write mode
      return fsOpenFile(path, FS_FILE_MODE_WRITE | FS_FILE_MODE_READ);
   }
}


/**
 * @brief Close a file
 * @param[in] file Handle that identifies the file to be closed
 **/

void fileSystemDriverClose(FsFile* file)
{
   if(file != NULL)
   {
      //Closing the specified file
      fsCloseFile(file);
   }
}


/**
 * @brief Write data to the specified file starting the given offset.
 * @param[in] file Handle that identifies the file to be written
 * @param[in] offset File's offset to start writting
 * @param[in] data Pointer to a buffer containing the data to be written
 * @param[in] length Number of data bytes to write
 * @return Error code
 **/

error_t fileSystemDriverWrite(FsFile* file, uint32_t offset, uint8_t* data, size_t length)
{
   error_t error;

   //Check parameters validity
   if(file == NULL || data == NULL || length == 0)
      return ERROR_INVALID_PARAMETER;

   //Moving file's cursor
   error = fsSeekFile(file, offset, FS_SEEK_SET);
   //Is any error?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to move file's cursor position!\r\n");
      return error;
   }

   //Writting data into file
   error = fsWriteFile(file, data, length);
   //Is any error?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to write data into file!\r\n");
      return error;
   }

   //Successful process
   return NO_ERROR;
}


/**
 * @brief Read data from the specified file starting at the given offset.
 * @param[in] file Handle that identifies the file to be read
 * @param[in] offset File's offset to start reading
 * @param[in] data Pointer to the buffer where to copy the data
 * @param[in] length Size of the buffer, in bytes
 * @return Error code
 **/

error_t fileSystemDriverRead(FsFile* file, uint32_t offset, uint8_t* data, size_t length)
{
   error_t error;

   //Check parameters validity
   if(file == NULL || data == NULL || length == 0)
      return ERROR_INVALID_PARAMETER;

   //Moving file's cursor
   error = fsSeekFile(file, offset, FS_SEEK_SET);
   //Is any error?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to move file's cursor position!\r\n");
      return error;
   }

   //Read data from file
   error = fsWriteFile(file, data, length);
   //Is any error?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to read data from file!\r\n");
      return error;
   }

   //Successful process
   return NO_ERROR;
}
