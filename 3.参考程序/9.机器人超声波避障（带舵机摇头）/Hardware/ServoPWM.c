#include "stm32f10x.h"                  // Device header

void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //Enable timer 3 clock
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); //Enable GPIO peripheral and AFIO multiplexing function module clock
	
	 GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3 partial remap TIM3_CH2->PB5    
 
   //Set this pin as a multiplexed output function and output the PWM pulse waveform of TIM3 CH2 GPIOB.5
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //Multiplex push-pull output
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);//Initialize GPIO
 
   //Initialize TIM3
	 TIM_TimeBaseStructure.TIM_Period = arr; //Set the value of the automatic reload register period that is loaded in the next update event
	 TIM_TimeBaseStructure.TIM_Prescaler =psc; //Set the prescaler value used as the divisor of the TIMx clock frequency 
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Set clock division: TDTS = Tck_tim
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM up counting mode
	 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //Initialize the time base unit of TIMx according to the parameters specified in TIM_TimeBaseInitStruct
	
	 //Initialize TIM3 Channel2 PWM mode	 
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Select timer mode: TIM pulse width modulation mode 2
 	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Comparison output enable
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Output polarity: TIM output comparison polarity is low
	  TIM_OC2Init(TIM3, &TIM_OCInitStructure); //Initialize the peripheral TIM3 OC2 according to the parameters specified by T
 
	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //Enable the preload register of TIM3 on CCR2
 
	  TIM_Cmd(TIM3, ENABLE); //Enable TIM3
	
 
}



