/**
 * @file main.c
 * @brief Main routine
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
#include <stdlib.h>
#include "stm32u5xx.h"
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo.h"
#include "resource_manager.h"
#include "version.h"
#include "uart_user.h"
#include "debug.h"

//Update signature public key resource path
#define UPDT_PUBLIC_SIGN_KEY "keys/rsa_public_key.pem"

//Global variables
const uint8_t* pemUpdtSignPublicKey;
size_t pemUpdtSignPublicKeyLen;

//IAP mutex handler
OsMutex updateMutex;

/**
 * @brief System clock configuration
 **/

void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
   RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStruct = {0};

   //Enable voltage range 1 for frequency above 100 MHz
   __HAL_RCC_PWR_CLK_ENABLE();
   HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
   __HAL_RCC_PWR_CLK_DISABLE();

   //Configure PLL with MSI as source
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_MSI;
   RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
   RCC_OscInitStruct.MSIState = RCC_MSI_ON;
   RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
   RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
   RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
   RCC_OscInitStruct.PLL.PLLM = 1;
   RCC_OscInitStruct.PLL.PLLN = 80;
   RCC_OscInitStruct.PLL.PLLR = 2;
   RCC_OscInitStruct.PLL.PLLP = 2;
   RCC_OscInitStruct.PLL.PLLQ = 2;
   RCC_OscInitStruct.PLL.PLLFRACN = 0;
   HAL_RCC_OscConfig(&RCC_OscInitStruct);

   //Select PLL as system clock source and configure bus clocks dividers
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
   RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

   //Select clock source for RNG peripheral
   RCC_PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
   RCC_PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
   HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStruct);
}


/**
 * @brief LED task
 * @param[in] param Unused parameter
 **/

void ledTask(void *param)
{
   //Endless loop
   while(1)
   {
      BSP_LED_On(LED1);
      osDelayTask(100);
      BSP_LED_Off(LED1);
      osDelayTask(900);
   }
}


/**
 * @brief Load user RSA public key from "res.c".
 * "res.c" is a C array file system representing the demo folder "resources/keys".
 **/

error_t updateUserLoadKeys(void)
{
   error_t error;

   //Get server's private key
   error = resGetData(UPDT_PUBLIC_SIGN_KEY, (const uint8_t **) &pemUpdtSignPublicKey,
      &pemUpdtSignPublicKeyLen);
   //Any error to report?
   if(error)
      return error;

   //Successful process
   return NO_ERROR;
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   error_t error;
   OsTaskId taskId;
   OsTaskParameters taskParams;

   //HAL library initialization
   HAL_Init();
   //Configure the system clock
   SystemClock_Config();

   //Initialize kernel
   osInitKernel();
   //Configure debug UART
   debugInit(115200);

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("***************************************************\r\n");
   TRACE_INFO("*** CycloneBOOT IAP Dual-Bank UART Y-MODEM Demo ***\r\n");
   TRACE_INFO("***************************************************\r\n");
   TRACE_INFO("Version: %s\r\n", APP_VERSION_STRING);
   TRACE_INFO("Copyright: 2010-2022 Oryx Embedded SARL\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: STM32u596\r\n");
   TRACE_INFO("\r\n");

   //LED configuration
   BSP_LED_Init(LED1);
   BSP_LED_Init(LED2);
   BSP_LED_Init(LED3);

   //Clear LEDs
   BSP_LED_Off(LED1);
   BSP_LED_Off(LED2);
   BSP_LED_Off(LED3);

   //Initialize user button
   BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

   //Load IAP public signature key
   error = updateUserLoadKeys();
   if (error)
   {
	  //Debug message
      TRACE_ERROR("Failed to load IAP user keys!\r\n");
      return error;
   }

   //Create IAP mutex
   if(!osCreateMutex(&updateMutex))
   {
      //Debug message
      TRACE_ERROR("Failed to create IAP mutex!\r\n");
   }

   //Set USER task parameters
   taskParams = OS_TASK_DEFAULT_PARAMS;
   taskParams.stackSize = 500;
   taskParams.priority = OS_TASK_PRIORITY_NORMAL;

   //Create user task
   taskId = osCreateTask("UART User", uartUserTask, NULL, &taskParams);
   //Failed to create the task?
   if(taskId == OS_INVALID_TASK_ID)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Set LED task parameters
   taskParams = OS_TASK_DEFAULT_PARAMS;
   taskParams.stackSize = 200;
   taskParams.priority = OS_TASK_PRIORITY_NORMAL;

   //Create a task to blink the LED
   taskId = osCreateTask("LED", ledTask, NULL, &taskParams);
   //Failed to create the task?
   if(taskId == OS_INVALID_TASK_ID)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Start the execution of tasks
   osStartKernel();

   //This function should never return
   return 0;
}
