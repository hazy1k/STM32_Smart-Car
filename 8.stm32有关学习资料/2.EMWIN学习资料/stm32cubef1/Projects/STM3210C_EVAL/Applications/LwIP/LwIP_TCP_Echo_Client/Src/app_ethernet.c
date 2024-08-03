/**
  ******************************************************************************
  * @file    LwIP/LwIP_TCP_Echo_Client/Src/app_ethernet.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   Ethernet specefic module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "lwip/opt.h"
#include "main.h"
#include "app_ethernet.h"
#include "ethernetif.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Notify the User about the nework interface config status 
  * @param  netif: the network interface
  * @retval None
  */
void User_notification(struct netif *netif) 
{
  if (netif_is_up(netif))
  {
    /* Turn On LED 1 to indicate ETH and LwIP init success*/
    BSP_LED_On(LED1);
  }
  else
  {     
    /* Turn On LED 2 to indicate ETH and LwIP init error */
    BSP_LED_On(LED2);
  } 
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
