/**
 * @file body.h
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

#ifndef __BODY_H
#define __BODY_H

#include <stdint.h>
#include "header.h"
#include "main.h"

typedef struct
{
    uint8_t* binary;        // pointer to the buffer containing the firmware binary (and any associated padding)
    size_t binarySize;      // size of the firmware binary
    uint8_t* checkData;     // pointer to the buffer containing image verification data
    size_t checkDataSize;   // image verification data buffer length
} ImageBody;

// Function to generate the update image body containing the firmware binary
int bodyMake(ImageHeader *header, ImageBody *body, CipherInfo cipherInfo);

#endif // __BODY_H
