/**
 * @file boot_common.h
 * @brief CycloneBOOT Bootloader common functions
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

#ifndef _BOOT_COMMON_H
#define _BOOT_COMMON_H

//Dependencies
#include "image/image.h"
#include "bootloader/boot.h"
#include "core/cboot_error.h"

//Initialization vector size of encrypted images in external flash memory
#define INIT_VECT_SIZE 16

//CycloneBOOT Bootloader common related functions
void bootChangeState(BootContext *context, BootState newState);
cboot_error_t bootInitPrimaryMem(BootContext *context, BootSettings *settings);
cboot_error_t bootInitSecondaryMem(BootContext *context, BootSettings *settings);
cboot_error_t bootSelectUpdateImageSlot(BootContext *context, Slot *selectedSlot);
cboot_error_t bootUpdateApp(BootContext *context, Slot *slot);
cboot_error_t bootCheckImage(Slot *slot);
cboot_error_t bootGetSlotImgHeader(Slot *slot, ImageHeader *header);
cboot_error_t bootCheckSlotAppResetVector(Slot *slot);

#endif //_BOOT_COMMON_H
