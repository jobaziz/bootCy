/**
 * @file sign_algorithms.h
 * @brief Collection of signature algorithms
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

#ifndef _SIGN_ALGORITHMS_H
#define _SIGN_ALGORITHMS_H

//Dependencies
#include "core/crypto.h"

//RSA support?
#if (RSA_SUPPORT == ENABLED)
   #include "pkc/rsa.h"
#endif

//DSA support?
#if (DSA_SUPPORT == ENABLED)
   #include "pkc/dsa.h"
#endif

//ECDSA support?
#if (ECDSA_SUPPORT == ENABLED)
   #include "ecc/ecdsa.h"
#endif

//SM2 support?
#if (SM2_SUPPORT == ENABLED)
   #include "ecc/sm2.h"
#endif

//EdDSA support?
#if (ED25519_SUPPORT == ENABLED || ED448_SUPPORT == ENABLED)
   #include "ecc/eddsa.h"
#endif

#endif
