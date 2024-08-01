#include "stm32f10x.h"                  // Device header
#include "Uart3.h"

TypeUsart3 MyUsart3;
void USART3_init(uint32_t baud)   //����3��ʼ������
{
   NVIC_InitTypeDef NVIC_InitStruct={0};   //NVIC�ṹ���ʼ��
   GPIO_InitTypeDef GPIO_InitStruct={0};    //GPIO�ṹ���ʼ��
   USART_InitTypeDef USART_InitStruct={0};  //USART�ṹ���ʼ��
   
   //1:��ʼ��ʱ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //����ʱ��ʹ��
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //ʹ��GPIOAʱ��
   
   
   //3:��ʼ������
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;         //PB10
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOB, &GPIO_InitStruct);           //��ʼ��PB10
   
    //USART3_RX	  PB11
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOB, &GPIO_InitStruct);               //��ʼ��PB11
   
   //��ʼ���ж�
   NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;    //ѡ�񴮿��ж�
   NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;      //�ж�ʹ��
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
   NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
   
   NVIC_Init(&NVIC_InitStruct);
   
  
   USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);   //ʹ�ܴ�����������ж�
   USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);   //ʹ�ܴ�����������ж�

   
   //4:��ʼ������
   USART_InitStruct.USART_BaudRate=baud;                    //������
   USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //�˸�����λ
   USART_InitStruct.USART_StopBits=USART_StopBits_1;        //һ��ֹͣλ
   USART_InitStruct.USART_Parity=USART_Parity_No;           //����żУ��
   USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx; //�շ�ģʽ
   USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   
   USART_Init(USART3,&USART_InitStruct);
   
   //5:ʹ�ܴ���
   USART_Cmd(USART3,ENABLE);
}

void USART3_IRQHandler(void)  //�жϷ�����
{
   if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)   //�жϽ����ж�
   {
	   USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //������ձ�־λ
      MyUsart3.buff[MyUsart3.len++]=USART_ReceiveData(USART3);      //��������
   }
   if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET)   //�жϿ����ж�
   { 
      MyUsart3.buff[MyUsart3.len]='\0';
      MyUsart3.flag=1;
      MyUsart3.len=0;
      USART_ReceiveData(USART3);
   }
}


void usatr3_send_data(uint8_t c)        //����һ���ֽڵĺ���
{
   while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));    //��ȡ������ɱ�־λ
   USART_SendData(USART3,c);      //��������
}

//��������
void uatr3_send_data(char * data,uint8_t len)   //����3����һ���ַ����ĺ���
{
    while(len--)
    {
        usatr3_send_data(*data);
        data++;
    }
}


