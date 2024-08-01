#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include "Incontrol.h"
#include "Key.h"
#include "Serial.h"

int main(void)
{
	uint8_t buf[2];
	uint8_t data_code=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	IRremote_Init();           // 红外遥控器初始化
	LEDSEG_Init();
	Serial_Init();             // 串口初始化
	robot_Init();              // 机器人初始化
	//Digital_Display(0);
	while (1)
	{
	  if(IR_Receiveflag == 1) //如果红外接收到
		{
			IR_Receiveflag = 0; //清零
			printf("红外接收码 %0.8X\r\n",IR_Receivecode);	//打印
			data_code=IR_Receivecode>>8;
			IR_Receivecode = 0; //接收码清零
			
			buf[0] = data_code/16;
			buf[1] = data_code%16;
			
		  printf("buf[0]:%d\r\n",buf[0]);
		  printf("buf[1]:%d\r\n",buf[1]);
		}
    if(buf[0] == 6 && buf[1] == 2)
		{
			makerobo_run(70,0);  // 前进2s
			Digital_Display(0);
		}
		else if(buf[0] == 10 && buf[1] == 8)
		{
			makerobo_back(70,0); // 后退2s
			Digital_Display(1);
		}
		else if(buf[0] == 2 && buf[1] == 2)
		{
			makerobo_Spin_Left(70,0); //左转
			Digital_Display(2);
		}
		else if(buf[0] == 12 && buf[1] == 2)
		{
			makerobo_Spin_Right(70,0); // 右转
			Digital_Display(3);
		}
		else if(buf[0] == 0 && buf[1] == 2)
		{
			makerobo_brake(0); // 停止
			Digital_Display(4);
		}
		else
		{
			makerobo_brake(0); // 停止
		}
	}
}
