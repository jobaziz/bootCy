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

//MT25TL01G name
#define MT25TL01G_NAME "MT25TL01G External QPSI Nor Flash"
//MT25TL01G start addr
#define MT25TL01G_ADDR 0x00000000
//MT25TL01G write size
#define MT25TL01G_WRITE_SIZE 0x04 //4-bytes word
//MT25TL01G read size
#define MT25TL01G_READ_SIZE 0x04 //4-bytes word


//MT25TL01G size
#define MT25TL01G_SIZE 0x8000000
//MT25TL01G sectors number
#define MT25TL01G_SECTORS_NUMBER 2048
//MT25TL01G Sectors size
#define MT25TL01G_SECTORS_SIZE 0x10000
//MT25TL01G Subsectors 4KB number
#define MT25TL01G_SUBSECTORS_NUMBER 32768
//MT25TL01G Subsectors 4KB size
#define MT25TL01G_SUBSECTORS_SIZE (2*0x1000)

//MT25TL01G Internal Memory Flash driver
extern const FlashDriver mt25tl01gFlashDriver;

#endif //!_N25Q512A_FLASH_DRIVER_H
