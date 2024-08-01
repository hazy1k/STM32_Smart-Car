#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "UltrasonicWave.h"
#include "Key.h"
#include "Serial.h"
#include "timer.h"
#include "Buzzer.h"
#include "Servo.h"

// ������תͷ����
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
	Timerx_Init(5000,7199);  //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ������2��
	Key_Init();
	Buzzer_Init();
	UltrasonicWave_Init();      //�Գ�����ģ���ʼ��
	Serial_Init();             // ���ڳ�ʼ��
	robot_Init();              // �����˳�ʼ�
	Servo_Init();              // �����ʼ�� 
	while(Key_GetNum() == 0);  //�ȴ���������
	while (1)
	{
			Q_temp = front_detection();
			printf("�⵽�ľ���ֵΪ��%d\r\n",Q_temp);
			if(Q_temp<60 && Q_temp>0) //��������ֵ	
			{
				makerobo_brake(500);		
				makerobo_back(70,500);	
				makerobo_brake(1000);	
				
				L_temp=left_detection();//��������ϰ���ľ���ֵ
				printf("�⵽�ľ���ֵΪ��%d\r\n",L_temp);
				Delay_ms(500);
				R_temp=right_detection();//�����ұ��ϰ���ľ���ֵ
				printf("�⵽�ľ���ֵΪ��%d\r\n",R_temp);
				Delay_ms(500);
				
				if((L_temp < 60 ) &&( R_temp < 60 ))//��������������ϰ��￿�ıȽϽ�
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
