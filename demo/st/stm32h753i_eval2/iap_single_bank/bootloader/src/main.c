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
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_lcd.h"
#include "bootloader/boot.h"
#include "drivers/memory/flash/internal/stm32h7xx_flash_driver.h"
#include "drivers/memory/flash/external/mt25tl01g_flash_driver.h"
#include "core/cboot_error.h"
#include "debug.h"

//LCD frame buffers
#define LCD_FRAME_BUFFER_LAYER0 0xD0400000
#define LCD_FRAME_BUFFER_LAYER1 0xD0480000


//Global variables
uint_t lcdLine = 0;
uint_t lcdColumn = 0;
uint_t adcValue = 0;
uint_t joystickState = 0;
int8_t ax = 0;
int8_t ay = 0;
int8_t az = 0;

uint8_t ledState = 0;
systime_t ledStart = 0;
systime_t ledTimeout = 0;

BootSettings bootSettings;
BootContext bootContext;

/**
 * @brief Set cursor location
 * @param[in] line Line number
 * @param[in] column Column number
 **/

void lcdSetCursor(uint_t line, uint_t column)
{
   lcdLine = MIN(line, 20);
   lcdColumn = MIN(column, 40);
}


/**
 * @brief Write a character to the LCD display
 * @param[in] c Character to be written
 **/

void lcdPutChar(char_t c)
{
   if(c == '\r')
   {
      lcdColumn = 0;
   }
   else if(c == '\n')
   {
      lcdColumn = 0;
      lcdLine++;
   }
   else if(lcdLine < 20 && lcdColumn < 40)
   {
      //Display current character
      BSP_LCD_DisplayChar(lcdColumn * 16, lcdLine * 24, c);

      //Advance the cursor position
      if(++lcdColumn >= 40)
      {
         lcdColumn = 0;
         lcdLine++;
      }
   }
}


/**
 * @brief System clock configuration
 **/

void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
   RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStruct = {0};

   //Supply configuration update enable
   HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

   //Configure voltage scaling
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
   while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY));

   //Enable HSE oscillator and activate PLL with HSE as source
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
   RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 5;
   RCC_OscInitStruct.PLL.PLLN = 160;
   RCC_OscInitStruct.PLL.PLLFRACN = 0;
   RCC_OscInitStruct.PLL.PLLP = 2;
   RCC_OscInitStruct.PLL.PLLR = 2;
   RCC_OscInitStruct.PLL.PLLQ = 4;
   RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
   RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;

   HAL_RCC_OscConfig(&RCC_OscInitStruct);

   //Select PLL as system clock source and configure bus clocks dividers
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1;

   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
   RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
   RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

   //Select clock source for RNG peripheral
   RCC_PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
   RCC_PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;

   HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStruct);

   //Enable CSI clock
   __HAL_RCC_CSI_ENABLE();
   //Enable SYSCFG clock
   __HAL_RCC_SYSCFG_CLK_ENABLE() ;

   //Enable the I/O compensation cell
   HAL_EnableCompensationCell();
}


/**
 * @brief MPU configuration
 **/

void MPU_Config(void)
{
   MPU_Region_InitTypeDef MPU_InitStruct;

   //Disable MPU
   HAL_MPU_Disable();

   //DTCM RAM
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER0;
   MPU_InitStruct.BaseAddress = 0x20000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
   MPU_InitStruct.SubRegionDisable = 0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   //AXI SRAM
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER1;
   MPU_InitStruct.BaseAddress = 0x24000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
   MPU_InitStruct.SubRegionDisable = 0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   //AHB SRAM
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER2;
   MPU_InitStruct.BaseAddress = 0x30000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
   MPU_InitStruct.SubRegionDisable = 0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   //AHB SRAM (no cache)
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER3;
   MPU_InitStruct.BaseAddress = 0x30040000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
   MPU_InitStruct.SubRegionDisable = 0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   //SDRAM
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER4;
   MPU_InitStruct.BaseAddress = 0xD0000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
   MPU_InitStruct.SubRegionDisable = 0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   //LCD frame buffer
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER5;
   MPU_InitStruct.BaseAddress = 0xD0400000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_4MB;
   MPU_InitStruct.SubRegionDisable = 0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   //Enable MPU
   HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}


/**
 * @brief LED task
 * @param[in] param Unused parameter
 **/

void ledTask(void)
{
   //Endless loop
   if(systemTicks - ledStart >= ledTimeout)
   {
      if(ledState == 0)
      {
         BSP_LED_On(LED1);
         ledStart = systemTicks;
         ledTimeout = 100;
         ledState = 1;
      }
      else
      {
         BSP_LED_Off(LED1);
         ledStart = systemTicks;
         ledTimeout = 900;
         ledState = 0;
      }
   }
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   cboot_error_t cerror;

   //MPU configuration
   MPU_Config();
   //HAL library initialization
   HAL_Init();
   //Configure the system clock
   SystemClock_Config();

   //Enable I-cache and D-cache
   SCB_EnableICache();
   SCB_EnableDCache();

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
   TRACE_INFO("Target: STM32H753\r\n");
   TRACE_INFO("\r\n");

   //Initialize I/Os
   BSP_IO_Init();

   //LED configuration
   BSP_LED_Init(LED1);
   BSP_LED_Init(LED3);

   //Clear LEDs
   BSP_LED_Off(LED1);
   BSP_LED_Off(LED3);

   //Initialize user button
   BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

   //Initialize potentiometer
   BSP_POTENTIOMETER_Init();

   //Initialize LCD display
   BSP_LCD_Init();
   BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);
   BSP_LCD_SelectLayer(0);
   BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
   BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
   BSP_LCD_SetFont(&Font24);
   BSP_LCD_DisplayOn();

   //Clear LCD display
   BSP_LCD_Clear(LCD_COLOR_BLUE);

   //Welcome message
   lcdSetCursor(0, 0);
   printf("CycloneBOOT Single-Bank Bootloader\r\n");

   //Begining of handling exception
   do
   {
	   //Get default booloader user settings
	   bootGetDefaultSettings(&bootSettings);

      bootSettings.memories[0].memoryType = MEMORY_TYPE_FLASH;
      bootSettings.memories[0].memoryRole = MEMORY_ROLE_PRIMARY;
      bootSettings.memories[0].driver = &stm32h7xxFlashDriver;
      bootSettings.memories[0].nbSlots = 1;

      bootSettings.memories[0].slots[0].type = SLOT_TYPE_DIRECT;
      bootSettings.memories[0].slots[0].cType = SLOT_CONTENT_BINARY;
      bootSettings.memories[0].slots[0].memParent = &bootSettings.memories[0];
      bootSettings.memories[0].slots[0].addr = 0x08020000;
      bootSettings.memories[0].slots[0].size = 0x1E0000;

      bootSettings.memories[1].memoryType = MEMORY_TYPE_FLASH;
      bootSettings.memories[1].memoryRole = MEMORY_ROLE_SECONDARY;
      bootSettings.memories[1].driver = &mt25tl01gFlashDriver;
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

   //Endless loop
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
