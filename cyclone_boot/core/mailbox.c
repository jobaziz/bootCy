/**
 * @file mailbox.c
 * @brief CycloneBOOT IAP Miscellaneous Functions
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
#include "mailbox.h"

// Create a section to place the boot mailbox in RAM
#if defined(__CC_ARM)
BootMailBox bootMailBox __attribute__((__section__(".boot_mailbox"), zero_init));
#elif defined(__GNUC__)
BootMailBox bootMailBox __attribute__((section(".boot_mailbox")));
#elif defined(_MSC_VER)
extern BootMailBox bootMailBox;
#endif

/**
 * @brief Set message into the shared bootloader mailbox.
 * It will be processed by the bootloader at the next reboot.
 * @param[in] mailbox Pointer to the message to be set in the bootloader mailbox
 * @return Status code
 **/

cboot_error_t setBootMailBox(BootMailBox *mailbox)
{
   //Check parameters validity
   if(mailbox == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   bootMailBox.version = mailbox->version;
   bootMailBox.signature = mailbox->signature;
   bootMailBox.pskSize = mailbox->pskSize;
   memcpy(bootMailBox.psk, (uint8_t*)mailbox->psk, BOOT_MBX_PSK_MAX_SIZE);

   //Successful process
   return CBOOT_NO_ERROR;
}


/**
 * @brief Get message from the shared bootloader mailbox.
 * @param[in,out] mailbox Pointer to the message to retrieved from the bootloader mailbox.
 * @return Status code
 **/

cboot_error_t getBootMailBox(BootMailBox *mailbox)
{
   //Check parameters validity
   if(mailbox == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   mailbox->version = bootMailBox.version;
   mailbox->signature = bootMailBox.signature;
   mailbox->pskSize = bootMailBox.pskSize;
   memcpy((uint8_t*)mailbox->psk, bootMailBox.psk, BOOT_MBX_PSK_MAX_SIZE);

   //Successful process
   return CBOOT_NO_ERROR;
}

/**
 * @brief Check Bootloader mailbox message validity.
 * @param[in] Pointeur to the message to be checked.
 * @return Status code
 **/

cboot_error_t checkBootMailBox(BootMailBox *mailbox)
{
   cboot_error_t error;

   //Check paramter validity
   if(mailbox == NULL)
      return CBOOT_ERROR_INVALID_PARAMETERS;

   //Initialize error code
   error = CBOOT_ERROR_FAILURE;

   //Check msg isn't null
   if(mailbox != NULL)
   {
      if ((mailbox->version == BOOT_MBX_VERSION) && (mailbox->signature == BOOT_MBX_SIGNATURE))
         error = CBOOT_NO_ERROR;
   }

   //Return error code
   return error;
}
