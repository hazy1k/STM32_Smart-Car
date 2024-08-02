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
 #define KEY_STAT_UP       (1 << 0)
 #define KEY_STAT_DOWN     (1 << 1)
 #define KEY_STAT_LEFT     (1 << 2)
 #define KEY_STAT_RIGHT    (1 << 3)
 #define KEY_STAT_BUTTON1  (1 << 4)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

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

  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
  
  /* Enable the CRC Module */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

  return 0;
}


/**
  * @brief  
  * @param  None
  * @retval 
  */
uint32_t BSP_GetKeyState(void)
{
  uint32_t r;

  r = 0;

  if (STM_EVAL_PBGetState(BUTTON_DOWN) == 0) {
    r |= KEY_STAT_DOWN;      // Joystick down
  }
  if (STM_EVAL_PBGetState(BUTTON_LEFT) == 0) {
    r |= KEY_STAT_LEFT;      // Joystick left
  }
  if (STM_EVAL_PBGetState(BUTTON_RIGHT) == 0) {
    r |= KEY_STAT_RIGHT;     // Joystick right
  }
  if (STM_EVAL_PBGetState(BUTTON_UP) == 0) {
    r |= KEY_STAT_UP;        // Joystick up
  }
  if (STM_EVAL_PBGetState(BUTTON_SEL) == 0) {
    r |= KEY_STAT_BUTTON1;   // Joystick select
  }
  return r;
}

/**
  * @brief  Provide the GUI with current state of the joystick
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{
  uint32_t KeyState;
  
  GUI_PID_STATE State = {0};
  KeyState = BSP_GetKeyState();
  if (KeyState & KEY_STAT_UP) {
    State.y = -1;
  }
  if (KeyState & KEY_STAT_DOWN) {
    State.y = 1;
  }
  if (KeyState & KEY_STAT_LEFT) {
    State.x = -1;
  }
  if (KeyState & KEY_STAT_RIGHT) {
    State.x = 1;
  }
  if (KeyState & KEY_STAT_BUTTON1) {
    State.Pressed = 1;
  }
  GUI_JOYSTICK_StoreState(&State);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
