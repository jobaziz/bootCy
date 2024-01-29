/**
 * @file version.h
 * @brief Application version file
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

#ifndef _VERSION_H
#define _VERSION_H

//Major version
#define APP_MAJOR_VERSION 1
//Minor version
#define APP_MINOR_VERSION 0
//Revision number
#define APP_REV_NUMBER 0
//Version string
#define APP_VERSION_STRING "1.0.0"

//Version
#define APP_VERSION (uint32_t)(((APP_MAJOR_VERSION & 0xFF) << 16) | ((APP_MINOR_VERSION & 0xFF) << 8) | (APP_REV_NUMBER & 0xFF))

#endif //!_VERSION_H
