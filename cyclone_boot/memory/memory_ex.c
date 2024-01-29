/**
 * @file memory_ex.c
 * @brief CycloneBOOT Memory Layer Abstraction (Extended)
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
#include "memory_ex.h"


/**
 * @brief Swap memory banks if and only if the memory is a flash
 * with dual bank capabilities.
 * @param[in] memory Memory pointer
 * @return Status code
 **/

cboot_error_t memoryExSwapBanks(Memory *memory)
{
    error_t error;
    const FlashDriver* fDrv;
    const FlashInfo* fInfo;

    //Initialize status code
    error = NO_ERROR;

    //Check parameters
    if(memory == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Check memory type
    if(memory->memoryType == MEMORY_TYPE_FLASH)
    {
        //Point the memory flash driver
        fDrv = (const FlashDriver*)memory->driver;

        //Get flash memory informations
        error = fDrv->getInfo(&fInfo);
        //Check there is no error
        if(!error)
        {
            //Check flash memory has dualbank capability
            if(fInfo->dualBank == 1)
            {
                //Swap memory banks
                error = fDrv->swapBanks();
            }
        }
    }
    else
        error = ERROR_FAILURE;

    //Return status code
    if(error)
       return CBOOT_ERROR_FAILURE;
    else
       return CBOOT_NO_ERROR;
}
