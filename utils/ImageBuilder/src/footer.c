/**
 * @file footer.c
 * @brief Generate the footer section of an image
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
#include <stdlib.h>
#include "crc32.h"
#include "hash/md5.h"
#include "hash/sha1.h"
#include "hash/sha256.h"
#include "hash/sha512.h"
#include "mac/hmac.h"
#include "main.h"
#include "inc/header.h"
#include "inc/body.h"
#include "inc/footer.h"

/**
 * @brief Make the image footer
 * @param[in] header Pointer to the image header
 * @param[in] body Pointer to the image body
 * @param[in] CipherInfo Crypto related settings for cipher operations
 * @param[in] checkDataInfo Crypto related settings for image verification operations
 * @param[in] check_data Buffer containing the check data section of the update image
 * @return Status code
 **/
int footerMake(ImageHeader *header, ImageBody *body, CipherInfo *cipherInfo, CheckDataInfo *checkDataInfo, char* check_data) {
    size_t checkDataContentsSize;
    error_t status;
    size_t check_data_len;
    char *checkDataContents;
    HashAlgo *hash_algo;

    printf("Computing application image check data tag...\n");

    // the buffer to calculate image verification data over will depend on if the image is encrypted or not.
    // if the image is encrypted, image verification data will contain the following sections:
    // headerCRC + initialization_vector + binary (padding and binary, more precisely)
    // if it is not encrypted, everything as above except the initialization_vector
    if(cipherInfo->cipherKey != NULL) {
        checkDataContentsSize = CRC32_DIGEST_SIZE + cipherInfo->ivSize + body->binarySize;
        checkDataContents = malloc(checkDataContentsSize);

        memcpy(checkDataContents, header->headCrc, CRC32_DIGEST_SIZE);
        memcpy(checkDataContents + CRC32_DIGEST_SIZE, cipherInfo->iv, cipherInfo->ivSize);
        memcpy(checkDataContents + CRC32_DIGEST_SIZE + cipherInfo->ivSize, body->binary, body->binarySize);
    } else {
        checkDataContentsSize = CRC32_DIGEST_SIZE + body->binarySize;
        checkDataContents = (char*)malloc(checkDataContentsSize);

        memcpy(checkDataContents,header->headCrc,CRC32_DIGEST_SIZE);
        memcpy(checkDataContents + CRC32_DIGEST_SIZE,body->binary,body->binarySize);
    }

    // Determine what sort of image verification method is utilized
    // Integrity: CRC32, MD5, SHA1, SHA256, SHA384 or SHA512
    if(checkDataInfo->integrity) {
        if(strcasecmp(checkDataInfo->integrity_algo, "crc32") == 0) {
            hash_algo = (HashAlgo *)CRC32_HASH_ALGO;
            body->checkDataSize = CRC32_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->integrity_algo, "md5") == 0) {
            hash_algo = (HashAlgo *)MD5_HASH_ALGO;
            body->checkDataSize = MD5_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->integrity_algo, "sha1") == 0) {
            hash_algo = (HashAlgo *)SHA1_HASH_ALGO;
            body->checkDataSize = SHA1_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->integrity_algo, "sha224") == 0) {
            hash_algo = (HashAlgo *)SHA224_HASH_ALGO;
            body->checkDataSize = SHA224_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->integrity_algo, "sha384") == 0) {
            hash_algo = (HashAlgo *)SHA384_HASH_ALGO;
            body->checkDataSize = SHA384_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->integrity_algo, "sha256") == 0) {
            hash_algo = (HashAlgo *)SHA256_HASH_ALGO;
            body->checkDataSize = SHA256_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->integrity_algo, "sha512") == 0) {
            hash_algo = (HashAlgo *)SHA512_HASH_ALGO;
            body->checkDataSize = SHA512_DIGEST_SIZE;
        } else {
            printf("footerMake: unknown integrity algorithm.\n");
            return EXIT_FAILURE;
        }

        status = hash_algo->compute(checkDataContents,checkDataContentsSize,(uint8_t *)check_data);
        if(status != NO_ERROR) {
            printf("footerMake: failed to calculate hash digest of check data.\n");
            return EXIT_FAILURE;
        }

    // Signature: ECDSA-SHA256 or RSA-SHA256
    } else if (checkDataInfo->signature) {

        status = sign(cipherInfo,checkDataInfo,checkDataContents,checkDataContentsSize,&check_data, &check_data_len);
        body->checkDataSize = check_data_len;
        if(status != NO_ERROR) {
            printf("footerMake: failed to sign the binary (check_data field).\n");
            return EXIT_FAILURE;
        }

    // Authentication: HMAC-MD5, HMAC-SHA256, HMAC-SHA512
    } else if (checkDataInfo->authentication) {
        if(strcasecmp(checkDataInfo->auth_algo, "hmac-md5") == 0) {
            hash_algo = (HashAlgo*)MD5_HASH_ALGO;
            body->checkDataSize = MD5_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->auth_algo, "hmac-sha256") == 0) {
            hash_algo = (HashAlgo *)SHA256_HASH_ALGO;
            body->checkDataSize = SHA256_DIGEST_SIZE;
        } else if(strcasecmp(checkDataInfo->auth_algo, "hmac-sha512") == 0) {
            hash_algo = (HashAlgo *)SHA512_HASH_ALGO;
            body->checkDataSize = SHA512_DIGEST_SIZE;
        } else {
            printf("footerMake: unknown authentication algorithm.\n");
            return EXIT_FAILURE;
        }

        status = hmacCompute(hash_algo,checkDataInfo->authKey,checkDataInfo->authKeySize,
                            checkDataContents,checkDataContentsSize,(uint8_t *)check_data);

        if(status != NO_ERROR) {
            printf("footerMake: failed to calculate application authentication tag.\n");
            return EXIT_FAILURE;
        }

        free(checkDataContents);

    // Default check data method : CRC32
    } else {
        hash_algo = ( HashAlgo *)CRC32_HASH_ALGO;
        status = hash_algo->compute(checkDataContents,checkDataContentsSize,(uint8_t *)check_data);
        body->checkDataSize = CRC32_DIGEST_SIZE;

        if(status != NO_ERROR) {
            printf("footerMake: failed to calculate CRC32 digest of check data.\n");
            free(checkDataContents);
            return EXIT_FAILURE;
        }

        free(checkDataContents);
    }

    // associate the image verification data buffer (check_data) to image body
    body->checkData = (uint8_t *)check_data;

    return EXIT_SUCCESS;
}
