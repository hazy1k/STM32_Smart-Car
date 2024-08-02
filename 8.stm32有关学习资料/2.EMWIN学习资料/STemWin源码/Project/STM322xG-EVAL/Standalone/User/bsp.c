/**
  ******************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    28-January-2014
  * @brief   This file provides targets hardware configuration 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t BSP_TSC_Init(void);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Initializes the target hardware.
* @param  None
* @retval None
*/
uint32_t BSP_Init (void)
{
  /* Setup SysTick Timer for 1 msec interrupts */
  SysTick_Config(SystemCoreClock / 1000);
  
  /* Initialize the LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);  

  BSP_TSC_Init();
  
  STM322xG_LCD_Init();

  /* Initialize the SRAM */
  SRAM_Init();

  /* Enable the CRC Module */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);  
  return 0;
}


/**
  * @brief  Initializes the IO Expander registers.
  * @param  None
  * @retval - 0: if all initializations are OK.
*/
static uint32_t BSP_TSC_Init(void)
{
  __disable_irq();    
  IOE_Config();
  __enable_irq();   
  return 0; /* Configuration is OK */
}

/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{
  GUI_PID_STATE TS_State;
  TS_STATE  *ts;

  ts = IOE_TS_GetState(); 

  TS_State.x = ts->X;
  TS_State.y = ts->Y;
  TS_State.Pressed = (ts->TouchDetected == 0x80);
  TS_State.Layer = 0;
  
  if((TS_State.x > 0 ) && 
     (TS_State.x < LCD_GetXSize())&&
     (TS_State.y > 0 ) && 
     (TS_State.y < LCD_GetYSize()))
    
  {
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
