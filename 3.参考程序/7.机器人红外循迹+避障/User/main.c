#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include "Irtracking.h"
#include "Irobstacle.h"
#include "Key.h"

void Robot_TractionAndAvoidance()                     //机器人循迹避障
{
	if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // 两侧传感器同时检测到红外信号)
	{
		makerobo_brake(0); // 停止
	}
	else
	{
		if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 0)
		{
			makerobo_run(70,0);   // 前进
		}
		else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 0) //左侧压线
		{
			makerobo_Left(70,0);    //左侧修正
		}
		else if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 1) // 右侧压线
		{
			makerobo_Right(70,0); // 右侧修正
		}			
		else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 1) // 全部压线，停止
		{
		 makerobo_brake(0); // 停止
		}
	}
}

int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	Key_Init();
	Irtracking_Init();         // 红外循迹初始化
	Irobstacle_Init();         // 红外避障初始化
	LEDSEG_Init();
	robot_Init();              // 机器人初始化
	while(Key_GetNum() == 0);  // 等待按键按下
	while (1)
	{
	Robot_TractionAndAvoidance();
	}
}
