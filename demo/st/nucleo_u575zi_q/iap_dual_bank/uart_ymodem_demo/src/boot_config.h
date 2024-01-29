/**
 * @file crypto_config.h
 * @brief CycloneCrypto configuration file
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

//Trace level for CycloneBOOT stack debugging
#define CBOOT_TRACE_LEVEL 4

//Number of memories used
#define NB_MEMORIES 1
//Update Single Bank Mode support
#define UPDATE_SINGLE_BANK_SUPPORT DISABLED
//Update Dual Bank Mode support
#define UPDATE_DUAL_BANK_SUPPORT ENABLED
//Update Anti-Rollback support
#define UPDATE_ANTI_ROLLBACK_SUPPORT DISABLED
//Update Fallback support
#define UPDATE_FALLBACK_SUPPORT ENABLED

//Cipher support
#define CIPHER_SUPPORT ENABLED
//Image input encrypted
#define IMAGE_INPUT_ENCRYPTED ENABLED
//Verification Integrity support
#define VERIFY_INTEGRITY_SUPPORT ENABLED
//Verification Signture support
#define VERIFY_SIGNATURE_SUPPORT ENABLED
//Verification RSA signture algo support
#define VERIFY_RSA_SUPPORT ENABLED


#endif //_BOOT_CONFIG_H
