#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include "Irtracking.h"
#include "Irobstacle.h"
#include "Key.h"

void Robot_TractionAndAvoidance()                     //������ѭ������
{
	if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // ���ഫ����ͬʱ��⵽�����ź�)
	{
		makerobo_brake(0); // ֹͣ
	}
	else
	{
		if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 0)
		{
			makerobo_run(70,0);   // ǰ��
		}
		else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 0) //���ѹ��
		{
			makerobo_Left(70,0);    //�������
		}
		else if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 1) // �Ҳ�ѹ��
		{
			makerobo_Right(70,0); // �Ҳ�����
		}			
		else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 1) // ȫ��ѹ�ߣ�ֹͣ
		{
		 makerobo_brake(0); // ֹͣ
		}
	}
}

int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ������2��
	Key_Init();
	Irtracking_Init();         // ����ѭ����ʼ��
	Irobstacle_Init();         // ������ϳ�ʼ��
	LEDSEG_Init();
	robot_Init();              // �����˳�ʼ��
	while(Key_GetNum() == 0);  // �ȴ���������
	while (1)
	{
	Robot_TractionAndAvoidance();
	}
}
