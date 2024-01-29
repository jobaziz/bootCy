/**
 * @file mimxrt1040_crypto.h
 * @brief i.MX RT1040 hardware cryptographic accelerator (DCP)
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

#ifndef _MIMXRT1040_CRYPTO_H
#define _MIMXRT1040_CRYPTO_H

//Dependencies
#include "core/crypto.h"

//DCP buffer size
#ifndef MIMXRT1040_DCP_BUFFER_SIZE
   #define MIMXRT1040_DCP_BUFFER_SIZE 1024
#elif (MIMXRT1040_DCP_BUFFER_SIZE < 256)
   #error MIMXRT1040_DCP_BUFFER_SIZE parameter is not valid
#endif

//Name of the section where to place DCP buffers
#ifndef MIMXRT1040_DCP_RAM_SECTION
   #define MIMXRT1040_DCP_RAM_SECTION ".dcp_ram"
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Global variables
extern OsMutex mimxrt1040CryptoMutex;

//i.MX RT1040 hardware cryptographic accelerator related functions
error_t mimxrt1040CryptoInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
