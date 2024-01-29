/**
 * @file scrypt.h
 * @brief scrypt PBKDF (Password-Based Key Derivation Function)
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

#ifndef _SCRYPT_H
#define _SCRYPT_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//scrypt related functions
error_t scrypt(const char_t *password, const uint8_t *salt, size_t saltLen,
   uint_t n, uint_t r, uint_t p, uint8_t *dk, size_t dkLen);

void scryptRoMix(uint_t r, uint8_t *b, uint_t n, uint8_t *v, uint8_t *y);
void scryptBlockMix(uint_t r, uint8_t *b, uint8_t *y);
void scryptXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
