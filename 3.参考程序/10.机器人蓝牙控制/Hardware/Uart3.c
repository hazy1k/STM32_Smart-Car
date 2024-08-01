#include "stm32f10x.h"                  // Device header
#include "Uart3.h"

TypeUsart3 MyUsart3;
void USART3_init(uint32_t baud)   //串口3初始化函数
{
   NVIC_InitTypeDef NVIC_InitStruct={0};   //NVIC结构体初始化
   GPIO_InitTypeDef GPIO_InitStruct={0};    //GPIO结构体初始化
   USART_InitTypeDef USART_InitStruct={0};  //USART结构体初始化
   
   //1:初始化时钟
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //串口时钟使能
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //使能GPIOA时钟
   
   
   //3:初始化引脚
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;         //PB10
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStruct);           //初始化PB10
   
    //USART3_RX	  PB11
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStruct);               //初始化PB11
   
   //初始化中断
   NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;    //选择串口中断
   NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;      //中断使能
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
   NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
   
   NVIC_Init(&NVIC_InitStruct);
   
  
   USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);   //使能串口外设接收中断
   USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);   //使能串口外设空闲中断

   
   //4:初始化串口
   USART_InitStruct.USART_BaudRate=baud;                    //波特率
   USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //八个数据位
   USART_InitStruct.USART_StopBits=USART_StopBits_1;        //一个停止位
   USART_InitStruct.USART_Parity=USART_Parity_No;           //无奇偶校验
   USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx; //收发模式
   USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   
   USART_Init(USART3,&USART_InitStruct);
   
   //5:使能串口
   USART_Cmd(USART3,ENABLE);
}

void USART3_IRQHandler(void)  //中断服务函数
{
   if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)   //判断接收中断
   {
	   USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //清除接收标志位
      MyUsart3.buff[MyUsart3.len++]=USART_ReceiveData(USART3);      //接收数据
   }
   if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET)   //判断空闲中断
   { 
      MyUsart3.buff[MyUsart3.len]='\0';
      MyUsart3.flag=1;
      MyUsart3.len=0;
      USART_ReceiveData(USART3);
   }
}


void usatr3_send_data(uint8_t c)        //发送一个字节的函数
{
   while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));    //获取发送完成标志位
   USART_SendData(USART3,c);      //发送数据
}

//发送数据
void uatr3_send_data(char * data,uint8_t len)   //串口3发送一个字符串的函数
{
    while(len--)
    {
        usatr3_send_data(*data);
        data++;
    }
}


