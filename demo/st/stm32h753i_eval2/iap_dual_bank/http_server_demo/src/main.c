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
#include "core/net.h"
#include "drivers/mac/stm32h7xx_eth_driver.h"
#include "drivers/phy/lan8742_driver.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "hardware/stm32h7xx/stm32h7xx_crypto.h"
#include "rng/trng.h"
#include "rng/yarrow.h"
#include "path.h"
#include "date_time.h"
#include "resource_manager.h"
#include "boot_config.h"
#include "http_server_callbacks.h"
#include "version.h"
#include "debug.h"

//LCD frame buffers
#define LCD_FRAME_BUFFER_LAYER0 0xD0400000
#define LCD_FRAME_BUFFER_LAYER1 0xD0480000

//Ethernet interface configuration
#define APP_IF_NAME "eth0"
#define APP_HOST_NAME "http-server-demo"
#define APP_MAC_ADDR "00-AB-CD-EF-07-69"

#define APP_USE_DHCP_CLIENT ENABLED
#define APP_IPV4_HOST_ADDR "192.168.0.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.0.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::769"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::769"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

//Application configuration
#define APP_HTTP_MAX_CONNECTIONS 8

//Update signature public key resource path
#define UPDT_PUBLIC_SIGN_KEY "keys/rsa_public_key.pem"

//Global variables
uint_t lcdLine = 0;
uint_t lcdColumn = 0;

DhcpClientSettings dhcpClientSettings;
DhcpClientContext dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;
HttpServerSettings httpServerSettings;
HttpServerContext httpServerContext;
HttpConnection httpConnections[APP_HTTP_MAX_CONNECTIONS];

//Update signature public key declaration
const uint8_t* pemUpdtSignPublicKey;
size_t pemUpdtSignPublicKeyLen;

//Update mutex handler
OsMutex updateMutex;


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
   NetInterface *interface;
   MacAddr macAddr;
#if (APP_USE_DHCP_CLIENT == DISABLED)
   Ipv4Addr ipv4Addr;
#endif
#if (APP_USE_SLAAC == DISABLED)
   Ipv6Addr ipv6Addr;
#endif

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
   TRACE_INFO("**************************************************\r\n");
   TRACE_INFO("*** CycloneBOOT IAP Dual-Bank HTTP Server Demo ***\r\n");
   TRACE_INFO("**************************************************\r\n");
   TRACE_INFO("Version: %s\r\n", APP_VERSION_STRING);
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
   printf("CycloneBOOT HTTP Serveur Dual-Bank Demo\r\n");

   //Initialize hardware cryptographic accelerator
   error = stm32h7xxCryptoInit();
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize hardware crypto accelerator!\r\n");
   }

   //TCP/IP stack initialization
   error = netInit();
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
   }

   //Configure the first Ethernet interface
   interface = &netInterface[0];

   //Set interface name
   netSetInterfaceName(interface, APP_IF_NAME);
   //Set host name
   netSetHostname(interface, APP_HOST_NAME);
   //Set host MAC address
   macStringToAddr(APP_MAC_ADDR, &macAddr);
   netSetMacAddr(interface, &macAddr);
   //Select the relevant network adapter
   netSetDriver(interface, &stm32h7xxEthDriver);
   netSetPhyDriver(interface, &lan8742PhyDriver);

   //Initialize network interface
   error = netConfigInterface(interface);
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
   }

#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP_CLIENT == ENABLED)
   //Get default settings
   dhcpClientGetDefaultSettings(&dhcpClientSettings);
   //Set the network interface to be configured by DHCP
   dhcpClientSettings.interface = interface;
   //Disable rapid commit option
   dhcpClientSettings.rapidCommit = FALSE;

   //DHCP client initialization
   error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
   //Failed to initialize DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize DHCP client!\r\n");
   }

   //Start DHCP client
   error = dhcpClientStart(&dhcpClientContext);
   //Failed to start DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start DHCP client!\r\n");
   }
#else
   //Set IPv4 host address
   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
   ipv4SetHostAddr(interface, ipv4Addr);

   //Set subnet mask
   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
   ipv4SetSubnetMask(interface, ipv4Addr);

   //Set default gateway
   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   ipv4SetDefaultGateway(interface, ipv4Addr);

   //Set primary and secondary DNS servers
   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 0, ipv4Addr);
   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (APP_USE_SLAAC == ENABLED)
   //Get default settings
   slaacGetDefaultSettings(&slaacSettings);
   //Set the network interface to be configured
   slaacSettings.interface = interface;

   //SLAAC initialization
   error = slaacInit(&slaacContext, &slaacSettings);
   //Failed to initialize SLAAC?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize SLAAC!\r\n");
   }

   //Start IPv6 address autoconfiguration process
   error = slaacStart(&slaacContext);
   //Failed to start SLAAC process?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start SLAAC!\r\n");
   }
#else
   //Set link-local address
   ipv6StringToAddr(APP_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   ipv6SetLinkLocalAddr(interface, &ipv6Addr);

   //Set IPv6 prefix
   ipv6StringToAddr(APP_IPV6_PREFIX, &ipv6Addr);
   ipv6SetPrefix(interface, 0, &ipv6Addr, APP_IPV6_PREFIX_LENGTH);

   //Set global address
   ipv6StringToAddr(APP_IPV6_GLOBAL_ADDR, &ipv6Addr);
   ipv6SetGlobalAddr(interface, 0, &ipv6Addr);

   //Set default router
   ipv6StringToAddr(APP_IPV6_ROUTER, &ipv6Addr);
   ipv6SetDefaultRouter(interface, 0, &ipv6Addr);

   //Set primary and secondary DNS servers
   ipv6StringToAddr(APP_IPV6_PRIMARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 0, &ipv6Addr);
   ipv6StringToAddr(APP_IPV6_SECONDARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 1, &ipv6Addr);
#endif
#endif

   //Get default settings
   httpServerGetDefaultSettings(&httpServerSettings);
   //Bind HTTP server to the desired interface
   httpServerSettings.interface = &netInterface[0];
   //Listen to port 80
   httpServerSettings.port = HTTP_PORT;
   //Client connections
   httpServerSettings.maxConnections = APP_HTTP_MAX_CONNECTIONS;
   httpServerSettings.connections = httpConnections;
   //Specify the server's root directory
   strcpy(httpServerSettings.rootDirectory, "/www/");
   //Set default home page
   strcpy(httpServerSettings.defaultDocument, "index.html");
   //Callback functions
   httpServerSettings.requestCallback = httpServerRequestCallback;
   httpServerSettings.uriNotFoundCallback = httpServerUriNotFoundCallback;

   //HTTP server initialization
   error = httpServerInit(&httpServerContext, &httpServerSettings);
   //Failed to initialize HTTP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize HTTP server!\r\n");
   }

   //Start HTTP server
   error = httpServerStart(&httpServerContext);
   //Failed to start HTTP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start HTTP server!\r\n");
   }

   //Load Update public signature key
   error = updateUserLoadKeys();
   if (error)
   {
	  //Debug message
      TRACE_ERROR("Failed to load Update user keys!\r\n");
      return error;
   }

   //Create Update mutex
   if(!osCreateMutex(&updateMutex))
   {
      //Debug message
      TRACE_ERROR("Failed to create Update mutex!\r\n");
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
