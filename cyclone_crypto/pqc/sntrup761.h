/**
 * @file sntrup761.h
 * @brief Streamlined NTRU Prime 761 KEM
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

#ifndef _SNTRUP761_H
#define _SNTRUP761_H

//Dependencies
#include "core/crypto.h"

//Public key length
#define SNTRUP761_PUBLIC_KEY_LEN 1158
//Secret key length
#define SNTRUP761_SECRET_KEY_LEN 1763
//Ciphertext length
#define SNTRUP761_CIPHERTEXT_LEN 1039
//Shared secret length
#define SNTRUP761_SHARED_SECRET_LEN 32

//Common interface for key encapsulation mechanisms (KEM)
#define SNTRUP761_KEM_ALGO (&sntrup761KemAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Streamlined NTRU Prime 761 related constants
extern const KemAlgo sntrup761KemAlgo;

//Streamlined NTRU Prime 761 related functions
error_t sntrup761GenerateKeyPair(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *pk, uint8_t *sk);

error_t sntrup761Encapsulate(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *ct, uint8_t *ss, const uint8_t *pk);

error_t sntrup761Decapsulate(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
