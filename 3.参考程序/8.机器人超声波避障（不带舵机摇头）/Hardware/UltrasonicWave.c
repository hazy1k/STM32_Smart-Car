#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "timer.h"


#define	TRIG_PORT      GPIOB		//TRIG       
#define	ECHO_PORT      GPIOB		//ECHO 
#define	TRIG_PIN       GPIO_Pin_15   //TRIG       
#define	ECHO_PIN       GPIO_Pin_14	//ECHO   

float UltrasonicWave_Distance;      //������ľ���    

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					  //PB15��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	      //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				   //PB14��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);				 //��ʼ��GPIOB
	
	//GPIOB.14	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

 	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		                         //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			     //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	     //��ռ���ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			       //�����ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						     //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);  	                             //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//------------------------ͨ��14�жϺ���---------------------------------------------
void EXTI15_10_IRQHandler(void)
{
	Delay_us(10);		                      //��ʱ10us
  if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE);                                       //����ʱ��
		
			while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));          //�ȴ��͵�ƽ

			TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*5*34/200.0;			//�������&&UltrasonicWave_Distance<150
			EXTI_ClearITPendingBit(EXTI_Line14);  //���EXTI1��·����λ
	}
}
/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
int UltrasonicWave_StartMeasure(void)
{
  int u_temp;
	GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?
  Delay_us(20);		                      //��ʱ20US
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
	u_temp = UltrasonicWave_Distance*10;
	return u_temp;
}

