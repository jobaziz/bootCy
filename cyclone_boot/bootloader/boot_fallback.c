/**
 * @file boot_fallback.c
 * @brief CycloneBOOT Bootloader fallback managment
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
#define TRACE_LEVEL BOOT_TRACE_LEVEL

//Dependencies
#include "image/image.h"
#include "bootloader/boot.h"
#include "bootloader/boot_common.h"
#include "boot_fallback.h"
#include "debug.h"

//CycloneBOOT Bootloader fallback private related functions
cboot_error_t fallbackFindSlotWithEquivImg(BootContext * context, Slot **slotEquivImg, Memory *memories);
cboot_error_t fallbackCompareSlots(Slot *slot1, Slot *slot2, int8_t *res);
cboot_error_t fallbackDeleteSlot(Slot *slot);
cboot_error_t fallbackRestoreBackupSlot(BootContext *context, Slot *slot);

/**
 * @brief Fallback Task routine
 * @param[in] context Pointer to the Bootloader context
 * @return Status Code
 **/

cboot_error_t fallbackTask(BootContext *context, Memory *memories)
{
   cboot_error_t cerror;
   Slot *slotEquivImg;
   Slot *slotBackupImg;
   Slot *internalSlot;
   Slot *externalSlotOne;
   Slot *externalSlotTwo;
   int8_t res;

   //Initialize variables
   cerror = CBOOT_NO_ERROR;
   slotEquivImg = NULL;
   slotBackupImg = NULL;
   internalSlot = &memories[0].slots[0];
   externalSlotOne = &memories[1].slots[0];
   externalSlotTwo = &memories[1].slots[1];

   //Beginning of handling block
   do
   {
      //Check the current app image in (internal flash slot)
      cerror = bootCheckImage(internalSlot);
      //Is any error?
      if(cerror)
         break;

      //Check external app image 1 (in external flash slot 1)
      cerror = bootCheckImage(externalSlotOne);
      //Is any error?
      if(cerror)
         break;

      //Check external app image 2 (in eternal flash slot 2)
      cerror = bootCheckImage(externalSlotTwo);
      //Is any error?
      if(cerror)
         break;

      //Find image in external flash that is equivalent to the current app image in internal flash
      cerror = fallbackFindSlotWithEquivImg(context, &slotEquivImg, memories);
      //If any error or slot with equivalent image isn't found?
      if(cerror || slotEquivImg == NULL)
      {
         cerror = CBOOT_ERROR_ABORTED;
         break;
      }

      //Check that the other external image (in the other external flash slot) has an index inferior ot the current image in internal flash
      //If it is not the case then goto error state

      //Select the remaining slot that should contain the backup image of the previous valid application
      if(slotEquivImg == externalSlotOne)
         slotBackupImg = externalSlotTwo;
      else
         slotBackupImg = externalSlotOne;

      //Check that the remaining slot (containing the backup image) hold an image older that the current
      // image in the primary flash slot. If it is not the case, then there is no backup image in external
      //flash memory.
      cerror = fallbackCompareSlots(slotBackupImg, internalSlot, &res);
      if(cerror || res >= 0)
      {
         cerror = CBOOT_ERROR_ABORTED;
         break;
      }

      //Delete the external image equivalent of the current app image
      cerror = fallbackDeleteSlot(slotEquivImg);
      //Is any error?
      if(cerror)
         break;

      //Restore the remaining image in external memory slot (backup of the previous valid app)
      cerror = fallbackRestoreBackupSlot(context, slotBackupImg);
      //Is any error?
      if(cerror)
         break;
   }while(0);

   //Return status code
   return cerror;
}


/**
 * @brief Delete the given slot. In other words it erase the content of the given slot.
 * @param[in] slot Pointer to the slot to be deleted.
 * @return Error code.
 **/

cboot_error_t fallbackDeleteSlot(Slot * slot)
{
   error_t error;
   Memory *memory;
   FlashDriver *flashDrv;

   //Point to the slot flash driver
   memory = (Memory *)slot->memParent;
   flashDrv = (FlashDriver *)memory->driver;

   //Erase slot data
   error = flashDrv->erase(slot->addr, slot->size);
   //Is any error?
   if(error)
      return CBOOT_ERROR_FAILURE;
   else
      return CBOOT_NO_ERROR;
}


/**
 * @brief Restore the image contained in the backup slot.
 * It will extract the firmware application from the image inside the backup slot.
 * Then a new image that old the back application firmware will be generated in internal memory slot.
 * It will allow the system to boot to the backup application firmware.
 * @param[in] slot Pointer to the slot to be deleted.
 * @return Error code.
 **/

cboot_error_t fallbackRestoreBackupSlot(BootContext *context, Slot *slot)
{
   //Restore the application firmware backup by updating the system using
   //the slot containing the backup image that hold the application firmware backup.
   return bootUpdateApp(context, slot);
}


/**
 * @brief Compare two given slots together with respect to the index
 * of the images inside the slots.
 * - If the image index of the first slot (slot1) is strictly inferior
 *   to the image index of the first slot (slot1) then result will be -1.
 * - If the image index of the first slot (slot1) is equal
 *   to the image index of the first slot (slot1) then result will be 0.
 * - Otherwise the result will be 1.
 * @param[in] slot1 Pointer to the first slot to be compared with.
 * @param[in] slot2 Pointer to the second slot to be compared with.
 * @param[ou] res Result of the slot comparison.
 * @return Error code.
 **/

cboot_error_t fallbackCompareSlots(Slot *slot1, Slot *slot2, int8_t *res)
{
   cboot_error_t cerror;
   uint32_t saveImgIdx;
   ImageHeader imgHeader;

   //Initialize status code
   cerror = CBOOT_NO_ERROR;

   //Check parameters validity
   if(slot1 == NULL || slot2 == NULL || res == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Get image header from the first slot.
   cerror = bootGetSlotImgHeader(slot1, &imgHeader);
   //Check there is no error
   if(!cerror)
   {
      //Save index of the image inside the first slot for
      // later comparison.
      saveImgIdx = imgHeader.imgIndex;

      //Get image header from the second slot.
      cerror = bootGetSlotImgHeader(slot2, &imgHeader);
      //Check there is no error
      if(!cerror)
      {
         //Is the image index from the first slot strictly
         // inferior to the image index from the second slot?
         if(saveImgIdx < imgHeader.imgIndex)
            *res = -1;
         //Is image index from the first slot equal the image index from the second slot?
         else if(saveImgIdx == imgHeader.imgIndex)
            *res = 0;
         //Is the image index from the first slot strictly
         // superior to the image index from the second slot?
         else
            *res = 1;
      }
   }

   //Return state code
   return cerror;
}


/**
 * @brief Search for the slot in external memory that contains the image equivalent
 * of the current image in internal flash slot. "Equivalent" means that the image to
 * be found will have the same index than the current image in internal flash.
 * @param[in] context Pointer to bootloader context.
 * @param[out] slotEquivImg Pointer to the slot that holding the equivalent image.
 * @return Error code.
 **/

cboot_error_t fallbackFindSlotWithEquivImg(BootContext *context, Slot **slotEquivImg, Memory *memories)
{
   cboot_error_t cerror;
   uint_t i;
   uint32_t currImgIdx;
   ImageHeader imgHeader;
   bool_t foundSlot;
   Slot internalSlot;

   cerror = CBOOT_NO_ERROR;
   foundSlot = FALSE;

   internalSlot = memories[0].slots[0];
   internalSlot.memParent = &memories[0];

   //Check parameters validity
   if(context == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Get image header from the internal flash memory slot.
   cerror = bootGetSlotImgHeader(&internalSlot, &imgHeader);
   //Check there is no error
   if(!cerror)
   {
      //Save index of the current image in internal flash
      currImgIdx = imgHeader.imgIndex;

      //Loop through the external flash slot list
      for(i = 0; i < FLASH_SLOTS_NUMBER; i++)
      {
         //Get image header from the listed slot
         cerror = bootGetSlotImgHeader(&memories[1].slots[i], &imgHeader);
         //Check there is no error
         if(cerror)
         {
            break;
         }
         else
         {
            //Is the index of the current image being equal to the index of the image from the listed slot?
            if(imgHeader.imgIndex == currImgIdx)
            {
               //Equivalent slot is found.
               foundSlot = TRUE;
               //Saving equivalent slot pointer.
               *slotEquivImg = &memories[1].slots[i];
               break;
            }
         }
      }

      //Is the equivalent slot not found?
      if(!foundSlot)
      {
         //Raise an error
         cerror = CBOOT_ERROR_FAILURE;
      }
   }

   //Return status code
   return cerror;
}


#if ((defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || \
   defined(__GNUC__) || defined(__CC_ARM) || defined(__IAR_SYSTEMS_ICC__) || \
   defined(__TASKING__) || defined(__CWCC__) || defined(__TI_ARM__))

/**
 * @brief Fallback Trigger Initialisation callback.
 * It is declared week and must be declared in user codde space.
 * @return Status Code
 **/

__weak_func cboot_error_t fallbackTriggerInit(void)
{
   //This function Should not be modified when the callback is needed,
   // the fallbackTriggerInit must be implemented in the user file

   return CBOOT_ERROR_NOT_IMPLEMENTED;
}


/**
 * @brief Fallback Trigger Status callback.
 * It is declared week and must be declared in user codde space.
 * @param[in] status Trigger status to be returned (Raised or Idle)
 * @return Status Code
 **/

__weak_func cboot_error_t fallbackTriggerGetStatus(TriggerStatus *status)
{
   //This function Should not be modified when the callback is needed,
   //the fallbackTriggerGetStatus must be implemented in the user file

   return CBOOT_ERROR_NOT_IMPLEMENTED;
}
#endif
