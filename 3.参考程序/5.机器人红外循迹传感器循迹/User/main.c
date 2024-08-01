#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "Irtracking.h"
#include "Key.h"

uint8_t i;

int main(void)
{
	robot_Init();        // �����˳�ʼ��
	Key_Init();          // ������ʼ��
	Irtracking_Init();   // ������ϴ�������ʼ��
	while(Key_GetNum() == 0);  //�ȴ���������
	while (1)
	{
	
			if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 0)
			{
				makerobo_run(70,0);   // ǰ��
			}
			else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 0) //���ѹ��
			{
				makerobo_Left(70,0);    //��ת����
			}
			else if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 1) // �Ҳ�ѹ��
			{
				makerobo_Right(70,0); // ��ת����
			}			
			else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 1) // ȫ��ѹ�ߣ�ֹͣ
			{
       makerobo_brake(0);
			}
	}
}
