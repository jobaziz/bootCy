/**
 * @file pkcs8_key_format.h
 * @brief PKCS #8 key formatting
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCRYPTO Eval.
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
 * @version 2.3.2
 **/

#ifndef _PKCS8_KEY_FORMAT_H
#define _PKCS8_KEY_FORMAT_H

//Dependencies
#include "core/crypto.h"
#include "pkc/rsa.h"
#include "pkc/dsa.h"
#include "ecc/ec.h"
#include "ecc/eddsa.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Key formatting functions
error_t pkcs8FormatRsaPrivateKey(const RsaPrivateKey *privateKey,
   uint8_t *output, size_t *written);

error_t pkcs8FormatDsaPrivateKey(const DsaPrivateKey *privateKey,
   uint8_t *output, size_t *written);

error_t pkcs8FormatEcPrivateKey(const EcCurveInfo *curveInfo,
   const EcPrivateKey *privateKey, const EcPublicKey *publicKey,
   uint8_t *output, size_t *written);

error_t pkcs8FormatEcPublicKey(const EcCurveInfo *curveInfo,
   const EcPublicKey *publicKey, uint8_t *output, size_t *written);

error_t pkcs8FormatEddsaPrivateKey(const EcCurveInfo *curveInfo,
   const EddsaPrivateKey *privateKey, uint8_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
