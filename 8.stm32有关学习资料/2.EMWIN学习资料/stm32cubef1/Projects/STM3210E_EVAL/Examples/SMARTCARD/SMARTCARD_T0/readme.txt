/**
  @page SMARTCARD_T0 Smart Card T0 protocol Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    SMARTCARD_T0/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   Description of the Smart Card example.
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
This example describes a firmware Smartcard Interface based on the STM32F1xx USART
peripheral. The main purpose of this firmware example is to provide resources 
facilitating the development of an application using the USART peripheral in 
smartcard mode.

The firmware interface is composed of library source files developed in order to
support ISO7816-3/4 specification, an application example is also provided.

This example is based on the AN2598 "Smartcard interface with the STM32F101xx 
and STM32F103xx" application note. For more details, please refer to AN2598 
available on www.st.com

ST sample Smartcard, provided with STM3210E-EVAL RevD board, contains a GSM SIM card
File System sample. This project implements following sequences taking place between 
STM32 MCU and Smartcard :
- At power on, Board is waiting for card insertion (LED1 Blinking)
- As soon as card insertion is detected, card is power on, and a list of APDU Command/Response pairs
  are exchanged between application and Smartcard, including PTS procedure (speed negotiation),
  File Selection (MF, DF or EF), Information reading, File content reading (IMSI and ICCID)
- During this steps, all LEDS are turned on.
- When this sequence is completed, Smartcard is powered off, and waiting for user to remove card 
  from smartcard slot (LED1 blinking)
- Example is running in loop, so that at this stage, user is prompted again for card insertion ...

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SMARTCARD_T0/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - SMARTCARD_T0/Inc/stm32f1xx_it.h          DMA interrupt handlers header file
  - SMARTCARD_T0/Inc/main.h                  Header for main.c module
  - SMARTCARD_T0/Inc/smartcard.h             Header for smartcard.c module
  - SMARTCARD_T0/Src/stm32f1xx_it.c          DMA interrupt handlers
  - SMARTCARD_T0/Src/main.c                  Main program
  - SMARTCARD_T0/Inc/smartcard.c             smartcard module
  - SMARTCARD_T0/Src/stm32f1xx_hal_msp.c     HAL MSP module
  - SMARTCARD_T0/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment  
  - This example runs on STM32F1xx devices.    
  - This example has been tested with STMicroelectronics STM3210E-EVAL RevD 
    evaluation board (embedding a STM32F103xG device) and 
    can be easily tailored to any other supported device and development board. 
      
  - STM3210E-EVAL RevD Set-up
      In order to get Smartcard corresponding I/O signals connected to smartcard interface chip,
      please ensure that JP15 and JP16 are closed..
      Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN12 (USART1) and PC serial port if you want to display data on the HyperTerminal.
  - USART1 connected to PC HyperTerminal could be used to follow Smartcard operations progress
    UART to be configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - No Hardware flow control


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - Test output (description of running actions) is available on USART1. Please connect USART1 connector 
   to Hypterminal to follow test progress.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
