/**
 * @file fs_driver.h
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

#ifndef _FS_DRIVER_H
#define _FS_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "core/fs.h"
#include "error.h"

//File System name
#define FILE_SYSTEM_NAME "Custom File SystemInternal Flash"
//File System volume path name
#define FILE_SYSTEM_PATH ""
//Fime System device size
#define FILE_SYSTEM_SIZE (size_t)0x200000000 //8GB

//File System driver
extern const FsDriver fsDriver;

#endif //!_FS_DRIVER_H
