/**
 * @file sam4c_crypto_hash.h
 * @brief SAM4C hash hardware accelerator
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

#ifndef _SAM4C_CRYPTO_HASH_H
#define _SAM4C_CRYPTO_HASH_H

//Dependencies
#include "core/crypto.h"

//Hash hardware accelerator
#ifndef SAM4C_CRYPTO_HASH_SUPPORT
   #define SAM4C_CRYPTO_HASH_SUPPORT DISABLED
#elif (SAM4C_CRYPTO_HASH_SUPPORT != ENABLED && SAM4C_CRYPTO_HASH_SUPPORT != DISABLED)
   #error SAM4C_CRYPTO_HASH_SUPPORT parameter is not valid
#endif

//Hash algorithm identifiers
#define ICM_ALGO_SHA1        0
#define ICM_ALGO_SHA256      1
#define ICM_ALGO_SHA224      4

//ICM region configuration
#define ICM_RCFG_MRPROT      0x3F000000
#define ICM_RCFG_ALGO        0x00007000
#define ICM_RCFG_ALGO_SHA1   0x00000000
#define ICM_RCFG_ALGO_SHA256 0x00001000
#define ICM_RCFG_ALGO_SHA224 0x00004000
#define ICM_RCFG_PROCDLY     0x00000400
#define ICM_RCFG_SUIEN       0x00000200
#define ICM_RCFG_ECIEN       0x00000100
#define ICM_RCFG_WCIEN       0x00000080
#define ICM_RCFG_BEIEN       0x00000040
#define ICM_RCFG_DMIEN       0x00000020
#define ICM_RCFG_RHIEN       0x00000010
#define ICM_RCFG_EOM         0x00000004
#define ICM_RCFG_WRAP        0x00000002
#define ICM_RCFG_CDWBN       0x00000001

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ICM region descriptor
 **/

typedef struct
{
   uint32_t raddr; ///<ICM region start address
   uint32_t rcfg;  ///<ICM region configuration
   uint32_t rctrl; ///<ICM region control
   uint32_t rnext; ///<ICM region next address
} Sam4cIcmDesc;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
