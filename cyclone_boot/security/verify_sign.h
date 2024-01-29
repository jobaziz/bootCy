/**
 * @file verify_sign.h
 * @brief CycloneBOOT Encryption & Decryption handling
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

#ifndef _VERIFY_SIGN_H
#define _VERIFY_SIGN_H

// Dependencies
#include "security/verify.h"

//CycloneBOOT signature verification related functions
cboot_error_t signInit(VerifyContext *context);
cboot_error_t signVerify(VerifyContext *context, uint8_t *verifyData, size_t verifyDataLength);

#endif // !_VERIFY_AUTH_H
