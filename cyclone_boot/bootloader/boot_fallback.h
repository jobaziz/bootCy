/**
 * @file boot_fallback.h
 * @brief CycloneBOOT Bootloader fallback managment
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

#ifndef _BOOT_FALLBACK_H
#define _BOOT_FALLBACK_H

//Dependencies
#include "bootloader/boot.h"
#include "core/cboot_error.h"

typedef enum
{
   TRIGGER_STATUS_IDLE,
   TRIGGER_STATUS_RAISED
}TriggerStatus;


//CycloneBOOT Booltoader Fallback related functions
cboot_error_t fallbackTask(BootContext *context, Memory *memories);

#if ((defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || \
   defined(__GNUC__) || defined(__CC_ARM) || defined(__IAR_SYSTEMS_ICC__) || \
   defined(__TASKING__) || defined(__CWCC__) || defined(__TI_ARM__))
cboot_error_t fallbackTriggerInit(void);
cboot_error_t fallbackTriggerGetStatus(TriggerStatus *status);
#elif ((defined(_MSC_VER) || defined(_WIN32) || defined(WIN32) || \
   defined(WIN64) || defined(__unix__))
extern cboot_error_t fallbackTriggerInit(void);
extern cboot_error_t fallbackTriggerGetStatus(TriggerStatus *status);
#endif

#endif //_BOOT_FALLBACK_H
