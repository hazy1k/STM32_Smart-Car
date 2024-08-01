#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include "Incontrol.h"
#include "Irtracking.h"
#include "Irobstacle.h"
#include "Key.h"
#include "Serial.h"

unsigned char temp = 1;    // �л������־λ
void Robot_Traction()                     //������ѭ���ӳ���
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

void Robot_Avoidance()     // ��������ӳ���
{
	if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 1)
			{
				makerobo_run(70,0);   // ǰ��
			}
			else if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 0) //�Ҳ��⵽�����ź�
			{
				makerobo_Left(70,400); 
			}
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 1) // ����⵽�����ź�
			{
				makerobo_Right(70,400);
			}			
			else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // ���ഫ����ͬʱ��⵽�����ź�
			{
				makerobo_brake(400);          // ���õ�����˺���
				makerobo_Spin_Right(70,900);  // ���õ����ת����
			}
}

void ControlCar_Ircontrol() // �������ң��������
{
  uint8_t buf[2];
	uint8_t data_code=0; 
	
	if(IR_Receiveflag == 1) //���������յ�
		{
			IR_Receiveflag = 0; //����
			printf("��������� %0.8X\r\n",IR_Receivecode);	//��ӡ
			data_code=IR_Receivecode>>8;
			IR_Receivecode = 0; //����������
			
			buf[0] = data_code/16;
			buf[1] = data_code%16;
			
		  printf("buf[0]:%d\r\n",buf[0]);
		  printf("buf[1]:%d\r\n",buf[1]);
		}
    if(buf[0] == 11 && buf[1] == 1)
		{
			makerobo_run(70,2000);  // ǰ��2s
		}
		else if(buf[0] == 13 && buf[1] == 4)
		{
			makerobo_back(70,2000); // ����2s
		}
		else if(buf[0] == 9 && buf[1] == 1)
		{
			makerobo_Spin_Left(70,2000); //��ת
		}
		else if(buf[0] == 14 && buf[1] == 1)
		{
			makerobo_Spin_Right(70,2000); // ��ת
		}
		else if(buf[0] == 8 && buf[1] == 1)
		{
			makerobo_brake(0); // ֹͣ
		}
		else
		{
			makerobo_brake(0); // ֹͣ
		}
}

void Irscan()
{
	uint8_t Tim=0,Ok=0,Data,Num=0;
	while(1)
	{
	  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==1)
		{
			 Tim=IRremote_Counttime();//��ô˴θߵ�ƽʱ��

			 if(Tim>=250) break;//�������õ��ź�

			 if(Tim>=200 && Tim<250)
			 {
			 	Ok=1;//�յ���ʼ�ź�
			 }
			 else if(Tim>=60 && Tim<90)
			 {
			 	Data=1;//�յ����� 1
			 }
			 else if(Tim>=10 && Tim<50)
			 {
			 	Data=0;//�յ����� 0
			 }

			 if(Ok==1)
			 {
			 	IR_Receivecode<<=1;
				IR_Receivecode+=Data;

				if(Num>=32)
				{
					IR_Receiveflag=1;
				  break;
				}
			 }
			 Num++;
		}
		ControlCar_Ircontrol();
	}
	EXTI_ClearITPendingBit(EXTI_Line8);	
}

int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ������2��
	Key_Init();
	IRremote_Init();           // ����ң������ʼ��
	Irtracking_Init();         // ����ѭ����ʼ��
	Irobstacle_Init();         // ������ϳ�ʼ��
	LEDSEG_Init();
	Serial_Init();             // ���ڳ�ʼ��
	robot_Init();              // �����˳�ʼ��
	while (1)
	{
	  if(Key_GetNum() == 1)
		{
			temp ++;
		}
		if(temp > 3)
		{
			temp = 1;
		}
		switch(temp)
		{
			case 1: Digital_Display(1);Robot_Traction();EXTI_DeInit();break;
			case 2: Digital_Display(2);Robot_Avoidance();break;
			case 3: Digital_Display(3);Irscan();break;
		}
	}
}
