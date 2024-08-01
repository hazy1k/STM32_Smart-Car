#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include <string.h>
#include <stdio.h>
#include "Key.h"
#include "Serial.h"
#include "Uart3.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	Key_Init();
	LEDSEG_Init();
	Serial_Init();             // 串口初始化
	USART3_init(9600);
	robot_Init();              // 机器人初始化
	while (1)
	{
		if(MyUsart3.flag)	//接收到一次数据了
		{
			MyUsart3.flag=0; //清空标志位
			
				if(strcmp((const char*)MyUsart3.buff,"ONA")==0)
				{
					printf("go forward!\r\n"); 
          makerobo_run(80,100);
				}
				else if(strcmp((const char*)MyUsart3.buff,"ONB")==0)
				{
					printf("go back!\r\n"); 
          makerobo_back(80,100);
				}
				else if(strcmp((const char*)MyUsart3.buff,"ONC")==0)
				{
         printf("go left!\r\n"); 
         makerobo_Left(80,100);
				}
			  else if(strcmp((const char*)MyUsart3.buff,"OND")==0)
				{
         printf("go right!\r\n"); 
         makerobo_Right(80,100);
				}
			  else if(strcmp((const char*)MyUsart3.buff,"ONF")==0)
				{
         printf("Stop!\r\n"); 
         makerobo_brake(100);
				}
				else if(strcmp((const char*)MyUsart3.buff,"ONE")==0)
				{
         printf("Stop!\r\n"); 
         makerobo_brake(100);
				}
			}
			
		}
}

