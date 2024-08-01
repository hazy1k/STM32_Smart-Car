#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "UltrasonicWave.h"
#include "Key.h"
#include "Serial.h"
#include "timer.h"
#include "Buzzer.h"
#include "Servo.h"

// ³¬Éù²¨×ªÍ·º¯Êı
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
	Timerx_Init(5000,7199);  //10KhzµÄ¼ÆÊıÆµÂÊ£¬¼ÆÊıµ½5000Îª500ms 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //ÖĞ¶ÏÓÅÏÈ¼¶·Ö×é·Ö2×é
	Key_Init();
	Buzzer_Init();
	UltrasonicWave_Init();      //¶Ô³¬Éù²¨Ä£¿é³õÊ¼»¯
	Serial_Init();             // ´®¿Ú³õÊ¼»¯
	robot_Init();              // »úÆ÷ÈË³õÊ¼»
	Servo_Init();              // ¶æ»ú³õÊ¼»¯ 
	while(Key_GetNum() == 0);  //µÈ´ı°´¼ü°´ÏÂ
	while (1)
	{
			Q_temp = front_detection();
			printf("²âµ½µÄ¾àÀëÖµÎª£º%d\r\n",Q_temp);
			if(Q_temp<60 && Q_temp>0) //²âÁ¿¾àÀëÖµ	
			{
				makerobo_brake(500);		
				makerobo_back(70,500);	
				makerobo_brake(1000);	
				
				L_temp=left_detection();//²âÁ¿×ó±ßÕÏ°­ÎïµÄ¾àÀëÖµ
				printf("²âµ½µÄ¾àÀëÖµÎª£º%d\r\n",L_temp);
				Delay_ms(500);
				R_temp=right_detection();//²âÁ¿ÓÒ±ßÕÏ°­ÎïµÄ¾àÀëÖµ
				printf("²âµ½µÄ¾àÀëÖµÎª£º%d\r\n",R_temp);
				Delay_ms(500);
				
				if((L_temp < 60 ) &&( R_temp < 60 ))//µ±×óÓÒÁ½²à¾ùÓĞÕÏ°­Îï¿¿µÄ±È½Ï½ü
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
