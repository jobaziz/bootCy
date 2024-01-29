/**
 * @file boot.c
 * @brief CycloneBOOT Bootloader management
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
#define TRACE_LEVEL TRACE_LEVEL_INFO

//Dependencies
#include "bootloader/boot.h"
#include "bootloader/boot_fallback.h"
#include "bootloader/boot_common.h"
#include "core/flash.h"
#include "image/image.h"
#include "core/crc32.h"
#if ((BOOT_FALLBACK_SUPPORT == DISABLED) && \
     (BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED))
#include "core/mailbox.h"
#endif
#include "core/cboot_error.h"
#include "debug.h"

//Bootloader private-related functions
cboot_error_t bootGetCipherKey(BootContext *context);
cboot_error_t bootJumpToApp(BootContext *context);

/**
* @brief Initialize bootloader settings with default values
* @param[in,out] settings Structure that contains Bootloader settings
**/

void bootGetDefaultSettings(BootSettings *settings)
{
   //Clear bootloader user settings structure
   memset(settings, 0x00, sizeof(BootSettings));

#if(BOOT_FALLBACK_SUPPORT == ENABLED)
#if (BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED)
   //Secondary flash cipher key settings
   settings->psk = NULL;
   settings->pskSize = 0;
#endif
#endif
}


/**
 * @brief Initialize bootloader context
 * @param[in,out] context Bootloader context
 * @param[in] settings Bootloader user settings
 * @return Status code
 **/

cboot_error_t bootInit(BootContext *context, BootSettings *settings)
{
   cboot_error_t cerror;

   //Check parameter validity
   if(context == NULL || settings == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Set context fields to zero
   memset(context, 0, sizeof(BootContext));

   //Save bootloader user settings
   memcpy(&context->settings, settings, sizeof(BootSettings));

   //Initialize a primary flash driver and slots
   cerror = bootInitPrimaryMem(context, settings);
   //Is any error?
   if(cerror)
      return cerror;

   //Initialize a secondary (external) flash driver and slots
   cerror = bootInitSecondaryMem(context, settings);
   //Is any error?
   if(cerror)
      return cerror;

#if (BOOT_FALLBACK_SUPPORT == ENABLED)
#if (BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED)
   //Check the cipher key used to decode data in secondary flash (external memory)
   if(settings->psk == NULL || (settings->pskSize > sizeof(context->psk)))
   {
      return CBOOT_ERROR_INVALID_PARAMETERS;
   }
   else
   {
      //Store the cipher key used to decode data in secondary flash (external memory)
      memcpy(context->psk, settings->psk, settings->pskSize);
      context->pskSize = settings->pskSize;
   }
#endif

   //Initialize fallback trigger
   cerror = fallbackTriggerInit();
   //Is any error?
   if(cerror)
      return cerror;
#endif

   //Initialize bootloader state
   context->state = BOOT_STATE_IDLE;

   //Successful process
   return CBOOT_NO_ERROR;
}


/**
 * @brief Bootloader Task routine
 * @param[in] context Pointer to Bootloader context
 * @return None
 **/

cboot_error_t bootTask(BootContext *context)
{
   cboot_error_t cerror;
   uint32_t appStartAddr;
#if (BOOT_FALLBACK_SUPPORT == ENABLED)
   TriggerStatus trigStatus;
#endif

   //Initialize bootloader error status
   cerror = CBOOT_NO_ERROR;

   //Bootloader IDLE state
   if(context->state == BOOT_STATE_IDLE)
   {
#if (BOOT_FALLBACK_SUPPORT == ENABLED)
      //Get fallback trigger status
      cerror = fallbackTriggerGetStatus(&trigStatus);
      //Is any error?
      if(cerror)
      {
         //Change bootloader state
         bootChangeState(context, BOOT_STATE_ERROR);
      }
      else
      {
         //Is fallback trigger raised?
         if(trigStatus == TRIGGER_STATUS_RAISED)
         {
            //Debug message
            TRACE_INFO("Fallback requested...\r\n");

            //Change bootloader state
            bootChangeState(context, BOOT_STATE_FALLBACK_APP);
         }
         else
         {
#else
      if(1)
      {
         if(1) {
#endif
             //Select update image slot
             cerror = bootSelectUpdateImageSlot(context, &context->selectedSlot);

             //Is any error?
             if (cerror || context->selectedSlot.memParent == NULL)
             {
                 //Debug message
                 TRACE_ERROR("No valid image found!\r\n");
                 cerror = CBOOT_ERROR_FAILURE;
                 return cerror; //ERROR NO VALID IMAGE -> state error ????
             }
            else
            {
               //Debug message
               TRACE_DEBUG("Selected slot:\r\n");
               TRACE_DEBUG("- address:  0x%08lX\r\n", (unsigned long) context->selectedSlot.addr);
               TRACE_DEBUG("- size:     0x%08X\r\n", context->selectedSlot.size);

               //Is selected slot different from slot containing current application?
               //In other words, is selected image (in selected slot) more recent than current application image?
               if(memcmp(&context->selectedSlot, &context->memories[0].slots[0],sizeof(Slot)) != 0)
               {
                  //Change bootloader state
                  bootChangeState(context, BOOT_STATE_UPDATE_APP);
               }
               else
               {
                  //Change bootloader state
                  bootChangeState(context, BOOT_STATE_RUN_APP);
               }
            }
         }
      }
   }
   //Bootloader RUN APP state
   else if(context->state == BOOT_STATE_RUN_APP)
   {
      //Make sure to select first primary memory slot (contains current application)
      context->selectedSlot = context->memories[0].slots[0];

      //Debug message
		TRACE_INFO("No update available...\r\n");
      TRACE_INFO("Checking current application image...\r\n");

      //Check current application image inside first primary memory slot
      cerror = bootCheckImage(&context->selectedSlot);
      //Is any error?
      if(cerror)
      {
         //Change bootloader state
         bootChangeState(context, BOOT_STATE_ERROR);
      }
      else
      {
         //Check reset vector of the current application
         cerror = bootCheckSlotAppResetVector(&context->selectedSlot);
         //Is reset vector valid?
         if(!cerror)
         {
            //Debug message
            TRACE_INFO("Current application image is valid\r\n");
            TRACE_INFO("Booting to the application...\r\n");

            //Compute application start address
            appStartAddr = context->selectedSlot.addr + mcuGetVtorOffset();

            //Jump to current application inside primary memory slot
            mcuJumpToApplication(appStartAddr);
         }
         else
         {
            //Change bootloader state
            bootChangeState(context, BOOT_STATE_ERROR);
         }
      }
   }
   //Bootloader UPDATE APP state
   else if(context->state == BOOT_STATE_UPDATE_APP)
   {
      //Debug message
      TRACE_INFO("Checking update application image...\r\n");

      //Check current application image inside first primary memory slot
      cerror = bootCheckImage(&context->selectedSlot);
      //Is any error?
      if(cerror)
      {
         //Discard error
         cerror = CBOOT_NO_ERROR;
         //Change bootloader state
         bootChangeState(context, BOOT_STATE_RUN_APP);
      }
      else
      {
#if (BOOT_FALLBACK_SUPPORT == DISABLED && \
     BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED)
         //Call bootGetCipherKey here
         cerror = bootGetCipherKey(context);
         //Is any error?
         if(cerror)
         {
            //Debug message
            TRACE_ERROR("Failed to retrieve cipher key!\r\n");
            //Discard error
            cerror = CBOOT_NO_ERROR;
            //Change bootloader state
            bootChangeState(context, BOOT_STATE_RUN_APP);
         }
         else
#else
         if(1)
#endif
         {
            //Debug message
            TRACE_INFO("Starting update procedure...\r\n");

            //Start update procedure (could be a new application or because of a previous fallback procedure)
            cerror = bootUpdateApp(context, &context->selectedSlot);
            //Is any error?
            if(cerror)
            {
               //Change bootloader state
               bootChangeState(context, BOOT_STATE_ERROR);
            }
            else
            {
               //Debug message
               TRACE_INFO("Update procedure finished\r\n");
               TRACE_INFO("Rebooting...\r\n");

               //Reset system
               mcuSystemReset();
            }
         }
      }
   }
   //Bootloader FALLBACK APP state
   else if(context->state == BOOT_STATE_FALLBACK_APP)
   {
      //Call fallback routine here
      cerror = fallbackTask(context, context->memories);
      //Is any error.
      if(cerror)
      {
         //Debug message
         TRACE_INFO("Fallback procedure failed!\r\n");
         //Change bootloader state
         bootChangeState(context, BOOT_STATE_RUN_APP);
      }
      else
      {
         //Debug message
         TRACE_INFO("Fallback procedure finished\r\n");
         TRACE_INFO("Rebooting...\r\n");
         //Reset device
         mcuSystemReset();
      }
   }
   //Bootloader ERROR state
   else if(context->state == BOOT_STATE_ERROR)
   {
      //Bootloader reached error state
      TRACE_ERROR("Bootloader is in error state!\r\n");
   }
   else
   {
      //For sanity
      return CBOOT_ERROR_INVALID_VALUE;
   }

   //Return bootloader status
   return cerror;
}


/**
 * @brief Get PSK cipher key used to encrypt output image in external flash memory.
 * @param[in,out] context Pointer to the bootloader context
 **/

cboot_error_t bootGetCipherKey(BootContext *context)
{
#if ((BOOT_FALLBACK_SUPPORT == DISABLED) && \
     (BOOT_EXT_MEM_ENCRYPTION_SUPPORT == ENABLED))
   cboot_error_t cerror;
   BootMailBox msgBox;

   //Initialize status code
   cerror = CBOOT_NO_ERROR;

   //Debug message
   TRACE_INFO("Retrieving cipher key...\r\n");

   //Begin of handling block
   do
   {
      //Get message from shared SRAM (contains PSK key)
      cerror = getBootMailBox(&msgBox);
      if(cerror)
         break;

      //Check message validity
      cerror = checkBootMailBox(&msgBox);
      //Is any error?
      if(cerror)
         break;

      //Check cipher key used to decode data in secondary flash (external memory)
      if(msgBox.pskSize > sizeof(context->psk))
         break;

      //Store cipher key used to decode data in secondary flash (external memory)
      memcpy(context->psk, msgBox.psk, msgBox.pskSize);
      context->pskSize = msgBox.pskSize;
   } while(0);

   //Make sure to reset message from shared RAM memory
   memset(&msgBox, 0, sizeof(BootMailBox));
   setBootMailBox(&msgBox);

   //Return status code
   return cerror;
#else
   //Return error code
   return CBOOT_ERROR_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Jump to the application binary inside the current image in internal flash.
 * @input[in] context Pointer to the bootloader context
 * @return Status code
 **/

cboot_error_t bootJumpToApp(BootContext *context)
{
   error_t error;
   const FlashInfo *info;
   uint32_t mcuVtorOffset;
   FlashDriver *driver;

   //Check parameter validity
   if(context == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Get primary flash memory information
   driver = (FlashDriver *)context->memories[0].driver;
   error = driver->getInfo(&info);
   //Is any error?
   if(error)
      return CBOOT_ERROR_FAILURE;

   //Get MCU VTOR offset
   mcuVtorOffset = mcuGetVtorOffset();

   //Jump to application at given address
   mcuJumpToApplication(info->flashAddr + BOOT_OFFSET + mcuVtorOffset);

   //Successful process
   return CBOOT_NO_ERROR;
}
