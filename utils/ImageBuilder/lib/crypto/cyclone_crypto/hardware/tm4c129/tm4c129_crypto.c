/**
 * @file tm4c129_crypto.c
 * @brief Tiva TM4C129 hardware cryptographic accelerator
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
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "core/crypto.h"
#include "hardware/tm4c129/tm4c129_crypto.h"
#include "debug.h"

//Global variables
OsMutex tm4c129CryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t tm4c129CryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&tm4c129CryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

   //Check status code
   if(!error)
   {
      //Enable the CCM module
      SysCtlPeripheralEnable(SYSCTL_PERIPH_CCM0);
      //Wait for the CCM module to be ready
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_CCM0))
      {
      }
   }

   //Return status code
   return error;
}
