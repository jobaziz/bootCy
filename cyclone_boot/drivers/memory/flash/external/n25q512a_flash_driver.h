/**
 * @file n25q512a_flash_driver.h
 * @brief CycloneBOOT N25Q512A Flash Driver
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

#ifndef _N25Q512A_FLASH_DRIVER_H
#define _N25Q512A_FLASH_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "core/flash.h"
#include "error.h"

//N25Q512A name
#define N25Q512A_NAME "N25Q512A External QPSI Nor Flash"
//N25Q512A start addr
#define N25Q512A_ADDR 0x00000000
//N25Q512A write size
#define N25Q512A_WRITE_SIZE 0x04 //4-bytes word
//N25Q512A read size
#define N25Q512A_READ_SIZE 0x04 //4-bytes word


//N25Q512A size
#define N25Q512A_SIZE 0x4000000
//N25Q512A sectors number
#define N25Q512A_SECTORS_NUMBER 1024
//N25Q512A Sectors size
#define N25Q512A_SECTORS_SIZE 0x10000
//N25Q512A Subsectors 4KB number
#define N25Q512A_SUBSECTORS_NUMBER 16384
//N25Q512A Subsectors 4KB size
#define N25Q512A_SUBSECTORS_SIZE 0x1000

//N25Q512A Internal Memory Flash driver
extern const FlashDriver n25q512aFlashDriver;

#endif //!_N25Q512A_FLASH_DRIVER_H
