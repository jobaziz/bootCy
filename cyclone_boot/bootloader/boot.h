/**
 * @file boot.h
 * @brief CycloneBOOT Bootloader managment
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

#ifndef _BOOT_H
#define _BOOT_H

//Dependencies
#include "boot_config.h"
#include "core/flash.h"
#include "core/crypto.h"
#include "cipher/aes.h"
#include "cipher_modes/cbc.h"
#include "core/cboot_error.h"

// default offset
#ifndef BOOT_OFFSET
#define BOOT_OFFSET 0x10000
#endif

// Enable fallback support
#ifndef BOOT_FALLBACK_SUPPORT
#define BOOT_FALLBACK_SUPPORT DISABLED
#elif (BOOT_FALLBACK_SUPPORT != ENABLED && BOOT_FALLBACK_SUPPORT != DISABLED)
#error BOOT_FALLBACK_SUPPORT parameter is not valid
#endif

// Enable anti-rollback support
#ifndef BOOT_ANTI_ROLLBACK_SUPPORT
#define BOOT_ANTI_ROLLBACK_SUPPORT DISABLED
#elif (BOOT_ANTI_ROLLBACK_SUPPORT != ENABLED && BOOT_ANTI_ROLLBACK_SUPPORT != DISABLED)
#error BOOT_ANTI_ROLLBACK_SUPPORT parameter is not valid
#endif

// Enable external memory encryption support
#ifndef BOOT_EXT_MEM_ENCRYPTION_SUPPORT
#define BOOT_EXT_MEM_ENCRYPTION_SUPPORT DISABLED
#elif (BOOT_EXT_MEM_ENCRYPTION_SUPPORT != ENABLED && BOOT_EXT_MEM_ENCRYPTION_SUPPORT != DISABLED)
#error BOOT_EXT_MEM_ENCRYPTION_SUPPORT parameter is not valid
#endif

/**
 * @brief Bootloader States definition
 **/

typedef enum
{
   BOOT_STATE_IDLE,
   BOOT_STATE_RUN_APP,
   BOOT_STATE_UPDATE_APP,
   BOOT_STATE_FALLBACK_APP,
   BOOT_STATE_ERROR
}BootState;


/**
 * @brief Bootloader user settings structure
 **/

typedef struct
{
#if ((BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED) \
   && (BOOT_FALLBACK_SUPPORT == ENABLED))
   const char_t *psk;                  ///<Secondary flash slot cipher key
   size_t pskSize;                     ///<Secondary flash slot cipher key size
#endif

  Memory memories[NB_MEMORIES];
} BootSettings;


/**
 * @brief Bootloader Context structure
 **/
typedef struct
{
   BootState state;              ///<Bootloader state
   BootSettings settings;        ///<Bootloader user settings
   Memory memories[NB_MEMORIES];
#if (BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED)
   uint8_t psk[32];              ///<Cipher PSK key for image decryption
   size_t pskSize;               ///<Cipher PSK key size
#endif
    Slot selectedSlot;
} BootContext;


//Bootloader related functions
cboot_error_t bootTask(BootContext *context);
void bootGetDefaultSettings(BootSettings *settings);
cboot_error_t bootInit(BootContext *context, BootSettings *settings);
cboot_error_t bootCreateBackupSlot(BootContext *context);
cboot_error_t bootUpdateApp(BootContext *context, Slot *slot);

//Extern device MCU related function
extern uint32_t mcuGetVtorOffset(void);
extern void mcuSystemReset(void);
extern void mcuJumpToApplication(uint32_t address);

#endif //_BOOT_H
