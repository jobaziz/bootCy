/**
 * @file uart_user.c
 * @brief User UART routine
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

//Dependencies
#include "stm32u5xx.h"
#include "stm32u5xx_hal.h"
#include "os_port.h"
#include "update/update.h"
#include "version.h"
#include "uart_user.h"
#include "common.h"
#include "ymodem.h"
#include "error.h"
#include "core/cboot_error.h"
#include "debug.h"
#include "drivers/memory/flash/internal/stm32u5xx_flash_driver.h"

//Y-Modem uart handler
UART_HandleTypeDef UartHandle;

//CycloneBOOT Iap Context
UpdateContext updateContext;
//CycloneBOOT Iap Settings
UpdateSettings updateSettings;

//IAP mutex handler
extern OsMutex updateMutex;

//Extern Y-Modem filename buffer holder
extern uint8_t aFileName[FILE_NAME_LENGTH];

//Extern update signature public key declaration
extern const uint8_t* pemUpdtSignPublicKey;
extern size_t pemUpdtSignPublicKeyLen;


/**
 * @brief Download a file via serial port
 **/

void uartUserDownload(void)
{
   uint8_t number[11] = {0};
   uint32_t size = 0;
   COM_StatusTypeDef result;

   cboot_error_t cboot_error;

   //Get exclusive access
   osAcquireMutex(&updateMutex);

   //Suspend scheduler
   osSuspendAllTasks();

   //Debug message
   TRACE_INFO("\r\n");
   TRACE_INFO("Starting firmware update...\r\n");

   //Get default Update settings
   updateGetDefaultSettings(&updateSettings);

#if (UPDATE_ANTI_ROLLBACK_SUPPORT == ENABLED)
   updateSettings.appVersion = APP_VERSION;
#endif
   //User update settings security configuration
   updateSettings.imageInCrypto.verifySettings.verifyMethod    = VERIFY_METHOD_SIGNATURE;
   updateSettings.imageInCrypto.verifySettings.signAlgo        = VERIFY_SIGN_RSA;
   updateSettings.imageInCrypto.verifySettings.signHashAlgo    = SHA256_HASH_ALGO;
   updateSettings.imageInCrypto.verifySettings.signKey         = (const char_t*) pemUpdtSignPublicKey;
   updateSettings.imageInCrypto.verifySettings.signKeyLen      = pemUpdtSignPublicKeyLen;
   updateSettings.imageInCrypto.cipherAlgo                     = AES_CIPHER_ALGO;
   updateSettings.imageInCrypto.cipherMode                     = CIPHER_MODE_CBC;
   updateSettings.imageInCrypto.cipherKey                      = "aa3ff7d43cc015682c7dfd00de9379e7";
   updateSettings.imageInCrypto.cipherKeyLen                   = 32;

   //User update settings primary memory configuration
   updateSettings.memories[0].memoryRole           = MEMORY_ROLE_PRIMARY;
   updateSettings.memories[0].memoryType           = MEMORY_TYPE_FLASH;
   updateSettings.memories[0].driver               = &stm32u5xxFlashDriver;
   updateSettings.memories[0].nbSlots              = 2;
   //User update settings primary memory slot 0 configuration
   updateSettings.memories[0].slots[0].type        = SLOT_TYPE_DIRECT;
   updateSettings.memories[0].slots[0].cType       = SLOT_CONTENT_APP | SLOT_CONTENT_BACKUP;
   updateSettings.memories[0].slots[0].memParent   = &updateSettings.memories[0];
   updateSettings.memories[0].slots[0].addr        = 0x08000000;
   updateSettings.memories[0].slots[0].size        = 0x100000;
   //User update settings primary memory slot 1 configuration
   updateSettings.memories[0].slots[1].type        = SLOT_TYPE_DIRECT;
   updateSettings.memories[0].slots[1].cType       = SLOT_CONTENT_APP | SLOT_CONTENT_BACKUP;
   updateSettings.memories[0].slots[1].memParent   = &updateSettings.memories[0];
   updateSettings.memories[0].slots[1].addr        = 0x08100000;
   updateSettings.memories[0].slots[1].size        = 0x100000;

   //Initialize IAP Application context
   cboot_error = updateInit(&updateContext, &updateSettings);
   //Is any error?
   if(cboot_error != CBOOT_NO_ERROR)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize update library!\r\n");
   }
   else
   {
      Serial_PutString((uint8_t *)"Waiting for the file to be sent ... (press 'a' to abort)\n\r");
      result = Ymodem_Receive( &size );
      if (result == COM_OK)
      {
         Serial_PutString((uint8_t *)"\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
         Serial_PutString(aFileName);
         Int2Str(number, size);
         Serial_PutString((uint8_t *)"\n\r Size: ");
         Serial_PutString(number);
         Serial_PutString((uint8_t *)" Bytes\r\n");
         Serial_PutString((uint8_t *)"-------------------\n");

         cboot_error = updateFinalize(&updateContext);
         if(cboot_error != CBOOT_NO_ERROR)
         {
            //Debug message
            TRACE_ERROR("Failed to finalize firmware update!\r\n");
         }
         else
         {
            cboot_error = updateReboot(&updateContext);
            if(cboot_error != CBOOT_NO_ERROR)
            {
               //Debug message
               TRACE_ERROR("Failed to reboot!\r\n");
            }
         }
      }
      else if (result == COM_LIMIT)
      {
         Serial_PutString((uint8_t *)"\n\n\rThe image size is higher than the allowed space memory!\n\r");
      }
      else if (result == COM_DATA)
      {
         Serial_PutString((uint8_t *)"\n\n\rVerification failed!\n\r");
      }
      else if (result == COM_ABORT)
      {
         Serial_PutString((uint8_t *)"\r\n\nAborted by user.\n\r");
      }
      else
      {
         Serial_PutString((uint8_t *)"\n\rFailed to receive the file!\n\r");
      }
   }

   //Resume scheduler
   osResumeAllTasks();

   //Release exclusive access
   osReleaseMutex(&updateMutex);
}


/**
 * @brief Y-Modem Receive User callback
 * @param[in] data Pointer to the received data
 * @param[in] length Length of the received data
 * @retuen Error code
 **/

cboot_error_t Ymodem_Receive_UserCallback(uint8_t *data, size_t length)
{
   cboot_error_t cboot_error;

   //Write received bytes in flash
   cboot_error = updateProcess(&updateContext, data, length);
   //Is any error?
   if(cboot_error != CBOOT_NO_ERROR)
   {
      //Debug message
      TRACE_ERROR("Failed to update firmware!\r\n");
   }

   //Status code
   return cboot_error;
}


/**
 * @brief Initialize user uart
 **/

void uartUserInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   //Enable GPIOD clock
   __HAL_RCC_GPIOD_CLK_ENABLE();
   //Enable USART2 clock
   __HAL_RCC_USART2_CLK_ENABLE();

   //Configure USART2_TX (PD5)
   GPIO_InitStructure.Pin = GPIO_PIN_5;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_PULLUP;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
   GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
   HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

   //Configure USART2_RX (PD6)
   GPIO_InitStructure.Pin = GPIO_PIN_6;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_PULLUP;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
   GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
   HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

   //Configure USART2
   UartHandle.Instance = USART2;
   UartHandle.Init.BaudRate = 115200;
   UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
   UartHandle.Init.StopBits = UART_STOPBITS_1;
   UartHandle.Init.Parity = UART_PARITY_NONE;
   UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   UartHandle.Init.Mode = UART_MODE_TX_RX;
   HAL_UART_Init(&UartHandle);
}


/**
 * @brief Uart user task routine
 **/

void uartUserTask(void *param)
{
   uint8_t key = 0;
   uint8_t state = 0;
   HAL_StatusTypeDef status;

   //Init uart
   uartUserInit();

   while(1)
   {
      if(state == 0)
      {
         Serial_PutString((uint8_t*) "\r\n");
         Serial_PutString((uint8_t*) "=======================\r\n");
         Serial_PutString((uint8_t*) "= Y-MODEM update menu =\r\n");
         Serial_PutString((uint8_t*) "=======================\r\n");
         Serial_PutString((uint8_t*) "Choices:\r\n");
         Serial_PutString((uint8_t*) "  1 -> Download update image\r\n");
         Serial_PutString((uint8_t*) "  2 -> Get current application version\r\n");
         Serial_PutString((uint8_t*) "\r\n");

         //Change state
         state = 1;
      }
      else if(state == 1)
      {
         /* Clean the input path */
         __HAL_UART_FLUSH_DRREGISTER(&UartHandle);

         /* Receive key */
         status = HAL_UART_Receive(&UartHandle, &key, 1, RX_TIMEOUT);

         if(status == HAL_OK)
         {
            switch(key)
            {
               case '1':
                  Serial_PutString((uint8_t*) "Downloading update image...\r\n");
                  //Start downloading file
                  uartUserDownload();
                  break;

               case '2':
            	   Serial_PutString((uint8_t*) APP_VERSION_STRING);
                  Serial_PutString((uint8_t*) "\r\n");
                  break;

               default:
                  Serial_PutString((uint8_t*) "Unknown number!\r\n");
                  Serial_PutString((uint8_t*) "Number should be 1 or 2!\r\n");
                  break;
            }
            Serial_PutString((uint8_t*) "\r\n");

            //Change state
            state = 0;
         }
      }
      else
      {
         //For sanity
      }

      //Os delay
      osDelayTask(200);
   }
}
