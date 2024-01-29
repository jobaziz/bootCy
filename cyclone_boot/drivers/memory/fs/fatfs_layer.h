/**
 * @file fatfs_layer.h
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

#ifndef _FS_LAYER_H
#define _FS_LAYER_H

//Dependencies
#include <stdio.h>
#include "core/fs.h"

//FatFS File System name
#define FAT_FS_NAME "Custom File SystemInternal Flash"
//FatFS File System volume path name
#define FAT_FS_PATH ""
//FatFS Fime System device size
#define FAT_FS_SIZE (size_t)0x200000000 //8GB

//FatFS driver
extern const FsDriver fat_fs_driver;

#endif //!_FS_LAYER_H
