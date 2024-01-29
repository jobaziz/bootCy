/**
 * @file stm32h7xx_flash_driver.h
 * @brief CycloneBOOT STM32H7xx Flash Driver
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

#ifndef _STM32H7xx_FLASH_DRIVER_H
#define _STM32H7xx_FLASH_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_flash.h"
#include "core/flash.h"
#include "error.h"

//STM32H7xx name
#define STM32H7xx_FLASH_NAME "STM32H7xx Internal Flash"
//STM32H7xx start addr
#define STM32H7xx_FLASH_ADDR 0x08000000
//STM32H7xx write size
#define STM32H7xx_FLASH_WRITE_SIZE (FLASH_NB_32BITWORD_IN_FLASHWORD*4) //N-bytes word
//STM32H7xx read size
#define STM32H7xx_FLASH_READ_SIZE 0x4 //32-bytes word

//Device flash size
#if (defined(STM32H742xI) || defined(STM32H743xI) || defined(STM32H753xI) || \
    defined(STM32H745xI) || defined(STM32H755xI) || defined(STM32H747xI) || \
    defined(STM32H757xI) || defined(STM32H7A3xI) || defined(STM32H73BxI))
#define STM32H7xx_FLASH_SIZE 0x200000 //2MB flash
#elif (defined(STM32H742xG) || defined(STM32H743xG) || defined(STM32H745xG) || \
       defined(STM32H755xG) || defined(STM32H747xG) || defined(STM32H757xG) || \
       defined(STM32H7A3xG))
#define STM32H7xx_FLASH_SIZE 0x100000 //1MB flash
#elif (defined(STM32H723xE) || defined(STM32H733xE) || defined(STM32H725xE) || \
       defined(STM32H735xE))
#define STM32H7xx_FLASH_SIZE 0x80000 //512KB flash
#elif (defined(STM32H750xB) || defined(STM32H730xx) || defined(STM32H7B0xx))
#define STM32H7xx_FLASH_SIZE 0x20000 //128KB flash
#else
#error You MUST define one of the above device part number!
#endif

//Device flash sector size
#define STM32H7xx_FLASH_SECTOR_SIZE FLASH_SECTOR_SIZE

//Device flash sector number
#define STM32H7xx_FLASH_SECTOR_NUMBER FLASH_SECTOR_TOTAL*(STM32H7xx_FLASH_SIZE/FLASH_SECTOR_TOTAL/FLASH_SECTOR_SIZE)

//Device flash bank IDs
#define STM32H7xx_FLASH_BANK1_ID 1
#define STM32H7xx_FLASH_BANK2_ID 2

//Device flash bank info
#if defined(DUAL_BANK)
#define STM32H7xx_FLASH_BANK_SIZE STM32H7xx_FLASH_SIZE/2
#define STM32H7xx_FLASH_BANK1_ADDR FLASH_BANK1_BASE
#define STM32H7xx_FLASH_BANK2_ADDR FLASH_BANK2_BASE
#else
#define STM32H7xx_FLASH_BANK_SIZE STM32H7xx_FLASH_SIZE
#define STM32H7xx_FLASH_BANK1_ADDR FLASH_BANK1_BASE
#define STM32H7xx_FLASH_BANK2_ADDR FLASH_BANK1_BASE
#endif

//STM32H7xx Internal Memory Flash driver
extern const FlashDriver stm32h7xxFlashDriver;

#endif //!_STM32H7xx_FLASH_DRIVER_H
