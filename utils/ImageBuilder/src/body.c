/**
 * @file body.c
 * @brief Generate the body section of an update image
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

#include <stdio.h>
#include "main.h"
#include "utils.h"
#include "header.h"
#include "body.h"

char *cipher_input = NULL;
size_t cipher_input_size = 0;

/**
 * @brief Make the image body
 * @param[in] header Pointer to the image header
 * @param[in] body Pointer to the image body
 * @param[in] CipherInfo Crypto related settings for cipher operations
 * @return Status code
 **/
int bodyMake(ImageHeader *header, ImageBody *body, CipherInfo cipherInfo) {
    error_t status = NO_ERROR;

    // Encrypt the image if necessary
    if(cipherInfo.cipherKey != NULL) {
        cipher_input = malloc(blockified_padding_and_input_binary_size); // TODO: free(cipher_input_binary) after writing to disk.
        memcpy(cipher_input, blockified_padding_and_input_binary, blockified_padding_and_input_binary_size);

        status = encrypt(blockified_padding_and_input_binary,
                         blockified_padding_and_input_binary_size,
                         cipher_input,
                         cipherInfo);

        if(status) {
            printf("bodyMake: failed to encrypt input binary.\n");
            return EXIT_FAILURE;
        }

        cipher_input_size = blockified_padding_and_input_binary_size;
        header->dataSize = cipher_input_size;

        body->binary = cipher_input;
        body->binarySize = cipher_input_size;
    } else {
        body->binary = (uint8_t *)padding_and_input_binary;
        body->binarySize = padding_and_input_binary_size;
    }

    return EXIT_SUCCESS;
}
