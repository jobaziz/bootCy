/**
 * @file kyber1024.h
 * @brief Kyber-1024 KEM
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

#ifndef _KYBER1024_H
#define _KYBER1024_H

//Dependencies
#include "core/crypto.h"

//Public key length
#define KYBER1024_PUBLIC_KEY_LEN 1568
//Secret key length
#define KYBER1024_SECRET_KEY_LEN 3168
//Ciphertext length
#define KYBER1024_CIPHERTEXT_LEN 1568
//Shared secret length
#define KYBER1024_SHARED_SECRET_LEN 32

//Common interface for key encapsulation mechanisms (KEM)
#define KYBER1024_KEM_ALGO (&kyber1024KemAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Kyber-1024 related constants
extern const KemAlgo kyber1024KemAlgo;

//Kyber-1024 related functions
error_t kyber1024GenerateKeyPair(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *pk, uint8_t *sk);

error_t kyber1024Encapsulate(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *ct, uint8_t *ss, const uint8_t *pk);

error_t kyber1024Decapsulate(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
