/**
  @page RTC_LSI RTC prescaler adjustment with LSI Measurements example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    RTC/RTC_LSI/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   Description of the RTC prescaler adjustment with LSI Measurements example.
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

This example demonstrates and explains how to use the LSI clock source auto
calibration to get a precise RTC clock. 
In the associated software, the system clock is set to 72 MHz.
As an application example, it demonstrates how to configure the TIM5 timer
internally connected to LSI clock output, in order to adjust the RTC prescaler. 

The Low Speed Internal (LSI) clock is used as RTC clock source.
After reset, the RTC prescaler is set with the default LSI frequency (40 kHz). 
The RTC WakeUp is configured to generate an interrupt each 1s.

A key value is written in backup data register 1 to indicate if the wakeup timer has
already been enabled. If so the wakeup timer is disabled prior configuring the RTC 
automatic wakeup.

LED1 is toggled inside the RTC WakeUp interrupt each 1s.

The inaccuracy of the LSI clock causes the RTC WakeUp Interrupt to be inaccurate.
The RTC CK_SPRE signal can be monitored by LED1 which is toggled into the RTC 
Wakeup interrupt service routine.

The program waits until Key push-button is pressed to begin the auto calibration 
procedure:
 - Configure the TIM5 to remap internally the TIM5 TIM_CHANNEL_4
   Input Capture to the LSI clock output.
 - Enable the TIM5 Input Capture interrupt: after one cycle of LSI clock, the
   period value is stored in a variable and compared to the HCLK clock to get
   its real value.
 - The RTC prescaler is adjusted with this LSI frequency value so that the RTC
   CK_SPRE value become more accurate.
 - When calibration is done the variable uwLsiFreq is visualized into the debugger
   to indicate the end of this operation.
 - The LSI value is updated and displayed on the debugger in uwLsiFreq variable.

- LED3 is On: This indicates that the system generates an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents  

  - RTC/RTC_LSI/Inc/stm32f1xx_conf.h         Library Configuration file
  - RTC/RTC_LSI/Inc/main.h                   header file for main.c
  - RTC/RTC_LSI/Inc/stm32f1xx_it.h           Header for stm32f1xx_it.c
  - RTC/RTC_LSI/Src/system_stm32f1xx.c       STM32F1xx system clock configuration file
  - RTC/RTC_LSI/Src/stm32f1xx_it.c           Interrupt handlers
  - RTC/RTC_LSI/Src/main.c                   Main program
  - RTC/RTC_LSI/Src/stm32f1xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F107xC devices.
    
  - This example has been tested with STM3210C-EVAL RevC  board and can be
    easily tailored to any other supported device and development board.

  - STM3210C-EVAL RevC Set-up
    - Use LED1 connected to PD.07 pin (pin 15 in CN8 connector).
    - Use the Key push-button connected to pin PB.09 (EXTI_Line9-5)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
