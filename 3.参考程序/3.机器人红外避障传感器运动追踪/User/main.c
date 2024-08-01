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
	    // û�м�⵽�ϰ��ﵲס��������ֹͣ
			if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 1)
			{
				makerobo_brake(0);   //ֹͣ
			}
			else if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 0) //�ұ߼�⵽�����ź�
			{
				makerobo_Right(70,200);
			}
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 1) // ��߼�⵽�����ź�
			{
				makerobo_Left(70,200);
			}			
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // ���ߴ�����ͬʱ��⵽����
			{
				makerobo_run(70,200);          // ����ǰ������
			}
	}
}
