#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include "Incontrol.h"
#include "Key.h"
#include "Serial.h"

int main(void)
{
	uint8_t buf[2];
	uint8_t data_code=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ������2��
	IRremote_Init();           // ����ң������ʼ��
	LEDSEG_Init();
	Serial_Init();             // ���ڳ�ʼ��
	robot_Init();              // �����˳�ʼ��
	//Digital_Display(0);
	while (1)
	{
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
    if(buf[0] == 6 && buf[1] == 2)
		{
			makerobo_run(70,0);  // ǰ��2s
			Digital_Display(0);
		}
		else if(buf[0] == 10 && buf[1] == 8)
		{
			makerobo_back(70,0); // ����2s
			Digital_Display(1);
		}
		else if(buf[0] == 2 && buf[1] == 2)
		{
			makerobo_Spin_Left(70,0); //��ת
			Digital_Display(2);
		}
		else if(buf[0] == 12 && buf[1] == 2)
		{
			makerobo_Spin_Right(70,0); // ��ת
			Digital_Display(3);
		}
		else if(buf[0] == 0 && buf[1] == 2)
		{
			makerobo_brake(0); // ֹͣ
			Digital_Display(4);
		}
		else
		{
			makerobo_brake(0); // ֹͣ
		}
	}
}
