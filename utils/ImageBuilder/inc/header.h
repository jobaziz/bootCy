/**
 * @file header.h
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

#ifndef __HEADER_H
#define __HEADER_H

#ifdef IS_LINUX
#include <stdint-gcc.h>
#endif
#include <stdint.h>
#include <time.h>
#include "ImageBuilderConfig.h"
#include "crc32.h"

//Image Header version
#define IMAGE_HEADER_VERSION (uint32_t)(((IMAGE_HEADER_VERSION_MAJOR & 0xFF) << 16) | ((IMAGE_HEADER_VERSION_MINOR & 0xFF) << 8) | (IMAGE_HEADER_VERSION_PATCH & 0xFF))
#define VERSION_32_BITS(major, minor, patch) (uint32_t)(((major&0xFF) << 16) | ((minor&0xFF) << 8) | (patch&0xFF))

/*
 * @brief Image types
 */
typedef enum
{
    IMG_TYPE_NONE,
    IMG_TYPE_APP //<Regular firmware binary
} ImageType;

#ifdef IS_WINDOWS

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
	uint8_t headCrc[CRC32_DIGEST_SIZE];       ///<Image header CRC32 integrity tag
} ImageHeader __end_packed;

// Function to make the update image header
int headerMake(ImageHeader* header, const char* input_binary_path, int imgIdx, const char* firmware_version, uint32_t vtor_align, int img_encrypted);

#endif

#ifdef IS_LINUX
#undef __start_packed
#define __start_packed
#undef __end_packed
#define __end_packed __attribute__((__packed__))

/**
 * @brief Image header definition
 **/

typedef __start_packed struct
{
    uint32_t headVers;      ///<Image header version
    uint32_t imgIndex;      ///<Image index
    ImageType imgType;        ///<Image type
    uint32_t dataPadding;   ///<Image data padding
    uint32_t dataSize;      ///<Image data size
    uint32_t dataVers;      ///<Image data version
    uint64_t imgTime;       ///<Image data generated time
    uint8_t reserved[31];   ///<Reserved field
    uint8_t headCrc[CRC32_DIGEST_SIZE];       ///<Image header CRC32 integrity tag
} __end_packed ImageHeader;

// Function to make the update image header
int headerMake(ImageHeader* header, const char* input_binary_path, int imgIdx, const char* firmware_version, uint32_t vtor_align, int img_encrypted);

#endif

#endif
