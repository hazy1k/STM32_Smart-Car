/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    28-January-2014
  * @brief   This file provides main program functions
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
#include "timers.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Background_Task_PRIO    ( tskIDLE_PRIORITY  + 9 )
#define Background_Task_STACK   ( 512 )

#define Demo_Task_PRIO          ( tskIDLE_PRIORITY  + 8 )
#define Demo_Task_STACK         ( 512 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
xTaskHandle                   Task_Handle;
xTaskHandle                   Demo_Handle;
xTimerHandle                  TouchScreenTimer;

/* Private function prototypes -----------------------------------------------*/
static void Background_Task(void * pvParameters);
static void Demo_Task(void * pvParameters);
static void vTimerCallback( xTimerHandle pxTimer );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main(void)
{

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* Create background task */
  xTaskCreate(Background_Task,
              (signed char const*)"BK_GND",
              Background_Task_STACK,
              NULL,
              Background_Task_PRIO,
              &Task_Handle);

    /* Create demo task */
  xTaskCreate(Demo_Task,
              (signed char const*)"GUI_DEMO",
              Demo_Task_STACK,
              NULL,
              Demo_Task_PRIO,
              &Demo_Handle);
  
  /* Launch Touchscreen Timer */
  TouchScreenTimer = xTimerCreate ("Timer", 50, pdTRUE, ( void * ) 1, vTimerCallback );
  
  if( TouchScreenTimer != NULL )
  {
    if( xTimerStart( TouchScreenTimer, 0 ) != pdPASS )
    {
      /* The timer could not be set into the Active state. */
    }
  }
  
  /* Start the FreeRTOS scheduler */
  vTaskStartScheduler();
}


/**
  * @brief  Background task
  * @param  pvParameters not used
  * @retval None
  */
static void Background_Task(void * pvParameters)
{
 
  /* Initilize the BSP layer */
  BSP_Init();
  
  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  
  /* Init the STemWin GUI Library */
  GUI_Init();
    
  /* Run the background task */
  while (1)
  {
    /* toggle LED1..4 each 250ms */
    STM_EVAL_LEDToggle(LED1);
    STM_EVAL_LEDToggle(LED2);
    STM_EVAL_LEDToggle(LED3);
    STM_EVAL_LEDToggle(LED4);

    GUI_Delay(250);
  }
}

/**
  * @brief  Demonstration task
  * @param  pvParameters not used
  * @retval None
  */
static void Demo_Task(void * pvParameters)
{
  /* Start Demo */
  GUIDEMO_Main();
}

/**
  * @brief  Timer callbacsk
  * @param  pxTimer 
  * @retval None
  */
static void vTimerCallback( xTimerHandle pxTimer )
{
   BSP_Pointer_Update();
}

/**
  * @brief  Error callback function
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook( void )
{
  while (1)
  {}
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
