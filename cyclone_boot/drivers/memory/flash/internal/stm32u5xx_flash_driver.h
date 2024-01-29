/**
 * @file stm32u5xx_flash_driver.h
 * @brief CycloneBOOT STM32U5xx Flash Driver
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

#ifndef _STM32U5XX_FLASH_DRIVER_H
#define _STM32U5XX_FLASH_DRIVER_H

//Dependencies
#include <string.h>
#include "stm32u5xx_hal.h"
#include "stm32u5xx_flash_driver.h"
#include "stm32u5xx_hal_flash_ex.h"
#include "core/flash.h"

//STM32U5xx name
#define STM32U5xx_FLASH_NAME "STM32U5xx Internal Flash"
//STM32U5xx start address
#define STM32U5xx_FLASH_ADDR 0x08000000UL
//STM32U5xx flash size
#define STM32U5xx_FLASH_SIZE 0x200000U
//STM32U5xx write size
#define STM32U5xx_FLASH_WRITE_SIZE 16 //N-bytes word
//STM32U5xx read size
#define STM32U5xx_FLASH_READ_SIZE 16 // 16 bytes (quad-word) also : 8 bytes (double-word), 4 bytes (word), 2 bytes and 1 byte

//Device flash sector size
//#define FLASH_SECTOR_SIZE 0x00002000UL
#define STM32U5xx_FLASH_SECTOR_SIZE FLASH_PAGE_SIZE

//Device flash bank IDs
#define STM32U5xx_FLASH_BANK1_ID 1
#define STM32U5xx_FLASH_BANK2_ID 2

//Device flash bank info
#define FLASH_BANK1_BASE          (0x08000000UL)
#define FLASH_BANK2_BASE          (0x08100000UL)

#if defined(FLASH_DB_MODE)
#define STM32U5xx_FLASH_BANK_SIZE   STM32U5xx_FLASH_SIZE/2
#define STM32U5xx_FLASH_BANK1_ADDR  FLASH_BANK1_BASE
#define STM32U5xx_FLASH_BANK2_ADDR  FLASH_BANK2_BASE

//Device flash sector number
#define FLASH_SECTOR_SIZE               STM32U5xx_FLASH_SECTOR_SIZE  //8 Kbytes
#define STM32U5xx_FLASH_SECTOR_NUMBER   256

#else
#define STM32U5xx_FLASH_BANK_SIZE       STM32U5xx_FLASH_SIZE
#define STM32U5xx_FLASH_BANK1_ADDR      FLASH_BANK1_BASE
#define STM32U5xx_FLASH_BANK2_ADDR      FLASH_BANK2_BASE

//Device flash sector number
#define FLASH_SECTOR_SIZE               STM32U5xx_FLASH_SECTOR_SIZE * 2 //16 Kbytes
#define STM32U5xx_FLASH_SECTOR_NUMBER   128
#endif

//STM32U5xx Internal Memory Flash driver
const extern FlashDriver stm32u5xxFlashDriver;

#endif /* _STM32U5XX_FLASH_DRIVER_H */
