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
File        : GUI_VNC_X_StartServer.c
Purpose     : Starts the VNC server via TCP/IP.
              This code works with embOS/IP.
              However, it can be easily modified to work with
              different kernel and IP Stack
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @file    GUI_VNC_X_StartServer.c
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    28-January-2014
  * @brief   Starts the VNC server via TCP/IP, it works with FreeRTOS and LwIP
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

#include <stdlib.h>

#include "lwip/sockets.h"
#include "string.h"

#include "bsp.h"

#include "GUI.h"
#include "GUI_VNC.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GUI_VNC_CONTEXT    _Context;
static struct sockaddr_in _Addr;

xTaskHandle            _VNCServer_TCB;
#define _VNCServer_PRIO          ( tskIDLE_PRIORITY  + 10 )
#define _VNCServer_Stack         ( 2048 )

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _Send
*
* Function description
*   This function is called indirectly by the server; it's address is passed to the actual
*   server code as function pointer. It is needed because the server is independent
*   of the TCP/IP stack implementation, so details for the TCP/IP stack can be placed here.
*/
static int _Send(const U8 * buf, int len, void * pConnectionInfo) {
  int r;

  r = send((long)pConnectionInfo, (const char *)buf, len, 0);
  return r;
}

/*********************************************************************
*
*       _Recv
*
* Function description
*   This function is called indirectly by the server; it's address is passed to the actual
*   server code as function pointer. It is needed because the server is independent
*   of the TCP/IP stack implementation, so details for the TCP/IP stack can be placed here.
*/
static int _Recv(U8 * buf, int len, void * pConnectionInfo) {
  return recv((long)pConnectionInfo, (char *)buf, len, 0);
}

/*********************************************************************
*
*       _ListenAtTcpAddr
*
* Starts listening at the given TCP port.
*/
static int _ListenAtTcpAddr(U16 Port) {
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(Port);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  listen(sock, 1);
  return sock;
}

/*********************************************************************
*
*       _ServerTask
*
* Function description
*   This routine is the actual server task.
*   It executes some one-time init code, then runs in an ednless loop.
*   It therefor does not terminate.
*   In the endless loop it
*     - Waits for a conection from a client
*     - Runs the server code
*     - Closes the connection
*/
static void _ServerTask(void * pvParameters)
{
  int s, Sock; 
  u32_t AddrLen;
  U16 Port;

  //
  // Prepare socket (one time setup)
  //
  Port = 5900 + _Context.ServerIndex; // Default port for VNC is is 590x, where x is the 0-based layer index
  //
  // Loop until we get a socket into listening state
  //
  do {
    s = _ListenAtTcpAddr(Port);
    if (s != -1) {
      break;
    }
    vTaskDelay(100); // Try again
  } while (1);
  //
  // Loop once per client and create a thread for the actual server
  //
  while (1) 
  {
    //
    // Wait for an incoming connection
    //
    AddrLen = sizeof(_Addr);
    Sock = accept(s, (struct sockaddr*)&_Addr, &AddrLen);
    //
    // Run the actual server
    //
    GUI_VNC_Process(&_Context, _Send, _Recv, (void *)Sock);
    //
    // Close the connection
    //
    closesocket(Sock);
    memset(&_Addr, 0, sizeof(struct sockaddr_in));
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_VNC_X_StartServer
*
* Function description
*   To start the server, the following steps are executed
*   - Make sure the TCP-IP stack is up and running
*   - Init the server context and attach it to the layer
*   - Start the thread (task) which runs the VNC server
* Notes:
*   (1) The first part of the code initializes the TCP/IP stack. In a typical
*       application, this is not required, since the stack should have already been
*       initialized some other place.
*       This could be done in a different module. (TCPIP_AssertInit() ?)
*/
int GUI_VNC_X_StartServer(int LayerIndex, int ServerIndex) {
  //
  // Init VNC context and attach to layer (so context is updated if the display-layer-contents change
  //
  GUI_VNC_AttachToLayer(&_Context, LayerIndex);
  _Context.ServerIndex = ServerIndex;
  //
  // Create task for VNC Server
  //
  xTaskCreate(_ServerTask,
              (signed char const*)"VNC_Server",
              _VNCServer_Stack,
              NULL,
              _VNCServer_PRIO,
              &_VNCServer_TCB);
  //
  // O.k., server has been started
  //  
  return 0;
}

/*********************************************************************
*
*       GUI_VNC_X_getpeername
*
* Function description
*   Retrieves the IP addr. of the currently connected VNC client.
*
*   Return values
*     IP addr. if VNC client connected
*     0 if no client connected
*/
void GUI_VNC_X_getpeername(U32 * Addr) {
  *Addr = _Addr.sin_addr.s_addr;
}

/*************************** End of file ****************************/

