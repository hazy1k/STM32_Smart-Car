/**
  @page IAP_Binary_Template IAP Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    Applications/IAP/IAP_Binary_Template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   Description of the IAP Binary Template directory.
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

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP through USART).

Generated binary file will be loaded using main IAP program loaded in 0x8000000
(please refer to IAP/IAP_Main for details on this Main program).

To build such application, some special configuration has to be performed:
1. Set the program load address at 0x08004000, using your toolchain linker file
2. Relocate the vector table at address 0x08004000, by modifying the value of the constant
   "VECT_TAB_OFFSET" defined in system_stm32f1xx.c file. 

The SysTick example provided within the STM32F1xx HAL Cube examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "IAP/IAP_Binary_Template/EWARM"
 - "IAP/IAP_Binary_Template/MDK-ARM"
 - "IAP/IAP_Binary_Template/TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "IAP/IAP_Binary_Template/Inc": contains the IAP Binary Template firmware header files 
     - IAP/IAP_Binary_Template/Inc/stm32f1xx_conf.h    Library Configuration file
     - IAP/IAP_Binary_Template/Inc/stm32f1xx_it.h      Header for stm32f1xx_it.c
     - IAP/IAP_Binary_Template/Inc/main.h              Header for main.c

 - "IAP/IAP_Binary_Template/Src": contains the IAP Binary Template firmware source files 
     - IAP/IAP_Binary_Template/Src/main.c              Main program
     - IAP/IAP_Binary_Template/Src/stm32f1xx_it.c      Interrupt handlers
     - IAP/IAP_Binary_Template/Src/system_stm32f1xx.c  STM32F1xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F1xx Devices.
  
  - This example has been tested with STM3210C-EVAL RevC and can be easily 
    tailored to any other development board.
    
  - STM3210C_EVAL Set-up :
     Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PD.03
     and PD.04 pins

@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - Go to "/IAP_Binary_Template/EWARM" directory 
    - A binary file "STM3210C_EVAL_SysTick.bin" will be generated under 
      "STM3210C_EVAL/Exe" folder.  
    - Load this image with the IAP application (Select option 1 in IAP menu)

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "/IAP_Binary_Template/MDK-ARM" directory and run "axftobin.bat"
	  (Fromelf Exe path might have to be updated in "axftobin.bat" file, according to your Keil setup).
    - A binary file "STM3210C_EVAL_SysTick.bin" will be generated under this
      directory location (reference file is also present in delivered package). 
    - Load this image with the IAP application (Select option 1 in IAP menu)

 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Browse to the TrueSTUDIO workspace directory, select the project (.project file in \IAP_Binary_Template\TrueSTUDIO\STM3210C_EVAL directory).
    - Rebuild all project files: Select the project in the "Project explorer" window then click on Project->build project menu.
    - Go to "\IAP_Binary_Template\TrueSTUDIO\STM3210C_EVAL" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM3210C_EVAL_SysTick.bin" will be generated under this directory location.
    - Load this image with the IAP application (Select option 1 in IAP menu)


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
