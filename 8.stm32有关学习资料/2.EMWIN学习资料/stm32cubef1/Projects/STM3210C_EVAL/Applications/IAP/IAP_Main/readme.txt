/**
  @page In-Application Programming (IAP) using the USART Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    Applications/IAP/IAP_Main/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   Description of the In-Application Programming (IAP) using the USART.
  ******************************************************************************
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
  @endverbatim

@par Description

This directory contains a set of sources files and pre-configured projects that 
build the In-Application Programming (IAP) through USART program.


@par Directory contents 

 - "IAP/IAP_Main": contains firmware source files for main program
 - "IAP/IAP_Main/Inc": contains the IAP firmware header files 
    - IAP/IAP_Main/Inc/common.h            This file provides all the headers of the common functions.
    - IAP/IAP_Main/Inc/flash_if.h          This file provides all the firmware function headers of the flash_if.c file.
    - IAP/IAP_Main/Inc/menu.h              This file provides all the firmware function headers of the menu.c file.
    - IAP/IAP_Main/Inc/ymodem.h            This file provides all the firmware function headers of the ymodem.c file.
    - IAP/IAP_Main/Inc/stm32f1xx_conf.h    Library Configuration file
    - IAP/IAP_Main/Inc/stm32f1xx_it.h      Header for stm32f1xx_it.c
 - "IAP/IAP_Main/Src": contains the IAP firmware source files
    - IAP/IAP_Main/Src/main.c              Main program
    - IAP/IAP_Main/Src/stm32f1xx_it.c      Interrupt handlers
    - IAP/IAP_Main/Src/flash_if.c          This file provides all the memory related operation functions(write, erase and
                                             write protection disable)
    - IAP/IAP_Main/Src/menu.c              This file contains the menu to select downloading a new binary file, uploading
                                             internal Flash memory, executing the binary and disabling the write protection of
                                             write protected sectors.
    - IAP/IAP_Main/Src/common.c            This file provides functions related to read/write from/to USART peripheral.
    - IAP/IAP_Main/Src/ymodem.c            This file provides all the firmware functions related to the ymodem protocol.
    - IAP/IAP_Main/Src/system_stm32f1xx.c  STM32F1xx system source file.
    
 - "IAP/IAP_Binary_Template": contains the binary template firmware (to be downloaded through USART) source files

 - "IAP/IAP_Main/EWARM": contains preconfigured project for EWARM toolchain (main program)
 - "IAP/IAP_Binary_Template/EWARM": contains preconfigured project for EWARM toolchain (binary template)
 - "IAP/IAP_Main/MDK-ARM": contains preconfigured project for MDK-ARM toolchain (main program)
 - "IAP/IAP_Binary_Template/MDK-ARM": contains preconfigured project for MDK-ARM toolchain (binary template)
 - "IAP/IAP_Main/TrueSTUDIO": contains preconfigured project for TrueSTUDIO toolchain (main program)
 - "IAP/IAP_Binary_Template/TrueSTUDIO": contains preconfigured project for TrueSTUDIO toolchain (binary template)
 

@par Hardware and Software environment

  - This example runs on STM32F1xx Devices.
  
  - This example has been tested with STM3210C-EVAL RevC and can be easily 
    tailored to any other development board.
    
  - STM3210C-EVAL RevC Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 and PC serial port(make sure that jumper JP16 is in position 2-3).
    - Use the Key push-button.

  - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following:

  1. Generate a binary image using the preconfigured project provided in the directory
      "Applications/IAP/IAP_Binary_Template" . 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP application, keep the Key push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

  In order to load the IAP code, you have do the following:
   - EWARM
      - Open the Project.eww workspace
      - Rebuild all files: Project->Rebuild all
      - Load project image: Project->Debug
      - Run program: Debug->Go(F5)

   - MDK-ARM
      - Open the Project.uvproj project
      - Rebuild all files: Project->Rebuild all target files
      - Load project image: Debug->Start/Stop Debug Session
      - Run program: Debug->Run (F5)

   - TrueSTUDO
      - Open the TrueSTUDIO toolchain.
      - Browse to the TrueSTUDIO workspace directory, select the project (.project file in \IAP_Main\TrueSTUDIO\STM3210C_EVAL directory).
      - Rebuild all project files: Select the project in the "Project explorer" 
        window then click on Project->build project menu.
      - Run program: Run->Debug (F11)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
