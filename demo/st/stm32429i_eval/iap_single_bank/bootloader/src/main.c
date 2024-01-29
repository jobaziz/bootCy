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
#include "stm32f4xx.h"
#include "stm324x9i_eval.h"
#include "bootloader/boot.h"
#include "drivers/memory/flash/internal/stm32f4xx_flash_driver.h"
#include "drivers/memory/flash/external/m29w128gl_flash_driver.h"
#include "core/cboot_error.h"
#include "debug.h"


//Global variables
uint8_t ledState = 0;
systime_t ledDelay = 0;
systime_t ledTimestamp = 0;

ADC_HandleTypeDef ADC_Handle;
BootSettings bootSettings;
BootContext bootContext;


/**
 * @brief System clock configuration
 **/

void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

   //Enable Power Control clock
   __HAL_RCC_PWR_CLK_ENABLE();

   //The voltage scaling allows optimizing the power consumption when the
   //device is clocked below the maximum system frequency
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   //Enable HSE Oscillator and activate PLL with HSE as source
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 25;
   RCC_OscInitStruct.PLL.PLLN = 360;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 7;
   HAL_RCC_OscConfig(&RCC_OscInitStruct);

   //Enable overdrive mode
   HAL_PWREx_EnableOverDrive();

   //Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
   //clocks dividers
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


/**
 * @brief ADC3 initialization
 **/

void adc3Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   ADC_ChannelConfTypeDef ADC_ChannelConfig;

   //Enable peripheral clocks
   __HAL_RCC_GPIOF_CLK_ENABLE();
   __HAL_RCC_ADC3_CLK_ENABLE();

   //Configure PF10 as an analog input
   GPIO_InitStructure.Pin = GPIO_PIN_10;
   GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

   //Configure ADC peripheral
   ADC_Handle.Instance = ADC3;
   ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
   ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
   ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   ADC_Handle.Init.ScanConvMode = DISABLE;
   ADC_Handle.Init.EOCSelection = DISABLE;
   ADC_Handle.Init.ContinuousConvMode = DISABLE;
   ADC_Handle.Init.NbrOfConversion = 1;
   ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
   ADC_Handle.Init.NbrOfDiscConversion = 0;
   ADC_Handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
   ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   ADC_Handle.Init.DMAContinuousRequests = DISABLE;
   HAL_ADC_Init(&ADC_Handle);

   //Configure ADC regular channel
   ADC_ChannelConfig.Channel = ADC_CHANNEL_8;
   ADC_ChannelConfig.Rank = 1;
   ADC_ChannelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
   ADC_ChannelConfig.Offset = 0;
   HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_ChannelConfig);
}


/**
 * @brief LED task
 * @param[in] param Unused parameter
 **/

void ledTask(void)
{
   if((uint32_t) (systemTicks - ledTimestamp) >= ledDelay)
   {
      //Toggle LED state
      if(ledState == 0)
      {
         BSP_LED_On(LED1);
         ledState = 1;
         ledDelay = 100;
      }
      else
      {
         BSP_LED_Off(LED1);
         ledState = 0;
         ledDelay = 900;
      }

      ledTimestamp = systemTicks;
   }
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   cboot_error_t cerror;

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
   TRACE_INFO("**********************************************\r\n");
   TRACE_INFO("*** CycloneBOOT IAP Single-Bank Bootloader ***\r\n");
   TRACE_INFO("**********************************************\r\n");
   TRACE_INFO("Copyright: 2010-2023 Oryx Embedded SARL\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: STM32F429\r\n");
   TRACE_INFO("\r\n");

   //LED configuration
   BSP_LED_Init(LED1);
   BSP_LED_Init(LED2);
   BSP_LED_Init(LED3);
   BSP_LED_Init(LED4);

   //Clear LEDs
   BSP_LED_Off(LED1);
   BSP_LED_Off(LED2);
   BSP_LED_Off(LED3);
   BSP_LED_Off(LED4);

   //Initialize user button
   BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
   //Initialize joystick
   BSP_JOY_Init(JOY_MODE_GPIO);

   //Begining of handling exception
   do
   {
	   //Get default booloader user settings
	   bootGetDefaultSettings(&bootSettings);

      bootSettings.memories[0].memoryType = MEMORY_TYPE_FLASH;
      bootSettings.memories[0].memoryRole = MEMORY_ROLE_PRIMARY;
      bootSettings.memories[0].driver = &stm32f4xxFlashDriver;
      bootSettings.memories[0].nbSlots = 1;

      bootSettings.memories[0].slots[0].type = SLOT_TYPE_DIRECT;
      bootSettings.memories[0].slots[0].cType = SLOT_CONTENT_BINARY;
      bootSettings.memories[0].slots[0].memParent = &bootSettings.memories[0];
      bootSettings.memories[0].slots[0].addr = 0x08020000;
      bootSettings.memories[0].slots[0].size = 0x1E0000;

      bootSettings.memories[1].memoryType = MEMORY_TYPE_FLASH;
      bootSettings.memories[1].memoryRole = MEMORY_ROLE_SECONDARY;
      bootSettings.memories[1].driver = &m29w128glFlashDriver;
      bootSettings.memories[1].nbSlots = 1;

      bootSettings.memories[1].slots[0].type = SLOT_TYPE_DIRECT;
      bootSettings.memories[1].slots[0].cType = SLOT_CONTENT_UPDATE;
      bootSettings.memories[1].slots[0].memParent = &bootSettings.memories[1];
      bootSettings.memories[1].slots[0].addr = 0x00000000;
      bootSettings.memories[1].slots[0].size = 0x1E0000;

	   //Initialize bootloader
	   cerror = bootInit(&bootContext, &bootSettings);
	   //Is any error?
	   if(cerror)
		   break;
   }while(0);

   //Is any error?
   if(cerror)
   {
	   //Debug message
	   TRACE_ERROR("Bootloader configuration failed!\r\n");
	   while(1);
   }

   while(1)
   {
	   ledTask();
	   cerror = bootTask(&bootContext);
	   //Is any error?
	   if(cerror)
	   {
		   //Debug message
		   TRACE_ERROR("Bootloader failure!\r\n");
		   break;
	   }
   }

   //This function should never return
   return 0;
}
