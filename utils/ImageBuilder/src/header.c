/**
 * @file header.c
 * @brief Generate the header section of an update image
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
#include "core/crypto.h"
#include "crc32.h"
#include "header.h"
#include "utils.h"
#include "ImageBuilderConfig.h"

// Global variables
char *input_binary = NULL;
size_t input_binary_size = 0;
char *blockified_padding_and_input_binary = NULL;
size_t blockified_padding_and_input_binary_size = 0;

char *padding_and_input_binary = NULL;
uint32_t padding_and_input_binary_size = 0;

/**
 * @brief Make the image header
 * @param[in] header Pointer to the image header
 * @param[in] input_binary_path Path of the binary file used to generate the image
 * @param[in] imgIdx Index of the image (used to keep track of the most recent image)
 * @param[in] firmware_version Firmware version of the binary file
 * @param[in] vtor_align Amount of padding to be inserted between the header and binary
 * @param[in] img_encrypted Flag to indicate if the supplied image should be encrypted
 * @return Status code
 **/

int headerMake(ImageHeader *header, const char *input_binary_path, int imgIdx, const char* firmware_version, uint32_t vtor_align, int img_encrypted) {
    size_t headerDataSize;
    int headerVersion;

    char *_firmware_version;
    uint8_t  majorVersion;
    uint8_t minorVersion;
    uint8_t patchVersion;

    HashAlgo const *crc32_algo;

    // Generate the header version
    headerVersion = VERSION_32_BITS(image_builder_HEADER_VERSION_MAJOR,image_builder_HEADER_VERSION_MINOR,image_builder_HEADER_VERSION_PATCH);
    // Choose header section integrity algorithm
    crc32_algo = (HashAlgo *)CRC32_HASH_ALGO;

    // Read the binary file from the disk
    int status = read_file(input_binary_path,&input_binary,&input_binary_size);

    if(status) {
        printf("headerMake: failed to open input binary file.\n");
        return EXIT_FAILURE;
    }

    //Computing padding according given vtor align value
    if((vtor_align == 0) || (vtor_align == sizeof(ImageHeader)))
    {
        //No padding
        header->dataPadding = 0;
    }
    else
    {
        header->dataPadding = vtor_align - (sizeof(ImageHeader) % vtor_align);
        //Debug message
        printf("Applying %d bytes padding between header and binary...\r\n",header->dataPadding);
    }

    // Make a buffer big enough to keep the padding (if supplied, otherwise 0) and the binary file
    padding_and_input_binary_size = input_binary_size + header->dataPadding;
    padding_and_input_binary = malloc(padding_and_input_binary_size);
    // Set the buffer to zero and copy the padding and the input binary (respectively) to the buffer
    memset(padding_and_input_binary,0,padding_and_input_binary_size);
    memcpy(padding_and_input_binary + header->dataPadding, input_binary, input_binary_size);
    // Calculate the size of the data (padding + binary size)
    headerDataSize = input_binary_size + header->dataPadding;

    // If the image should be encrypted, it must be further divided into block of 16-bytes each
    // this is the size of data an algorithm like AES-CBC expects to work on.
    if(img_encrypted) {
        status = blockify(16, padding_and_input_binary, padding_and_input_binary_size,
                          &blockified_padding_and_input_binary, &blockified_padding_and_input_binary_size);

        if(status) {
            printf("headerMake: failed to blockify input binary file.\n");
            return EXIT_FAILURE;
        }

        headerDataSize = blockified_padding_and_input_binary_size;
    }

    // Fill-in the rest of the fields of header
    header->dataSize = headerDataSize;
    header->headVers = headerVersion;
    header->imgType = IMG_TYPE_APP;
    header->imgIndex = imgIdx;

    //Parse received firmware "string" version
    majorVersion = (uint8_t)strtol(firmware_version, &_firmware_version, 10);
    minorVersion = (uint8_t)strtol(++_firmware_version, &_firmware_version, 10);
    patchVersion = (uint8_t)strtol(++_firmware_version, &_firmware_version, 10);

    header->dataVers = VERSION_32_BITS(majorVersion,minorVersion,patchVersion);
    header->imgTime = time(NULL);

    memset(header->reserved, 0, sizeof(header->reserved));

    // Calculate the CRC of the header
    crc32_algo->compute(header,sizeof(ImageHeader) - CRC32_DIGEST_SIZE, header->headCrc);

    return EXIT_SUCCESS;
}
