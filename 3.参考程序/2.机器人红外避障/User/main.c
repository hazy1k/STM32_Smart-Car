#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "Irobstacle.h"
#include "Key.h"

uint8_t i;

int main(void)
{
	robot_Init();        // 机器人初始化
	Key_Init();          // 按键初始化
	Irobstacle_Init();   // 红外避障传感器初始化
	while(Key_GetNum() == 0);  //等待按键按下
	while (1)
	{
	
			if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 1)
			{
				makerobo_run(70,0);   // 前进
			}
			else if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 0) //右边检测到红外信号
			{
				makerobo_Left(70,400); 
			}
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 1) // 左边检测到红外信号
			{
				makerobo_Right(70,400);
			}			
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // 两边传感器同时检测到红外
			{
				makerobo_brake(400);          // 调用电机后退函数
				makerobo_Spin_Right(70,900);  // 调用电机右转函数
			}
	}
}
