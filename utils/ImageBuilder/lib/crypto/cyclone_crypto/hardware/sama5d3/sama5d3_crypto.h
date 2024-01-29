/**
 * @file sama5d3_crypto.h
 * @brief SAMA5D3 hardware cryptographic accelerator
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
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

#ifndef _SAMA5D3_CRYPTO_H
#define _SAMA5D3_CRYPTO_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Global variables
extern OsMutex sama5d3CryptoMutex;

//SAMA5D3 hardware cryptographic accelerator related functions
error_t sama5d3CryptoInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
