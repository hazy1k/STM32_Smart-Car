/**
  ******************************************************************************
  * @file    TIM/TIM_DMA/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   This sample code shows how to use DMA with TIM1 Update request to
  *          transfer Data from memory to TIM1 Capture Compare Register 3 (CCR3).
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

/** @addtogroup TIM_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Capture Compare buffer */
uint32_t aCCValue_Buffer[3] = {0, 0, 0};

/* Timer Period*/
uint32_t uwTimerPeriod  = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to use DMA with TIM1 Update request to transfer
  Data from memory to TIM1 Capture Compare Register 3 (CCR3), through the
  STM32F1xx HAL API. To proceed, 3 steps are required */

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

  /* Configure the system clock to 64 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Compute the value of ARR regiter to generate signal frequency at 17.57 Khz */
  uwTimerPeriod = (uint32_t)((SystemCoreClock / 17570) - 1);
  /* Compute CCR1 value to generate a duty cycle at 75% */
  aCCValue_Buffer[0] = (uint32_t)(((uint32_t) 75 * (uwTimerPeriod - 1)) / 100);
  /* Compute CCR2 value to generate a duty cycle at 50% */
  aCCValue_Buffer[1] = (uint32_t)(((uint32_t) 50 * (uwTimerPeriod - 1)) / 100);
  /* Compute CCR3 value to generate a duty cycle at 25% */
  aCCValue_Buffer[2] = (uint32_t)(((uint32_t) 25 * (uwTimerPeriod - 1)) / 100);

  /*##-1- Configure the TIM peripheral #######################################*/
  /* ---------------------------------------------------------------------------
  TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2), since APB2
    prescaler is 1.
    TIM1CLK = PCLK2
    PCLK2 = HCLK
    => TIM1CLK = HCLK = SystemCoreClock

  TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
  SystemCoreClock is set to 64 MHz for STM32F1xx devices.

  The objective is to configure TIM1 channel 3 to generate a PWM
  signal with a frequency equal to 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
  and a variable duty cycle that is changed by the DMA after a specific number of
  Update DMA request.

  The number of this repetitive requests is defined by the TIM1 Repetion counter,
  each 4 Update Requests, the TIM1 Channel 3 Duty Cycle changes to the next new
  value defined by the aCCValue_Buffer.

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f1xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  -----------------------------------------------------------------------------*/
  /* Initialize TIM1 peripheral as follows:
      + Period = TimerPeriod (To have an output frequency equal to 17.570 KHz)
      + Repetition Counter = 3
      + Prescaler = 0
      + ClockDivision = 0
      + Counter direction = Up
  */
  TimHandle.Instance = TIMx;

  TimHandle.Init.Period            = uwTimerPeriod;
  TimHandle.Init.RepetitionCounter = 3;
  TimHandle.Init.Prescaler         = 0;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channel 3 ########################################*/
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.Pulse        = aCCValue_Buffer[0];
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*##-3- Start PWM signal generation in DMA mode ############################*/
  if (HAL_TIM_PWM_Start_DMA(&TimHandle, TIM_CHANNEL_3, aCCValue_Buffer, 3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  while (1)
  {
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
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
