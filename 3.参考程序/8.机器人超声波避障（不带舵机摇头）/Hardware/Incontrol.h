#ifndef __INCONTROL_H
#define __INCONTROL_H

void IRremote_Init(void);      // ����ң������ʼ��
uint8_t IRremote_Counttime();  // ����ң��������ʱ��

//����ȫ�ֱ���
extern uint32_t IR_Receivecode;
extern uint8_t  IR_Receiveflag;

#endif

