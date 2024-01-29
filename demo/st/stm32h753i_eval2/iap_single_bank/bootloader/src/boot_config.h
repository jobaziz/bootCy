/**
 * @file boot_config.h
 * @brief CycloneBOOT configuration file
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

#ifndef _BOOT_CONFIG_H
#define _BOOT_CONFIG_H

//Bootloader trace level
#define BOOT_TRACE_LEVEL 4

//Number of memory to be used
#define NB_MEMORIES 2

//Bootloader external flash memory encryption support
#define BOOT_EXT_MEM_ENCRYPTION_SUPPORT DISABLED

//Bootloader fallback support
#define BOOT_FALLBACK_SUPPORT DISABLED

//Bootloader Anti-Rollback support
#define BOOT_ANTI_ROLLBACK_SUPPORT DISABLED

#endif //!_BOOT_CONFIG_H
