/**
 * @file mcu.h
 * @brief CycloneBOOT MCU layer
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

#ifndef _MCU_H
#define _MCU_H

//Dependencies
#include <stdint.h>

//CycloneBOOT MCU Driver Major version
#define MCU_DRIVER_VERSION_MAJOR 0x01
//CycloneBOOT MCU Driver Minor version
#define MCU_DRIVER_VERSION_MINOR 0x00
//CycloneBOOT MCU Driver Revison version
#define MCU_DRIVER_VERSION_PATCH 0x00
//CycloneBOOT MCU Driver version
#define MCU_DRIVER_VERSION (uint32_t)(((MCU_DRIVER_VERSION_MAJOR & 0xFF) << 16) | \
                               ((MCU_DRIVER_VERSION_MINOR & 0xFF) << 8) | \
                               (MCU_DRIVER_VERSION_PATCH & 0xFF))

//CycloneBOOT mcu layer related functions
extern uint32_t mcuGetVtorOffset(void);
extern void mcuSystemReset(void);
extern void mcuJumpToApplication(uint32_t address) __attribute__ ((section (".code_in_ram")));

#endif //!_MCU_H
