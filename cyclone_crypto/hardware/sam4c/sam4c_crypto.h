/**
 * @file sam4c_crypto.h
 * @brief SAM4C hardware cryptographic accelerator
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

#ifndef _SAM4C_CRYPTO_H
#define _SAM4C_CRYPTO_H

//Device-specific definitions
#if defined(__SAM4C4C_0__)
   #include "sam4c4c_0.h"
#elif defined(__SAM4C4C_1__)
   #include "sam4c4c_1.h"
#elif defined(__SAM4C8C_0__)
   #include "sam4c8c_0.h"
#elif defined(__SAM4C8C_1__)
   #include "sam4c8c_1.h"
#elif defined(__SAM4C16C_0__)
   #include "sam4c16c_0.h"
#elif defined(__SAM4C16C_1__)
   #include "sam4c16c_1.h"
#elif defined(__SAM4C32C_0__)
   #include "sam4c32c_0.h"
#elif defined(__SAM4C32C_1__)
   #include "sam4c32c_1.h"
#elif defined(__SAM4C32E_0__)
   #include "sam4c32e_0.h"
#elif defined(__SAM4C32E_1__)
   #include "sam4c32e_1.h"
#elif defined(__SAM4CP16B_0__)
   #include "sam4cp16b_0.h"
#elif defined(__SAM4CP16B_1__)
   #include "sam4cp16b_1.h"
#elif defined(__SAM4CP16C_0__)
   #include "sam4cp16c_0.h"
#elif defined(__SAM4CP16C_1__)
   #include "sam4cp16c_1.h"
#elif defined(__SAM4CMP8C_0__)
   #include "sam4cmp8c_0.h"
#elif defined(__SAM4CMP8C_1__)
   #include "sam4cmp8c_1.h"
#elif defined(__SAM4CMP16C_0__)
   #include "sam4cmp16c_0.h"
#elif defined(__SAM4CMP16C_1__)
   #include "sam4cmp16c_1.h"
#elif defined(__SAM4CMS4C_0__)
   #include "sam4cms4c_0.h"
#elif defined(__SAM4CMS4C_1__)
   #include "sam4cms4c_1.h"
#elif defined(__SAM4CMS8C_0__)
   #include "sam4cms8c_0.h"
#elif defined(__SAM4CMS8C_1__)
   #include "sam4cms8c_1.h"
#elif defined(__SAM4CMS16C_0__)
   #include "sam4cms16c_0.h"
#elif defined(__SAM4CMS16C_1__)
   #include "sam4cms16c_1.h"
#elif defined(__SAM4CMP32C_0__)
   #include "sam4cmp32c_0.h"
#elif defined(__SAM4CMP32C_1__)
   #include "sam4cmp32c_1.h"
#elif defined(__SAM4CMS32C_0__)
   #include "sam4cms32c_0.h"
#elif defined(__SAM4CMS32C_1__)
   #include "sam4cms32c_1.h"
#endif

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Global variables
extern OsMutex sam4cCryptoMutex;

//SAM4C hardware cryptographic accelerator related functions
error_t sam4cCryptoInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
