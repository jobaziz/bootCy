/**
 * @file ctr.c
 * @brief Counter(CTR) mode
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "cipher_mode/ctr.h"
#include "debug.h"

//Check crypto library configuration
#if (CTR_SUPPORT == ENABLED)


/**
 * @brief CTR encryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in] m Size in bits of the specific part of the block to be incremented
 * @param[in,out] t Initial counter block
 * @param[in] p Plaintext to be encrypted
 * @param[out] c Ciphertext resulting from the encryption
 * @param[in] length Total number of data bytes to be encrypted
 * @return Error code
 **/

__weak_func error_t ctrEncrypt(const CipherAlgo *cipher, void *context, uint_t m,
   uint8_t *t, const uint8_t *p, uint8_t *c, size_t length)
{
   size_t i;
   size_t n;
   uint16_t temp;
   uint8_t o[16];

   //The parameter must be a multiple of 8
   if((m % 8) != 0)
      return ERROR_INVALID_PARAMETER;

   //Determine the size, in bytes, of the specific part of the block
   //to be incremented
   m = m / 8;

   //Check the resulting value
   if(m > cipher->blockSize)
      return ERROR_INVALID_PARAMETER;

   //Process plaintext
   while(length > 0)
   {
      //CTR mode operates in a block-by-block fashion
      n = MIN(length, cipher->blockSize);

      //Compute O(j) = CIPH(T(j))
      cipher->encryptBlock(context, t, o);

      //Compute C(j) = P(j) XOR T(j)
      for(i = 0; i < n; i++)
      {
         c[i] = p[i] ^ o[i];
      }

      //Standard incrementing function
      for(temp = 1, i = 1; i <= m; i++)
      {
         //Increment the current byte and propagate the carry
         temp += t[cipher->blockSize - i];
         t[cipher->blockSize - i] = temp & 0xFF;
         temp >>= 8;
      }

      //Next block
      p += n;
      c += n;
      length -= n;
   }

   //Successful encryption
   return NO_ERROR;
}


/**
 * @brief CTR decryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in] m Size in bits of the specific part of the block to be incremented
 * @param[in,out] t Initial counter block
 * @param[in] c Ciphertext to be decrypted
 * @param[out] p Plaintext resulting from the decryption
 * @param[in] length Total number of data bytes to be decrypted
 * @return Error code
 **/

error_t ctrDecrypt(const CipherAlgo *cipher, void *context, uint_t m,
   uint8_t *t, const uint8_t *c, uint8_t *p, size_t length)
{
   //Decryption is the same the as encryption with P and C interchanged
   return ctrEncrypt(cipher, context, m, t, c, p, length);
}

#endif
