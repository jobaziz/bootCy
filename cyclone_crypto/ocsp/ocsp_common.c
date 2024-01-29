/**
 * @file ocsp_common.c
 * @brief OCSP common definitions
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

//Switch to the appropriate trace level
#define TRACE_LEVEL OCSP_TRACE_LEVEL

//Dependencies
#include "ocsp/ocsp_common.h"
#include "encoding/oid.h"

//Check crypto library configuration
#if (OCSP_SUPPORT == ENABLED)

//PKIX OCSP Basic OID (1.3.6.1.5.5.7.48.1.1)
const uint8_t PKIX_OCSP_BASIC_OID[9] = {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x01};
//PKIX OCSP Nonce OID (1.3.6.1.5.5.7.48.1.2)
const uint8_t PKIX_OCSP_NONCE_OID[9] = {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x02};


/**
 * @brief Hash algorithm selection
 * @return Pointer to the preferred hash algorithm
 **/

const HashAlgo *ocspSelectHashAlgo(void)
{
   const HashAlgo *hashAlgo;

#if (OCSP_SHA1_SUPPORT == ENABLED && SHA1_SUPPORT == ENABLED)
   //Select SHA-1 hash algorithm
   hashAlgo = SHA1_HASH_ALGO;
#elif (OCSP_SHA256_SUPPORT == ENABLED && SHA256_SUPPORT == ENABLED)
   //Select SHA-256 hash algorithm
   hashAlgo = SHA256_HASH_ALGO;
#elif (OCSP_SHA384_SUPPORT == ENABLED && SHA384_SUPPORT == ENABLED)
   //Select SHA-384 hash algorithm
   hashAlgo = SHA384_HASH_ALGO;
#elif (OCSP_SHA512_SUPPORT == ENABLED && SHA512_SUPPORT == ENABLED)
   //Select SHA-512 hash algorithm
   hashAlgo = SHA512_HASH_ALGO;
#else
   //Just for sanity
   hashAlgo = NULL;
#endif

   //Return the preferred hash algorithm
   return hashAlgo;
}


/**
 * @brief Get the hash algorithm that matches the specified identifier
 * @param[in] oid Hash algorithm OID
 * @param[in] length Length of the hash algorithm OID, in bytes
 * @return Pointer to the hash algorithm
 **/

const HashAlgo *ocspGetHashAlgo(const uint8_t *oid, size_t length)
{
   const HashAlgo *hashAlgo;

#if (OCSP_SHA1_SUPPORT == ENABLED && SHA1_SUPPORT == ENABLED)
   //SHA-1 hash algorithm identifier?
   if(!oidComp(oid, length, SHA1_OID, sizeof(SHA1_OID)))
   {
      hashAlgo = SHA1_HASH_ALGO;
   }
   else
#endif
#if (OCSP_SHA256_SUPPORT == ENABLED && SHA256_SUPPORT == ENABLED)
   //SHA-256 hash algorithm identifier?
   if(!oidComp(oid, length, SHA256_OID, sizeof(SHA256_OID)))
   {
      hashAlgo = SHA256_HASH_ALGO;
   }
   else
#endif
#if (OCSP_SHA384_SUPPORT == ENABLED && SHA384_SUPPORT == ENABLED)
   //SHA-384 hash algorithm identifier?
   if(!oidComp(oid, length, SHA384_OID, sizeof(SHA384_OID)))
   {
      hashAlgo = SHA384_HASH_ALGO;
   }
   else
#endif
#if (OCSP_SHA512_SUPPORT == ENABLED && SHA512_SUPPORT == ENABLED)
   //SHA-512 hash algorithm identifier?
   if(!oidComp(oid, length, SHA512_OID, sizeof(SHA512_OID)))
   {
      hashAlgo = SHA512_HASH_ALGO;
   }
   else
#endif
   //Unknown hash algorithm identifier?
   {
      hashAlgo = NULL;
   }

   //Return the hash algorithm that matches the specified OID
   return hashAlgo;
}

#endif
