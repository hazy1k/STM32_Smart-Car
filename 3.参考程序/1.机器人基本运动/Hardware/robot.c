#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

#define MAX_SPEED 65535

// 机器人初始化
void robot_Init(void)
{
	PWM_Init(); 
}

//四路PWM控制速度调节
void robot_speed(uint16_t left1_speed, uint16_t left2_speed, uint16_t right1_speed, uint16_t right2_speed)
{	
    TIM_SetCompare1(TIM4, left1_speed);
    TIM_SetCompare2(TIM4, left2_speed);
    TIM_SetCompare3(TIM4, right1_speed);
    TIM_SetCompare4(TIM4, right2_speed);
}

// 基本的运动函数
// 前进函数
void makerobo_run(int32_t speed, uint16_t time)
{
    if (speed > MAX_SPEED) { 
        speed = MAX_SPEED; 
    }
    if (speed < 0) { 
        speed = 0; 
    }
    
    robot_speed(speed, 0, speed, 0); // 设置左、右电机的速度
    Delay_ms(time);                  // 延迟指定时间
    robot_speed(0, 0, 0, 0);         // 停止机器人
}

// 后退函数
void makerobo_back(int32_t speed, uint16_t time)
{
    if (speed > MAX_SPEED) { 
        speed = MAX_SPEED; 
    }
    if (speed < 0) { 
        speed = 0; 
    }
    
    robot_speed(0, speed, 0, speed); // 设置左、右电机的速度
    Delay_ms(time);                  // 延迟指定时间
    robot_speed(0, 0, 0, 0);         // 停止机器人
}

// 左转函数
void makerobo_left(int32_t speed, uint16_t time)
{
    if (speed > MAX_SPEED) { 
        speed = MAX_SPEED; 
    }
    if (speed < 0) { 
        speed = 0; 
    }
    
    robot_speed(0, speed, speed, 0); // 设置左、右电机的速度
    Delay_ms(time);                  // 延迟指定时间
    robot_speed(0, 0, 0, 0);         // 停止机器人
}

// 右转函数
void makerobo_right(int32_t speed, uint16_t time)
{
    if (speed > MAX_SPEED) { 
        speed = MAX_SPEED; 
    }
    if (speed < 0) { 
        speed = 0; 
    }
    
    robot_speed(speed, 0, 0, speed); // 设置左、右电机的速度
    Delay_ms(time);                  // 延迟指定时间
    robot_speed(0, 0, 0, 0);         // 停止机器人
}

