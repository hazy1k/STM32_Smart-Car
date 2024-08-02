/*********************************************************************
*          Portions COPYRIGHT 2014 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Intro.c
Purpose     : Introduction for emWin generic demo
              (This is also a good file to demo and explain basic
              emWin features by setting breakpoints)
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO_Intro.c
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    28-January-2014
  * @brief   Introduction for emWin generic demo
  ******************************************************************************
  * @attention
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


#include <string.h>

#include "GUIDEMO.h"
#include "GUI_VNC.h"
#include "main.h"
#include "bsp.h"

extern __IO uint8_t DHCP_state;
extern uint8_t iptxt[20];
extern xTaskHandle Task_Handle;
extern xTaskHandle Demo_Handle;

/*********************************************************************
*
*       Defines
*/
#define SCREEN_DIV          6 // 2^6 = 64

#define FACTOR_EMWIN        4
#define FACTOR_DESC        11
#define FACTOR_ANY_COMP    22
#define FACTOR_VERSION     31
#define FACTOR_LOGO        38
#define FACTOR_WWW         56

#define DIST_ANY_COMP      18

  

#define DHCP_TASK_PRIO          ( tskIDLE_PRIORITY + 7 )

static void _UpdateVNCFrame(void) ;
/*********************************************************************
*
*       GUIDEMO_Intro
*
**********************************************************************
*/
void GUIDEMO_Vnc(void) {
  
  int  xCenter, yCenter;
  int  VNC_Initialized = 0;
  int  DHCP_Done = 0;
  
  xCenter   = LCD_GetXSize() / 2;
  yCenter   = LCD_GetYSize() / 2;
  
  GUIDEMO_HideControlWin();
  GUIDEMO_SetControlText("vnc");

  _UpdateVNCFrame();        

  while (1)
  {
    if(VNC_Initialized == 0)
    {
      VNC_Initialized = 1;


      GUI_DispStringHCenterAt("Checking for cable connection...\n CAUTION: This can take long time.\n", xCenter, yCenter - 22 + 60);
      /* configure ethernet MAC */ 
      ETH_MAC_Config();

      /* Initilaize the LwIP stack */
      LwIP_Init();
      
#ifdef USE_DHCP
      /* Start DHCPClient */
      xTaskCreate(LwIP_DHCP_task, "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
#endif
      
      /*Start VNC*/
      GUI_VNC_X_StartServer(0,0);
    }
    else
    {
      switch (DHCP_state)
      {
#ifdef USE_DHCP
      case DHCP_WAIT_ADDRESS:
        {
          if(DHCP_Done == 0)
          {
          _UpdateVNCFrame();
           GUI_SetFont(&GUI_Font16_ASCII);
           GUI_DispStringHCenterAt("Waiting for a DHCP Address...\n", xCenter, yCenter - 22 + 60);
           DHCP_Done = 1;
          }
        }
        break;
      case DHCP_ADDRESS_ASSIGNED:
        {
          _UpdateVNCFrame();
          GUI_SetFont(&GUI_Font16_ASCII);
          GUI_DispStringHCenterAt("DHCP Address assigned : \n", xCenter, yCenter - 22 + 60);          
          GUI_SetFont(&GUI_FontRounded22);
          GUI_DispStringHCenterAt((const char *)iptxt, xCenter, yCenter - 22 + 120);
        }
        break;
      case DHCP_TIMEOUT:
        {
          _UpdateVNCFrame();  
          GUI_SetFont(&GUI_Font16_ASCII);
          GUI_DispStringHCenterAt("Error : DHCP Failed\n", xCenter, yCenter - 22 + 60);          
        }
        break;
#endif /* USE_DHCP */
      case DHCP_LINK_DOWN:
        {
          _UpdateVNCFrame();          
          GUI_SetFont(&GUI_Font16_ASCII);
          GUI_DispStringHCenterAt("Ethernet cable is not connected\n", xCenter, yCenter - 22 + 60);
        }
        break; 
      case STATIC_IP:
        {
          _UpdateVNCFrame();          
          GUI_SetFont(&GUI_FontRounded22);
          GUI_DispStringHCenterAt((const char *)iptxt, xCenter, yCenter - 22 + 120);
        }
        break; 
      default: 
        break;
      }
      if ((DHCP_state == DHCP_ADDRESS_ASSIGNED) || (DHCP_state == DHCP_TIMEOUT) || 
          (DHCP_state == STATIC_IP) || (DHCP_state == DHCP_LINK_DOWN))
      {
        GUIDEMO_ShowControlWin();
        break;
      }
      GUIDEMO_Delay(100);
    }
  }
  GUIDEMO_Delay(5000);
}

/*********************************************************************
*
*       _UpdateVNCFrame
*
**********************************************************************
*/
static void _UpdateVNCFrame(void) 
{
  int  xCenter, yCenter;
  
  xCenter   = LCD_GetXSize() / 2;
  yCenter   = LCD_GetYSize() / 2;
  
  GUIDEMO_DrawBk(1);

  GUI_SetColor(GUI_WHITE);
  /*Title*/
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontRounded22);
  GUI_DispStringHCenterAt("VNC Server", xCenter, 60);
  
  GUI_SetFont(&GUI_Font16_ASCII);
  GUI_DispStringHCenterAt("Use a VNC viewer to display and \n control current Demo on PC \n",
                          xCenter, 
                          yCenter - 22 + 10);
}
/*************************** End of file ****************************/
