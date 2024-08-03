    /**
  @page FSMC_NOR_CodeExecute FSMC NOR memory basic functionalities use example

  @verbatim
  ******************* (C) COPYRIGHT 2014 STMicroelectronics ********************
  * @file    FSMC/FSMC_NOR_CodeExecute/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   Description of the FSMC NOR example.
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

@par Example Description 

This directory contains a set of sources files that describes how to build an
application to be loaded into the NOR memory mounted on STM3210E-EVAL RevD board then
execute it from internal Flash.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

In order to make the program work, you must do the following:

1. Program the NOR memory with the example provided in the "binary" directory
@note You can use ST-LINK v3.4.6 tool and select NOR Flash memory in the external loader and
      then flash the binary file at @0x64000000.

2. Program the internal Flash with the code that will jump to the NOR memory to execute 
   the loaded example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

 - FSMC/NOR_CodeExecute/binary: Contains a set of sources files that build the 
                                application to be loaded into the NOR memory 
                                mounted on STM3210E-EVAL RevD board.
 - FSMC/FSMC_NOR_CodeExecute/Inc/stm32f1xx_hal_conf.h        HAL Configuration file
 - FSMC/FSMC_NOR_CodeExecute/Inc/main.h                      Header for main.c module 
 - FSMC/FSMC_NOR_CodeExecute/Inc/stm32f1xx_it.h              Header for stm32f1xx_it.c
 - FSMC/FSMC_NOR_CodeExecute/Src/main.c                      Main program
 - FSMC/FSMC_NOR_CodeExecute/Src/stm32f1xx_hal_msp.c         HAL MSP module  
 - FSMC/FSMC_NOR_CodeExecute/Src/stm32f1xx_it.c              Interrupt handlers
 - FSMC/FSMC_NOR_CodeExecute/Src/system_stm32f1xx.c          STM32F1xx system clock configuration file 
    

@par Hardware and Software environment 

  - This example runs on STM32F1xx devices.
    
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
