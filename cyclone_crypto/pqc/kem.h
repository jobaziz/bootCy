/**
 * @file kem.h
 * @brief Key encapsulation mechanism (KEM)
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

#ifndef _KEM_H
#define _KEM_H

//Dependencies
#include "core/crypto.h"

//Streamlined NTRU Prime 761 KEM supported?
#if (SNTRUP761_SUPPORT == ENABLED)
   #include "pqc/sntrup761.h"
#endif

//Kyber-512 KEM supported?
#if (KYBER512_SUPPORT == ENABLED)
   #include "pqc/kyber512.h"
#endif

//Kyber-768 KEM supported?
#if (KYBER768_SUPPORT == ENABLED)
   #include "pqc/kyber768.h"
#endif

//Kyber-1024 KEM supported?
#if (KYBER1024_SUPPORT == ENABLED)
   #include "pqc/kyber1024.h"
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief KEM context
 **/

typedef struct
{
   const KemAlgo *kemAlgo; ///<Key encapsulation mechanism
   uint8_t *sk;            ///<Secret key
   uint8_t *pk;            ///<Public key
} KemContext;


//KEM related functions
void kemInit(KemContext *context, const KemAlgo *kemAlgo);
void kemFree(KemContext *context);

error_t kemGenerateKeyPair(KemContext *context, const PrngAlgo *prngAlgo,
   void *prngContext);

error_t kemLoadPublicKey(KemContext *context, const uint8_t *pk);

error_t kemEncapsulate(KemContext *context, const PrngAlgo *prngAlgo,
   void *prngContext, uint8_t *ct, uint8_t *ss);

error_t kemDecapsulate(KemContext *context, const uint8_t *ct, uint8_t *ss);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
