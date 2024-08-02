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
#ifdef USE_MB1063 
 __IO uint32_t TS_Pressed;
#endif

/* Private function prototypes -----------------------------------------------*/
uint32_t BSP_TSC_Init(void);

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

  /*Init Touchscreen */
  BSP_TSC_Init();
  
  /* Initialize the SDRAM */
  SDRAM_Init();

  /* Enable the CRC Module */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);  
  return 0;
}


/**
  * @brief  Initializes the IO Expander registers.
  * @param  None
  * @retval - 0: if all initializations are OK.
*/
uint32_t BSP_TSC_Init(void)
{
#ifdef USE_MB1046 
  __disable_irq();   
  IOE_Config();
  __enable_irq ();
  return 0; /* Configuration is OK */
#elif defined (USE_MB1063)
  uint32_t ret = 1;
  
  TS_Pressed = 0;
  
  if (IOE16_Config() == IOE16_OK)
  {
    /* Enable Interrupt */
    IOE16_ITConfig(IOE16_TS_IT);
    /* Read GPMR register */
    IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_LSB);  
    ret = 0;
  }
  return ret; 
#endif
 
  
}


/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{
  GUI_PID_STATE TS_State;

#ifdef USE_MB1063 
  uint32_t xDiff, yDiff , x , y;  
  uint8_t Buffer[11];
  static uint32_t _x = 0, _y = 0;
  
  LCD_GetPosition(Buffer);
  
  x = Buffer[2] | Buffer[1] << 8;
  y = Buffer[4] | Buffer[3] << 8;
  
  if(((x == 0xFFFF ) && 
      (y == 0xFFFF ))||
     ((x == 0 ) && 
      (y == 0 )))    
  {
	  TS_Pressed = 0;  
  }
  
  if(TS_Pressed)
  {
    x = Buffer[2] | Buffer[1] << 8;
    y = Buffer[4] | Buffer[3] << 8;
    xDiff = x > _x? (x - _x): (_x - x);
    yDiff = y > _y? (y - _y): (_y - y);       
	if ((xDiff + yDiff > 2))
    {
      _x = x;
      _y = y;       
    }
  }
     TS_State.x = _x;
     TS_State.y = _y;
	 TS_State.Pressed = TS_Pressed;
#else
  TS_STATE  *ts;
    
  ts = IOE_TS_GetState();

  TS_State.x = ts->X;
  TS_State.y = ts->Y;
  TS_State.Pressed = (ts->TouchDetected == 0x80);

  TS_State.x -= 10;
  TS_State.y = 271 - TS_State.y;
#endif 
  
  TS_State.Layer = 0;

  if((TS_State.x >= 0 ) && 
     (TS_State.x < LCD_GetXSize())&&
     (TS_State.y >= 0 ) && 
     (TS_State.y < LCD_GetYSize()))
    
  {
    GUI_TOUCH_StoreStateEx(&TS_State);  
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
