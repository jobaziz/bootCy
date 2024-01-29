/**
 * @file update_fallaback.c
 * @brief CycloneBOOT IAP Fallback Functions API
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include "update/update_fallback.h"
#include "error.h"

//Extern Flash driver Swap bank (No-Init) routine
extern error_t flashDriverSwapBanksNoInit(void);
//Extern device MCU related function
extern void mcuSystemReset(void);

/**
 * @brief Start CycloneBOOT Dual Bank mode fallback procedure.
 * It will switch flash bank and then restart the MCU.
 * @return Status code
 **/

cboot_error_t updateFallbackStart(void)
{
   error_t error;

   //Start fallback procedure
   error = flashDriverSwapBanksNoInit();
   //Is any error?
   if(error)
   {
      //Return Status code
      return CBOOT_ERROR_FAILURE;
   }
   else
   {
      //Reset MCU
      mcuSystemReset();

      //To avoid compilation warning
      return CBOOT_NO_ERROR;
   }
}
