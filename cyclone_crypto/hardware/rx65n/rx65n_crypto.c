/**
 * @file rx65n_crypto.c
 * @brief RX65N hardware cryptographic accelerator (TSIP)
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
#include "r_tsip_rx_if.h"
#include "core/crypto.h"
#include "hardware/rx65n/rx65n_crypto.h"
#include "debug.h"

//Global variables
OsMutex rx65nCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t rx65nCryptoInit(void)
{
   e_tsip_err_t status;

   //Initialize status code
   status = TSIP_SUCCESS;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&rx65nCryptoMutex))
   {
      //Failed to create mutex
      status = TSIP_ERR_FAIL;
   }

   //Check status code
   if(status == TSIP_SUCCESS)
   {
      //Initialize TSIP module
      status = R_TSIP_Open(NULL, NULL);
   }

   //Return status code
   return (status == TSIP_SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}
