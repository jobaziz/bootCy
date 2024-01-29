/**
 * @file pic32cx_sg_crypto_pkc.h
 * @brief PIC32CX SG41/SG60/SG61 public-key hardware accelerator (PUKCC)
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

#ifndef _PIC32CX_SG_CRYPTO_PKC_H
#define _PIC32CX_SG_CRYPTO_PKC_H

//Dependencies
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef PIC32CX_SG_CRYPTO_PKC_SUPPORT
   #define PIC32CX_SG_CRYPTO_PKC_SUPPORT DISABLED
#elif (PIC32CX_SG_CRYPTO_PKC_SUPPORT != ENABLED && PIC32CX_SG_CRYPTO_PKC_SUPPORT != DISABLED)
   #error PIC32CX_SG_CRYPTO_PKC_SUPPORT parameter is not valid
#endif

//Crypto memory base address
#define PUKCC_CRYPTO_RAM_BASE 0x02011000UL

//Far to near pointer conversion
#define PUKCC_FAR_TO_NEAR(p) ((uint16_t) ((uint32_t) (p)))

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Elliptic curve point
 **/

typedef struct
{
   uint8_t *x;
   uint8_t *y;
   uint8_t *z;
} PukccEcPoint;


/**
 * @brief Fmult service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *x;
   uint8_t *y;
   uint8_t *z;
   uint8_t *r;
} PukccFmultParams;


/**
 * @brief GCD service parameters
 **/

typedef struct
{
   uint8_t *x;
   uint8_t *y;
   uint8_t *a;
   uint8_t *z;
   uint8_t *w;
} PukccGcdParams;


/**
 * @brief RedMod service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *exp;
   uint8_t *r;
   uint8_t *x;
} PukccRedModParams;


/**
 * @brief ExpMod service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *exp;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccExpModParams;


/**
 * @brief PrimeGen service parameters
 **/

typedef struct
{
   uint8_t *n;
   uint8_t *cns;
   uint8_t *rnd;
   uint8_t *w;
   uint8_t *r;
   uint8_t *exp;
} PukccPrimeGenParams;


/**
 * @brief CRT service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *x;
   uint8_t *exp;
   uint8_t *p;
   uint8_t *q;
   uint8_t *dp;
   uint8_t *dq;
   uint8_t *r;
} PukccCrtParams;


/**
 * @brief ZpEcPointIsOnCurve service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   PukccEcPoint point;
   uint8_t *a;
   uint8_t *b;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcPointIsOnCurveParams;


/**
 * @brief ZpEcConvProjToAffine service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *k;
   PukccEcPoint point;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcConvProjToAffineParams;


/**
 * @brief ZpEccMul service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   uint8_t *k;
   PukccEcPoint point;
   uint8_t *a;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEccMulParams;


/**
 * @brief ZpEcDsaGenerate service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   PukccEcPoint basePoint;
   uint8_t *order;
   uint8_t *a;
   uint8_t *privateKey;
   uint8_t *k;
   uint8_t *h;
   uint8_t *r;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcDsaGenerateParams;


/**
 * @brief ZpEcDsaVerify service parameters
 **/

typedef struct
{
   uint8_t *mod;
   uint8_t *cns;
   PukccEcPoint basePoint;
   uint8_t *order;
   uint8_t *a;
   PukccEcPoint publicKey;
   uint8_t *h;
   uint8_t *r;
   uint8_t *s;
   uint8_t *x;
   uint8_t *w;
} PukccZpEcDsaVerifyParams;


//PUKCC related functions
error_t pukccInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
