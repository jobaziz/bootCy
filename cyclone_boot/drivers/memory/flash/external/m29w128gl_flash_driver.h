/**
 * @file m29w128gl_flash_driver.h
 * @brief CycloneBOOT M29W128GL Flash Driver
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

#ifndef _M29W128GL_FLASH_DRIVER_H
#define _M29W128GL_FLASH_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "core/flash.h"
#include "error.h"

//M29W128GL name
#define M29W128GL_NAME "M29W128GL External Parallel Nor Flash"
//M29W128GL start addr
#define M29W128GL_ADDR 0x00000000
//M29W128GL write size
#define M29W128GL_WRITE_SIZE 0x02 //2-bytes word
//M29W128GL read size
#define M29W128GL_READ_SIZE 0x02 //2-bytes word


//M29W128GL size
#define M29W128GL_SIZE 0x8000000
//M29W128GL sectors number
#define M29W128GL_SECTORS_NUMBER 1024
//M29W128GL Sectors size
#define M29W128GL_SECTORS_SIZE 0x20000
//M29W128GL Subsectors 4KB number
#define M29W128GL_SUBSECTORS_NUMBER 1024
//M29W128GL Subsectors 4KB size
#define M29W128GL_SUBSECTORS_SIZE 0x20000

//M29W128GL Internal Memory Flash driver
extern const FlashDriver m29w128glFlashDriver;

#endif //!_M29W128GL_FLASH_DRIVER_H
