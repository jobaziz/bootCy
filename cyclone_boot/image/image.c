/**
 * @file image.c
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include <stdlib.h>
#include "core/crc32.h"
#include "core/crypto.h"
#include "memory/memory.h"
#include "image.h"
#include "debug.h"


/**
 * @brief Check Internal Image header validity
 * @param[in] header Pointer to the internal image header to be checked
 * @return Status code
 **/

cboot_error_t imageCheckHeader(ImageHeader *header)
{
   error_t error;
   uint32_t computedCrc;

   //Check parameter vailidty
   if(header == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Compute image header crc
   error = CRC32_HASH_ALGO->compute((uint8_t*)header, sizeof(ImageHeader) - CRC32_DIGEST_SIZE, (uint8_t*)&computedCrc);
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to compute image header crc!\r\n");
      return CBOOT_ERROR_FAILURE;
   }

   //Check image header integrity
   if(header->headCrc != computedCrc)
      return CBOOT_ERROR_INVALID_IMAGE_HEADER;

   //Check image header version
   if(header->headVers != IMAGE_HEADER_VERSION)
      return CBOOT_ERROR_INVALID_IMAGE_HEADER_VERSION;

   //Successful process
   return CBOOT_NO_ERROR;
}


/**
 * @brief Get image header from data buffer
 * @param[in] buffer Data buffer containing the image header
 * @param[in] bufferLen Data buffer length
 * @param[out] header Pointer to the header structure to be returned
 * @return Status code
 **/

cboot_error_t imageGetHeader(uint8_t *buffer, size_t bufferLen, ImageHeader **header)
{
   cboot_error_t cerror;
   ImageHeader *tempHeader;

   //Check parameters validity
   if(buffer == NULL || bufferLen == 0 || header == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   if(bufferLen < sizeof(ImageHeader))
      return CBOOT_ERROR_INVALID_LENGTH;

   //Point to the image header
   tempHeader = (ImageHeader *)buffer;

   //Check image header
   cerror = imageCheckHeader(tempHeader);
   //Is any error?
   if(cerror)
      return cerror;

   //Save image header
   *header = tempHeader;

   //Successful process
   return CBOOT_NO_ERROR;
}
