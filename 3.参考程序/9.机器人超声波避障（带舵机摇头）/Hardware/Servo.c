#include "stm32f10x.h"                  // Device header
#include "ServoPWM.h"

void Servo_Init(void)
{
	TIM3_PWM_Init(19999,71); //72 division. PWM frequency=72000000/72/20000=50hz
}

void Servo_SetAngle(float Angle)
{
	TIM_SetCompare2(TIM3,Angle / 180 * 2000 +500);
}
