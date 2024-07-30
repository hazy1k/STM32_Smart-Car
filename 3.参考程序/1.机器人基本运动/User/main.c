#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "robot.h"
#include "Key.h"

// ���� MAX_SPEED ���� robot.h �ж���ĺ�
#ifndef MAX_SPEED
#define MAX_SPEED 10000  // �滻Ϊʵ�ʵ�����ٶ�ֵ
#endif

uint8_t i;

int main(void)
{
    robot_Init();   // ��ʼ��������
    makerobo_run(MAX_SPEED / 20, 10000);  // ǰ�����룬�ٶ�Ϊ����ٶȵ�һ��
    while(1);
}

