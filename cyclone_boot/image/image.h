/**
 * @file image.h
 * @brief CycloneBOOT Image managment
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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

#ifndef _IMAGE_H
#define _IMAGE_H

//Dependencies
#include "compiler_port.h"
#include "boot_config.h"
#include "core/crypto.h"
#include "memory/memory.h"
#include "security/verify.h"
#include "core/cboot_error.h"

/*
 * If you fully understand and accept the terms of the evaluation license, then
 * edit the os_port_config.h header and add the following directive:
 *
 * #define EVAL_LICENSE_TERMS_ACCEPTED
 */

#ifndef EVAL_LICENSE_TERMS_ACCEPTED
   #error Before compiling CycloneBOOT Eval, you must accept the terms of the evaluation license
#endif

//Image input encryption support
#ifndef IMAGE_INPUT_ENCRYPTED
#define IMAGE_INPUT_ENCRYPTED DISABLED
#elif (IMAGE_INTPUT_ENCRYPTED != ENABLED && IMAGE_INTPUT_ENCRYPTED != DISABLED)
   #error IMAGE_INTPUT_ENCRYPTED parameter is not valid!
#endif

//Image output encryption support (for encrypted external memory if needed)
#ifndef IMAGE_OUTPUT_ENCRYPTED
#define IMAGE_OUTPUT_ENCRYPTED DISABLED
#elif (IMAGE_OUTPUT_ENCRYPTED != ENABLED && IMAGE_OUTPUT_ENCRYPTED != DISABLED)
   #error IMAGE_OUTPUT_ENCRYPTED parameter is not valid!
#endif

//Acceptable input or output image encryption support
#if (((IMAGE_OUTPUT_ENCRYPTED == ENABLED) || (IMAGE_INTPUT_ENCRYPTED == ENABLED)) && \
   (CIPHER_SUPPORT == DISABLED))
   #error CIPHER_SUPPORT MUST be ENABLED if IMAGE_INPUT_ENCRYPTED or IMAGE_OUTPUT_ENCRYPTED is enabled!
#endif

//Add image encryption related dependencies
#if (CIPHER_SUPPORT == ENABLED) && ((IMAGE_INPUT_ENCRYPTED == ENABLED) || (IMAGE_OUTPUT_ENCRYPTED == ENABLED))
#include "security/cipher.h"
#endif

//Maximum image check data size
#define IMAGE_MAX_CHECK_DATA_SIZE 512


/**
 * @brief Image type definition
 **/

typedef enum
{
    IMAGE_TYPE_NONE,
    IMAGE_TYPE_APP
} ImageType;

/**
 * @brief Image states
 **/

typedef enum
{
    IMAGE_STATE_IDLE,
    IMAGE_STATE_RECV_APP_HEADER,
    IMAGE_STATE_RECV_APP_DATA,
    IMAGE_STATE_RECV_APP_CHECK,
    IMAGE_STATE_VALIDATE_APP,
    IMAGE_STATE_APP_REBOOT,
    IMAGE_STATE_WRITE_APP_INIT,
    IMAGE_STATE_WRITE_APP_HEADER,
    IMAGE_STATE_WRITE_APP_DATA,
    IMAGE_STATE_WRITE_APP_CHECK,
    IMAGE_STATE_WRITE_APP_END
} ImageState;


/**
 * @brief Image Anti-Rollback callback definition
 **/

typedef bool_t (*ImageAntiRollbackCallback)(uint32_t currentAppVersion, uint32_t updateAppVersion);


//Image Header Major version
#define IMAGE_HEADER_VERSION_MAJOR 1
//Image Header Minor version
#define IMAGE_HEADER_VERSION_MINOR 1
//Image Header Revision number
#define IMAGE_HEADER_VERSION_PATCH 0
//Image Header version
#define IMAGE_HEADER_VERSION (uint32_t)(((IMAGE_HEADER_VERSION_MAJOR & 0xFF) << 16) | ((IMAGE_HEADER_VERSION_MINOR & 0xFF) << 8) | (IMAGE_HEADER_VERSION_PATCH & 0xFF))


#if ((defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || \
   defined(__GNUC__) || defined(__CC_ARM) || defined(__IAR_SYSTEMS_ICC__) || \
   defined(__TASKING__) || defined(__CWCC__) || defined(__TI_ARM__))

/**
 * @brief Image header definition
 **/

typedef __packed_struct
{
    uint32_t headVers;      ///<Image header version
    uint32_t imgIndex;      ///<Image index
    uint8_t imgType;        ///<Image type
    uint32_t dataPadding;   ///<Image data padding
    uint32_t dataSize;      ///<Image data size
    uint32_t dataVers;      ///<Image data version
    uint64_t imgTime;       ///<Image data generated time
    uint8_t reserved[31];   ///<Reserved field
    uint32_t headCrc;       ///<Image header CRC32 integrity tag
} ImageHeader;

#else

#undef interface
#undef __start_packed
#define __start_packed __pragma( pack(push, 1) )
#undef __end_packed
#define __end_packed __pragma( pack(pop) )
#define __weak

/**
 * @brief Image header definition
 **/

__start_packed typedef struct
{
   uint32_t headVers;      ///<Image header version
   uint32_t imgIndex;      ///<Image index
   uint8_t imgType;        ///<Image type
   uint32_t dataPadding;   ///<Image data padding
   uint32_t dataSize;      ///<Image data size
   uint32_t dataVers;      ///<Image data version
   uint64_t imgTime;       ///<Image data generated time
   uint8_t reserved[31];   ///<Reserved field
   uint32_t headCrc;       ///<Image header CRC32 integrity tag
} ImageHeader __end_packed;

#endif


/**
 * @brief Image context definition
 **/

typedef struct
{
    uint8_t buffer[128];                              ///<Image processing buffer
    uint8_t *bufferPos;                               ///<Position in image processing buffer
    size_t bufferLen;                                 ///<Number of byte in image processing buffer

    Slot *activeSlot;                                 ///<Pointer to the slot to write the image in

    uint16_t newImageIdx;                             ///<Image index number

    uint32_t firmwareAddr;                            ///<Image firmware data write address
    size_t firmwareLength;                            ///<Image data firmware length
    uint32_t pos;                                     ///<Image current firmware data write position
    size_t written;                                   ///<Current written firmware data byte number

    ImageState state;

#if ((CIPHER_SUPPORT == ENABLED) && ((IMAGE_INPUT_ENCRYPTED == ENABLED) || (IMAGE_OUTPUT_ENCRYPTED == ENABLED)))
    CipherEngine cipherEngine;                        ///<Image cipher engine
    bool_t ivRetrieved;
#endif

    VerifyContext verifyContext;                      ///<Image verification context

    uint8_t checkData[IMAGE_MAX_CHECK_DATA_SIZE];     ///<Image check data buffer
    uint8_t *checkDataPos;                            ///<Position in image check data buffer
    size_t checkDataLen;                              ///<Current number of byte in image check data buffer
    size_t checkDataSize;                             ///<Image check data buffer size

} Image;


/**
 * @brief Image Process context definition
 **/

typedef struct
{
    Image inputImage;                                   ///<Input Image context
    Image outputImage;                                  ///<Output Image context

    uint32_t currentAppVersion;                         ///<Current Application version
    ImageAntiRollbackCallback imgAntiRollbackCallback;  ///<Anti-Rollback callback

    Memory *memories;                                   ///<Memories list
} ImageProcessContext;


//CycloneBOOT Image related functions
cboot_error_t imageCheckHeader(ImageHeader *header);
cboot_error_t imageGetHeader(uint8_t *buffer, size_t bufferLen, ImageHeader **header);

#endif //!_IMAGE_H
