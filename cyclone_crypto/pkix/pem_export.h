/**
 * @file pem_export.h
 * @brief PEM file export functions
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

#ifndef _PEM_EXPORT_H
#define _PEM_EXPORT_H

//Dependencies
#include "core/crypto.h"
#include "pkix/pem_common.h"
#include "pkc/dh.h"
#include "pkc/rsa.h"
#include "pkc/dsa.h"
#include "ecc/ec.h"
#include "ecc/eddsa.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//PEM related functions
error_t pemExportCertificate(const uint8_t *cert, size_t certLen,
   char_t *output, size_t *written);

error_t pemExportCrl(const uint8_t *crl, size_t crlLen,
   char_t *output, size_t *written);

error_t pemExportCsr(const uint8_t *csr, size_t csrLen,
   char_t *output, size_t *written);

error_t pemExportRsaPublicKey(const RsaPublicKey *publicKey,
   char_t *output, size_t *written);

error_t pemExportRsaPrivateKey(const RsaPrivateKey *privateKey,
   char_t *output, size_t *written);

error_t pemExportRsaPssPublicKey(const RsaPublicKey *publicKey,
   char_t *output, size_t *written);

error_t pemExportRsaPssPrivateKey(const RsaPrivateKey *privateKey,
   char_t *output, size_t *written);

error_t pemExportDsaPublicKey(const DsaPublicKey *publicKey,
   char_t *output, size_t *written);

error_t pemExportDsaPrivateKey(const DsaPrivateKey *privateKey,
   char_t *output, size_t *written);

error_t pemExportEcParameters(const EcCurveInfo *curveInfo,
   char_t *output, size_t *written);

error_t pemExportEcPublicKey(const EcCurveInfo *curveInfo,
   const EcPublicKey *publicKey, char_t *output, size_t *written);

error_t pemExportEcPrivateKey(const EcCurveInfo *curveInfo,
   const EcPrivateKey *privateKey, const EcPublicKey *publicKey,
   char_t *output, size_t *written);

error_t pemExportEddsaPublicKey(const EcCurveInfo *curveInfo,
   const EddsaPublicKey *publicKey, char_t *output, size_t *written);

error_t pemExportEddsaPrivateKey(const EcCurveInfo *curveInfo,
   const EddsaPrivateKey *privateKey, char_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
