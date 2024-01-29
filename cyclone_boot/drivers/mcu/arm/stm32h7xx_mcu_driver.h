/**
 * @file arm_driver.h
 * @brief ARM MCU driver managment
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


#ifndef _ARM_DRIVER_H
#define _ARM_DRIVER_H

//Dependencies
#include "stdint.h"

//You must align the offset to the number of exception entries in the vector
// table. The minimum alignment is 32 words, enough for up to 16 interrupts. For more interrupts,
// adjust the alignment by rounding up to the next power of two

#define MCU_VTOR_OFFSET 0x400 //Could be less according to the cortex-m device but
                              // 0x400 will work for every arm cortex-m devices
                              // (16 execptions + 240 interrupts (max arm interrupt number) = 256 words = 1024 bytes = 0x400 bytes)

//STM32H7xx mcu driver related functions
uint32_t mcuGetVtorOffset(void);
void mcuSystemReset(void);
void mcuJumpToApplication(uint32_t address) __attribute__ ((section (".code_in_ram")));

#endif //!_ARM_DRIVER_H
