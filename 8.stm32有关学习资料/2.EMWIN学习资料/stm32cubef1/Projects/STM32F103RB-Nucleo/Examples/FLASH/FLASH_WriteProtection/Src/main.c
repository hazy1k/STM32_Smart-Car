/**
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   This example provides a description of how to set write protection on
  *          STM32F1xx FLASH.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup FLASH_WriteProtection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/

#define FLASH_USER_START_ADDR       ADDR_FLASH_PAGE_32   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR         ADDR_FLASH_PAGE_36   /* End @ of user Flash area */
#define FLASH_PAGE_TO_BE_PROTECTED (OB_WRP_PAGES32TO35 | OB_WRP_PAGES48TO51)  

#define DATA_32                     ((uint32_t)0x12345678)

/* Uncomment this line to program the Flash pages */
#define FLASH_PAGE_PROGRAM

/* Uncomment this line to Enable Write Protection */
/* #define WRITE_PROTECTION_ENABLE */

/* Uncomment this line to Disable Write Protection */
/* #define WRITE_PROTECTION_DISABLE */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Address = 0;
uint32_t PageError = 0;
uint32_t ProtectedPAGE = 0x0;
__IO TestStatus MemoryProgramStatus = PASSED;
/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
/*Variable used to handle the Options Bytes*/
static FLASH_OBProgramInitTypeDef OptionsBytesStruct;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F103xB HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Initialize LED2 */
  BSP_LED_Init(LED2);

  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
  
  /* Initialize test status */
  MemoryProgramStatus = PASSED;
  
  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  HAL_FLASH_OB_Unlock();

  /* Get pages write protection status ****************************************/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

#ifdef WRITE_PROTECTION_DISABLE
  /* Get pages already write protected ****************************************/
  ProtectedPAGE = ~(OptionsBytesStruct.WRPPage | FLASH_PAGE_TO_BE_PROTECTED);

  /* Check if desired pages are already write protected ***********************/
  if((OptionsBytesStruct.WRPPage | (~FLASH_PAGE_TO_BE_PROTECTED)) != 0xFFFFFFFF )
  {
    /* Erase all the option Bytes *********************************************/
    if(HAL_FLASHEx_OBErase() != HAL_OK)
    {
      /* Error occurred while options bytes erase. ****************************/
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_On(LED2);
        HAL_Delay(2000);
      }
    }
    
    /* Check if there is write protected pages ********************************/
    if(ProtectedPAGE != 0x0)
    {
    /* Restore write protected pages */
    OptionsBytesStruct.OptionType   = OPTIONBYTE_WRP;
      OptionsBytesStruct.WRPState   = OB_WRPSTATE_ENABLE;
      OptionsBytesStruct.WRPPage    = ProtectedPAGE;
    if(HAL_FLASHEx_OBProgram(&OptionsBytesStruct) != HAL_OK)
    {
      /* Error occurred while options bytes programming. **********************/
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in OB programming operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }
    }

    /* Generate System Reset to load the new option byte values ***************/
    HAL_FLASH_OB_Launch();
  }
#elif defined WRITE_PROTECTION_ENABLE
  /* Get current write protected pages and the new pages to be protected ******/
  ProtectedPAGE =  (~OptionsBytesStruct.WRPPage) | FLASH_PAGE_TO_BE_PROTECTED; 

  /* Check if desired pages are not yet write protected ***********************/
  if(((~OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED )!= FLASH_PAGE_TO_BE_PROTECTED)
  {
    /* Erase all the option Bytes because if a program operation is 
      performed on a protected page, the Flash memory returns a 
      protection error */
    if(HAL_FLASHEx_OBErase() != HAL_OK)
    {
      /* Error occurred while options bytes erase. ****************************/
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }
    /* Enable the pages write protection **************************************/
    OptionsBytesStruct.OptionType = OPTIONBYTE_WRP;
    OptionsBytesStruct.WRPState   = OB_WRPSTATE_ENABLE;
    OptionsBytesStruct.WRPPage    = ProtectedPAGE;
    if(HAL_FLASHEx_OBProgram(&OptionsBytesStruct) != HAL_OK)
    {
      /* Error occurred while options bytes programming. **********************/
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in OB programming operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }

    /* Generate System Reset to load the new option byte values ***************/
    HAL_FLASH_OB_Launch();
  }
#endif /* WRITE_PROTECTION_DISABLE */

  /* Lock the Options Bytes *************************************************/
  HAL_FLASH_OB_Lock();

#ifdef FLASH_PAGE_PROGRAM  
  /* The selected pages are not write protected *******************************/
  if ((OptionsBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != 0x00)
  {
    /* Fill EraseInit structure************************************************/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages     = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR)/FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
      /* 
        Error occurred while page erase. 
        User can add here some code to deal with this error. 
        PageError will contain the faulty page and then to know the code error on this page,
        user can call function 'HAL_FLASH_GetError()'
      */
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }

    /* FLASH Word program of DATA_32 at addresses defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
    Address = FLASH_USER_START_ADDR;
    while (Address < FLASH_USER_END_ADDR)
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
      {
        Address = Address + 4;
      }
      else
      {
        /* Error occurred while writing data in Flash memory. 
           User can add here some code to deal with this error */
        while (1)
        {
          /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
          BSP_LED_On(LED2);
          HAL_Delay(100);
          BSP_LED_Off(LED2);
          HAL_Delay(2000);
        }
      }
    }

    /* Check the correctness of written data */
    Address = FLASH_USER_START_ADDR;

    while (Address < FLASH_USER_END_ADDR)
    {
      if((*(__IO uint32_t*) Address) != DATA_32)
      {
        MemoryProgramStatus = FAILED;
      }
      Address += 4;
    }
  }
  else
  { 
    /* The desired pages are write protected */ 
    /* Check that it is not allowed to write in this page */
    Address = FLASH_USER_START_ADDR;
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) != HAL_OK)
    {
      /* Error returned during programmation. */
      /* Check that WRPERR flag is well set */
      if (HAL_FLASH_GetError() == HAL_FLASH_ERROR_WRP) 
      {
        MemoryProgramStatus = FAILED;
      }
      else
      {
        /* Another error occurred.
           User can add here some code to deal with this error */
        while (1)
        {
          /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
          BSP_LED_On(LED2);
          HAL_Delay(100);
          BSP_LED_Off(LED2);
          HAL_Delay(2000);
        }
      }
    }
    else
    {
      /* Write operation is successful. Should not occur
         User can add here some code to deal with this error */
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }
  }
#endif /* FLASH_PAGE_PROGRAM */

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /*Check if there is an issue to program data*/
  if (MemoryProgramStatus == PASSED)
  {
    /* No error detected. Switch on LED2*/
    BSP_LED_On(LED2);
  }
  else
  {
    /* Error detected. LED2 will blink with 1s period */
    while (1)
    {
      BSP_LED_On(LED2);
      HAL_Delay(1000);
      BSP_LED_Off(LED2);
      HAL_Delay(1000);
    }
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 16
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Configure PLL ------------------------------------------------------*/
  /* PLL configuration: PLLCLK = (HSI / 2) * PLLMUL = (8 / 2) * 16 = 64 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLLCLK / HSEPredivValue = 64 / 1 = 64 MHz */
  /* Enable HSI and activate PLL with HSi_DIV2 as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI;
  oscinitstruct.HSEState        = RCC_HSE_OFF;
  oscinitstruct.LSEState        = RCC_LSE_OFF;
  oscinitstruct.HSIState        = RCC_HSI_ON;
  oscinitstruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  oscinitstruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSI_DIV2;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
