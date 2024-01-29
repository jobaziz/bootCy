/**
 * @file footer.h
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

#ifndef __FOOTER_H
#define __FOOTER_H

#include <string.h>
#include "body.h"
#include "utils.h"

// Function to generate the footer section of the update image using the header and body data
int footerMake(ImageHeader *header, ImageBody *body, CipherInfo *cipherInfo, CheckDataInfo *checkDataInfo, char* check_data);

#endif // __FOOTER_H
