#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "Irobstacle.h"
#include "Key.h"

uint8_t i;

int main(void)
{
	robot_Init();        // �����˳�ʼ��
	Key_Init();          // ������ʼ��
	Irobstacle_Init();   // ������ϴ�������ʼ��
	while(Key_GetNum() == 0);  //�ȴ���������
	while (1)
	{
	
			if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 1)
			{
				makerobo_run(70,0);   // ǰ��
			}
			else if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 0) //�ұ߼�⵽�����ź�
			{
				makerobo_Left(70,400); 
			}
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 1) // ��߼�⵽�����ź�
			{
				makerobo_Right(70,400);
			}			
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // ���ߴ�����ͬʱ��⵽����
			{
				makerobo_brake(400);          // ���õ�����˺���
				makerobo_Spin_Right(70,900);  // ���õ����ת����
			}
	}
}
