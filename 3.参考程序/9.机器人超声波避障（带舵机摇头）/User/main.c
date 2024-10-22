#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "UltrasonicWave.h"
#include "Key.h"
#include "Serial.h"
#include "timer.h"
#include "Buzzer.h"
#include "Servo.h"

// 超声波转头函数
int front_detection()
{
	int distance;
	Servo_SetAngle(90);
	Delay_ms(100);
	distance = UltrasonicWave_StartMeasure();
	return distance;
}

int left_detection()
{
	int distance;
	Servo_SetAngle(175);
	Delay_ms(300);
	distance = UltrasonicWave_StartMeasure();
	return distance;
}

int right_detection()
{
	int distance;
	Servo_SetAngle(5);
	Delay_ms(300);
	distance = UltrasonicWave_StartMeasure();
	return distance;
}

int main(void)
{
  int Q_temp,L_temp,R_temp;
	Timerx_Init(5000,7199);  //10Khz的计数频率，计数到5000为500ms 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	Key_Init();
	Buzzer_Init();
	UltrasonicWave_Init();      //对超声波模块初始化
	Serial_Init();             // 串口初始化
	robot_Init();              // 机器人初始�
	Servo_Init();              // 舵机初始化 
	while(Key_GetNum() == 0);  //等待按键按下
	while (1)
	{
			Q_temp = front_detection();
			printf("测到的距离值为：%d\r\n",Q_temp);
			if(Q_temp<60 && Q_temp>0) //测量距离值	
			{
				makerobo_brake(500);		
				makerobo_back(70,500);	
				makerobo_brake(1000);	
				
				L_temp=left_detection();//测量左边障碍物的距离值
				printf("测到的距离值为：%d\r\n",L_temp);
				Delay_ms(500);
				R_temp=right_detection();//测量右边障碍物的距离值
				printf("测到的距离值为：%d\r\n",R_temp);
				Delay_ms(500);
				
				if((L_temp < 60 ) &&( R_temp < 60 ))//当左右两侧均有障碍物靠的比较近
				{
					makerobo_Spin_Left(60,500);
				}				
				else if(L_temp > R_temp)
				{
					makerobo_Left(70,700);
					makerobo_brake(500);
				}	
				else
				{
					makerobo_Right(70,700);
					makerobo_brake(500);					
				}							
			}	
			else
			{
				makerobo_run(70,10);
			}

		}
}
