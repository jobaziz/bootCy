/**
 * @file sam4l_crypto_cipher.h
 * @brief SAM4L cipher hardware accelerator
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCRYPTO Eval.
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
 * @version 2.3.2
 **/

#ifndef _SAM4L_CRYPTO_CIPHER_H
#define _SAM4L_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef SAM4L_CRYPTO_CIPHER_SUPPORT
   #define SAM4L_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (SAM4L_CRYPTO_CIPHER_SUPPORT != ENABLED && SAM4L_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error SAM4L_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//CFBS bitfield
#define AESA_MODE_CFBS_128BIT AESA_MODE_CFBS(0)
#define AESA_MODE_CFBS_64BIT  AESA_MODE_CFBS(1)
#define AESA_MODE_CFBS_32BIT  AESA_MODE_CFBS(2)
#define AESA_MODE_CFBS_16BIT  AESA_MODE_CFBS(3)
#define AESA_MODE_CFBS_8BIT   AESA_MODE_CFBS(4)

//OPMODE bitfield
#define AESA_MODE_OPMODE_ECB  AESA_MODE_OPMODE(0)
#define AESA_MODE_OPMODE_CBC  AESA_MODE_OPMODE(1)
#define AESA_MODE_OPMODE_CFB  AESA_MODE_OPMODE(2)
#define AESA_MODE_OPMODE_OFB  AESA_MODE_OPMODE(3)
#define AESA_MODE_OPMODE_CTR  AESA_MODE_OPMODE(4)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
