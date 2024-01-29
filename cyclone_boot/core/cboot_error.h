/**
 * @file error.h
 * @brief CycloneBOOT Error codes description
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

#ifndef _CBOOT_ERROR_H
#define _CBOOT_ERROR_H

//Dependencies
#include "error.h"

// C++ guard
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Error codes
 **/

typedef enum
{

   CBOOT_NO_ERROR,
   CBOOT_ERROR_FAILURE,
   CBOOT_ERROR_NOT_IMPLEMENTED,
   CBOOT_ERROR_ABORTED,
   CBOOT_ERROR_INVALID_PARAMETERS,
   CBOOT_ERROR_INVALID_ADDRESS,
   CBOOT_ERROR_INVALID_VALUE,
   CBOOT_ERROR_INVALID_IMAGE_HEADER,
   CBOOT_ERROR_INVALID_IMAGE_HEADER_VERSION,
   CBOOT_ERROR_INVALID_HEADER_APP_TYPE,
   CBOOT_ERROR_INVALID_IMAGE_CHECK,
   CBOOT_ERROR_MISSING_IMAGE_CHECK_METHOD,
   CBOOT_ERROR_INVALID_IMAGE_VERIFY_METHOD,
   CBOOT_ERROR_INCOMPATIBLE_IMAGE_APP_VERSION,
   CBOOT_ERROR_INCORRECT_IMAGE_APP_VERSION,
   CBOOT_ERROR_INVALID_IMAGE_INTEGRITY_TAG,
   CBOOT_ERROR_INVALID_IMAGE_AUTHENTICATION_TAG,
   CBOOT_ERROR_INVALID_IMAGE_APP,
   CBOOT_ERROR_IMAGE_NOT_READY,
   CBOOT_ERROR_SLOTS_OVERLAP,
   CBOOT_ERROR_UNKNOWN_SLOT_TYPE,
   CBOOT_ERROR_INVALID_STATE,
   CBOOT_ERROR_BUFFER_OVERFLOW,
   CBOOT_ERROR_INVALID_LENGTH,
   CBOOT_ERROR_MEMORY_DRIVER_OPEN_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_CLOSE_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_INIT_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_DEINIT_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_WRITE_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_READ_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_ERASE_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_GET_STATUS_FAILED,
   CBOOT_ERROR_MEMORY_DRIVER_GET_INFO_FAILED,
   CBOOT_ERROR_UNSUPPORTED_AUTH_ALGO,
   CBOOT_ERROR_UNSUPPORTED_SIGNATURE_ALGO,
   CBOOT_ERROR_UNSUPPORTED_CIPHER_ALGO,
   CBOOT_ERROR_UNSUPPORTED_CIPHER_MODE,
   CBOOT_ERROR_UNKNOWN_MEMORY_TYPE,

} cboot_error_t;

// C++ guard
#ifdef __cplusplus
}
#endif

#endif
