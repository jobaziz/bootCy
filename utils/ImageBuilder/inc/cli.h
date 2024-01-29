/**
 * @file cli.h
 * @brief Front-end to the CLI to fetch user parameters
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

#ifndef __CLI_H
#define __CLI_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lib/cargs/include/cargs.h"

// Error code to signify a 'correct' CLI config, even when 'required' options are not present.
// For example, -v or --version and -h or --help
#define CLI_OK 2

/**
 * This is a custom project configuration structure where you can store the
 * parsed information.
 */
struct builder_cli_configuration {
    const char *input;               // Required
    const char *output;              // Required
    const char *firmware_index;      // Optional
    const char *vtor_align;          // Optional, specifies VTOR offset padding. Default value 1024 bytes.
    const char *firmware_version;    // Optional, unless using anti-rollback
    const char *encryption_algo;     // Optional, unless encryption is required. AES-CBC by default as of v1
    const char *encryption_key;      // Optional
    const char *authentication_algo; // Optional, unless authentication is required. Supported algorithms: HMAC-[md5,sha256,sha512]
    const char *authentication_key;  // Optional
    const char *signature_algo;      // Optional
    const char* signature_key;       // Optional, unless signature is required. Supported algorithms: ecdsa-sha256, rsa-sha256,
    const char* integrity_algo;      // Optional. CRC32 is chosen by default. Supported algorithms: MD5, SHA26, SHA512
    bool verbose;                    // if passed, extra output will be passed to STDOUT
    bool version;                    // if passed, CLI version will be passed to STDOUT
    bool help;                      // if passed, a help message will be passed to STDOUT
};

// function to iterate over user parameters and copy to those to a struct
int parse_options(int argc, char **argv, struct builder_cli_configuration *cli_options);

#endif // __CLI_H
