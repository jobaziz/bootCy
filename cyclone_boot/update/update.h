/**
 * @file update.h
 * @brief CycloneBOOT IAP User API
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

#ifndef _UPDATE_H
#define _UPDATE_H

//Dependencies
#include <stdint.h>
#include "boot_config.h"
#include "core/flash.h"
#include "memory/memory.h"
#include "image/image.h"
#include "core/crypto.h"
#include "hash/hash_algorithms.h"
#include "core/crc32.h"
#if ((CIPHER_SUPPORT == ENABLED) && ((IMAGE_INPUT_ENCRYPTED == ENABLED) || (IMAGE_OUTPUT_ENCRYPTED == ENABLED)))
#include "security/cipher.h"
#endif
#include "security/verify.h"
#include "error.h"
#include "core/cboot_error.h"

//CycloneBOOT Version string
#define CYCLONE_BOOT_UPDATE_VERSION_STRING "2.1.0"
//CycloneBOOT Major version
#define CYCLONE_BOOT_UPDATE_MAJOR_VERSION 2
//CycloneBOOT Minor version
#define CYCLONE_BOOT_UPDATE_MINOR_VERSION 1
//CycloneBOOT Revision number
#define CYCLONE_BOOT_UPDATE_REV_NUMBER 0

#define IMAGE_MAX_CHECK_DATA_SIZE 512

//Update single bank internal flash memory mode support
#ifndef UPDATE_SINGLE_BANK_SUPPORT
#define UPDATE_SINGLE_BANK_SUPPORT DISABLED
#elif (UPDATE_SINGLE_BANK_SUPPORT != ENABLED && UPDATE_SINGLE_BANK_SUPPORT != DISABLED)
   #error UPDATE_SINGLE_BANK_SUPPORT parameter is not valid!
#endif

//Update dual bank internal flash memory mode support
#ifndef UPDATE_DUAL_BANK_SUPPORT
#define UPDATE_DUAL_BANK_SUPPORT ENABLED
#elif (UPDATE_DUAL_BANK_SUPPORT != ENABLED && UPDATE_DUAL_BANK_SUPPORT != DISABLED)
   #error UPDATE_DUAL_BANK_SUPPORT parameter is not valid!
#endif

//Update fallback support
#ifndef UPDATE_FALLBACK_SUPPORT
#define UPDATE_FALLBACK_SUPPORT DISABLED
#elif (UPDATE_FALLBACK_SUPPORT != ENABLED && UPDATE_FALLBACK_SUPPORT != DISABLED)
   #error UPDATE_FALLBACK_SUPPORT parameter is not valid!
#endif

//Update anti-rollback support
#ifndef UPDATE_ANTI_ROLLBACK_SUPPORT
#define UPDATE_ANTI_ROLLBACK_SUPPORT DISABLED
#elif (UPDATE_ANTI_ROLLBACK_SUPPORT != ENABLED && UPDATE_ANTI_ROLLBACK_SUPPORT != DISABLED)
   #error UPDATE_ANTI_ROLLBACK_SUPPORT parameter is not valid!
#endif

//Acceptable internal memory mode
#if ((UPDATE_SINGLE_BANK_SUPPORT == ENABLED && UPDATE_DUAL_BANK_SUPPORT == ENABLED) || \
(UPDATE_SINGLE_BANK_SUPPORT == DISABLED && UPDATE_DUAL_BANK_SUPPORT == DISABLED))
   #error Exactly one of the following parameters MUST be enabled: \
UPDATE_SINGLE_BANK_SUPPORT - \
UPDATE_DUAL_BANK_SUPPORT
#endif

//Acceptable encryption of the output image activation
#if (((CIPHER_SUPPORT == ENABLED) && (IMAGE_OUTPUT_ENCRYPTED == ENABLED)) && \
   (UPDATE_SINGLE_BANK_SUPPORT == DISABLED))
#error Encryption of the output image is available only in Singel Bank mode!
#endif

//Add update encryption related dependencies
#if ((CIPHER_SUPPORT == ENABLED) && ((IMAGE_INPUT_ENCRYPTED == ENABLED) || \
    (IMAGE_OUTPUT_ENCRYPTED == ENABLED)))
#include "security/cipher.h"
#endif

#if (UPDATE_SINGLE_BANK_SUPPORT == ENABLED)
#if (VERIFY_INTEGRITY_SUPPORT == DISABLED)
//Force activation of VERIFY_INTEGRITY_SUPPORT when UPDATE_SINGLE_BANK_SUPPORT is activated.
#error Integrity support (VERIFY_INTEGRITY_SUPPORT) MUST be enabled when using CycloneBOOT in Single Bank Mode!
#endif
#endif

//Forward declaration of UpdateContext structure
struct _UpdateContext;
#define UpdateContext struct _UpdateContext

/**
 * @brief Random data generation callback function
 **/

typedef cboot_error_t (*IapRandCallback)(uint8_t *data, size_t length);


/**
 *@brief Supported authentication algorithms
 **/

typedef enum
{
   UPDATE_AUTH_NONE,
   UPDATE_AUTH_HMAC
} UpdateAuthAlgo;


/**
 *@brief Supported signature algorithms
 **/

typedef enum
{
   UPDATE_SIGN_NONE,
   UPDATE_SIGN_RSA,
   UPDATE_SIGN_ECDSA
} UpdateSignAlgo;


/**
 * @brief Update Crypto settings
 **/

typedef struct
{
#if ((CIPHER_SUPPORT == ENABLED) && ((IMAGE_INPUT_ENCRYPTED == ENABLED) || (IMAGE_OUTPUT_ENCRYPTED == ENABLED)))
   const CipherAlgo *cipherAlgo;                   ///<Image cipher algorithm
   CipherMode cipherMode;                          ///<Image cipher mode
   const char_t *cipherKey;                        ///<Image cipher key
   size_t cipherKeyLen;                            ///<Image cipher key size
#endif
   VerifySettings verifySettings;                 ///<Various crypto settings for image verification
} UpdateCryptoSettings;


/**
 * @brief Update user settings
 **/

typedef struct
{
#if (UPDATE_ANTI_ROLLBACK_SUPPORT == ENABLED)
   uint32_t appVersion;                         ///<Version of the current running application
#endif
   UpdateCryptoSettings imageInCrypto;          ///<Cryptographic settings used to manage an update image
#if (UPDATE_SINGLE_BANK_SUPPORT == ENABLED)
   UpdateCryptoSettings imageOutCrypto;         ///<Cryptographic settings used to generate the update image
#if (IMAGE_OUTPUT_ENCRYPTED == ENABLED)
   const char_t *psk;                           ///<PSK key used to encrypt the output image
   uint32_t pskSize;                            ///<Size of the PSK key used to encrypt the output image
#endif
#endif
   Memory memories[NB_MEMORIES];
}UpdateSettings;


/**
 * @brief Update states
 **/

typedef enum
{
   UPDATE_STATE_IDLE,
   UPDATE_STATE_RECV_APP_HEADER,
   UPDATE_STATE_RECV_APP_DATA,
   UPDATE_STATE_RECV_APP_CHECK,
   UPDATE_STATE_VALIDATE_APP,
   UPDATE_STATE_APP_REBOOT,
   UPDATE_STATE_WRITE_APP_INIT,
   UPDATE_STATE_WRITE_APP_HEADER,
   UPDATE_STATE_WRITE_APP_DATA,
   UPDATE_STATE_WRITE_APP_CHECK,
   UPDATE_STATE_WRITE_APP_END
} UpdateState;

/**
 * @brief Update context
 **/

struct _UpdateContext
{
   UpdateSettings settings;      ///<Update user settings
   Memory memories[NB_MEMORIES];
   ImageProcessContext imageProcessCtx;
};

//CycloneBOOT Update application related functions
void updateGetDefaultSettings(UpdateSettings *settings);
char_t *updateGetVersion(void);
cboot_error_t updateRegisterRandCallback(IapRandCallback callback);

cboot_error_t updateInit(UpdateContext *context, UpdateSettings *settings);
cboot_error_t updateProcess(UpdateContext *context, const void *data, size_t length);
cboot_error_t updateFinalize(UpdateContext *context);
cboot_error_t updateReboot(UpdateContext *context);

//Extern device MCU related function
extern uint32_t mcuGetVtorOffset(void);
extern void mcuSystemReset(void);
extern void mcuJumpToApplication(uint32_t address);

//Random data generation callback function
extern IapRandCallback updateRandCallback;

#endif // !_UPDATE_H
