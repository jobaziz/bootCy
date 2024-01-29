/**
 * @file pic32cx_mt_crypto.c
 * @brief PIC32CX MTC/MTG/MTSH hardware cryptographic accelerator
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
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "pic32c.h"
#include "core/crypto.h"
#include "hardware/pic32cx_mt/pic32cx_mt_crypto.h"
#include "hardware/pic32cx_mt/pic32cx_mt_crypto_trng.h"
#include "hardware/pic32cx_mt/pic32cx_mt_crypto_hash.h"
#include "hardware/pic32cx_mt/pic32cx_mt_crypto_cipher.h"
#include "debug.h"

//Global variables
OsMutex pic32cxmtCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t pic32cxmtCryptoInit(void)
{
   error_t error;
#if (PIC32CX_MT_CRYPTO_HASH_SUPPORT == ENABLED || PIC32CX_MT_CRYPTO_CIPHER_SUPPORT == ENABLED)
   uint32_t temp;
#endif

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&pic32cxmtCryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

#if (PIC32CX_MT_CRYPTO_TRNG_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize TRNG module
      error = trngInit();
   }
#endif

#if (PIC32CX_MT_CRYPTO_HASH_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable SHA peripheral clock
      PMC_REGS->PMC_PCR = PMC_PCR_PID(ID_SHA);
      temp = PMC_REGS->PMC_PCR;
      PMC_REGS->PMC_PCR = temp | PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk;
   }
#endif

#if (PIC32CX_MT_CRYPTO_CIPHER_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable AES peripheral clock
      PMC_REGS->PMC_PCR = PMC_PCR_PID(ID_AES);
      temp = PMC_REGS->PMC_PCR;
      PMC_REGS->PMC_PCR = temp | PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk;
   }
#endif

   //Return status code
   return error;
}
