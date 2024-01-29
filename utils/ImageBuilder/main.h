/**
 * @file main.h
 * @brief CycloneBOOT compatible Update Image Builder
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

#ifndef __MAIN_H
#define __MAIN_H

#include "crypto.h"
#include "rng/yarrow.h"

#define SEED_LENGTH 32         // length of Crypto seed
#define CHECK_DATA_LENGTH 256  // length of check data field
#define INIT_VECTOR_LENGTH 16  // length of initialization vector for AES-CBC

/**
 * Stores the information about encryption operations.
 * Encryption Key, Initialization Vector (IV), Size of IV and Size of Encryption Key.
*/
typedef struct {
    const char* iv;
    size_t ivSize;
    const char* cipherKey;
    size_t cipherKeySize;
    PrngAlgo *prngAlgo;
    YarrowContext *yarrowContext;
    char seed[SEED_LENGTH];
} CipherInfo;

/**
 * Stores the information about application check data field.
*/
typedef struct {
    int signature;
    int authentication;
    int integrity;
    const char* auth_algo;
    const char* integrity_algo;
    const char* authKey;
    size_t authKeySize;
    const char* sign_algo;
    const char *signKey;
    size_t signKeySize;
    const HashAlgo *signHashAlgo;
} CheckDataInfo;

// Global variables
extern char *input_binary;
extern size_t input_binary_size;
extern char *blockified_padding_and_input_binary;
extern size_t blockified_padding_and_input_binary_size;
extern char *cipher_input;
extern size_t cipher_input_size;
extern char *padding_and_input_binary;
extern uint32_t padding_and_input_binary_size;

#endif // __MAIN_H
