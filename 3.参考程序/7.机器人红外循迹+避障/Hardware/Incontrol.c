#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//�˿ڶ���
#define IRED_PORT 			GPIOA  
#define IRED_PIN 			  GPIO_Pin_8
#define IRED_PORT_RCC		RCC_APB2Periph_GPIOA

uint32_t IR_Receivecode;	  //����һ��32λ���ݱ��������������
uint8_t  IR_Receiveflag;   //����һ��8λ���ݵı���������ָʾ���ձ�־

/*******************************************************************************
* �� �� ��         : IRremote_Init
* ��������		     : ����˿ڳ�ʼ������	  ʱ�Ӷ˿ڼ��ⲿ�жϳ�ʼ�� 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IRremote_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(IRED_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=IRED_PIN;                        //�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(IRED_PORT,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(EXTI_Line8);
	
	/* �����ⲿ�жϵ�ģʽ */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* ����NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
	NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* �� �� ��         : IRremote_Counttime
* ��������		     : �ߵ�ƽ����ʱ�䣬����¼��ʱ�䱣����t�з��أ�����һ�δ�Լ20us 
* ��    ��         : ��
* ��    ��         : t
*******************************************************************************/
uint8_t IRremote_Counttime()
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(IRED_PORT,IRED_PIN)==1)//�ߵ�ƽ
	{
		t++;
		Delay_us(20);
		if(t>=250) return t;//��ʱ���
	}
	return t;
}

void EXTI9_5_IRQHandler(void)	  //����ң���ⲿ�ж�
{
//	uint8_t Tim=0,Ok=0,Data,Num=0;

//	while(1)
//	{
//	  if(GPIO_ReadInputDataBit(IRED_PORT,IRED_PIN)==1)
//		{
//			 Tim=IRremote_Counttime();//��ô˴θߵ�ƽʱ��

//			 if(Tim>=250) break;//�������õ��ź�

//			 if(Tim>=200 && Tim<250)
//			 {
//			 	Ok=1;//�յ���ʼ�ź�
//			 }
//			 else if(Tim>=60 && Tim<90)
//			 {
//			 	Data=1;//�յ����� 1
//			 }
//			 else if(Tim>=10 && Tim<50)
//			 {
//			 	Data=0;//�յ����� 0
//			 }

//			 if(Ok==1)
//			 {
//			 	IR_Receivecode<<=1;
//				IR_Receivecode+=Data;

//				if(Num>=32)
//				{
//					IR_Receiveflag=1;
//				  break;
//				}
//			 }
//			 Num++;
//		}
//	}
//	EXTI_ClearITPendingBit(EXTI_Line8);	
}
