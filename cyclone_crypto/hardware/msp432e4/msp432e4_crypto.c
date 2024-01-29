/**
 * @file msp432e4_crypto.c
 * @brief MSP432E4 hardware cryptographic accelerator
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
#include <stdint.h>
#include "msp432.h"
#include "driverlib/sysctl.h"
#include "core/crypto.h"
#include "hardware/msp432e4/msp432e4_crypto.h"
#include "debug.h"

//Global variables
OsMutex msp432e4CryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t msp432e4CryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&msp432e4CryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

   //Check status code
   if(!error)
   {
      //Enable and reset CCM peripheral
      SysCtlPeripheralEnable(SYSCTL_PERIPH_CCM0);
      SysCtlPeripheralReset(SYSCTL_PERIPH_CCM0);

      //Wait for the CCM peripheral to be ready
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_CCM0))
      {
      }
   }

   //Return status code
   return error;
}
