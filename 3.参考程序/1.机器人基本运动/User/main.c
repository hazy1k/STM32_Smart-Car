#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "robot.h"
#include "Key.h"

// 假设 MAX_SPEED 是在 robot.h 中定义的宏
#ifndef MAX_SPEED
#define MAX_SPEED 10000  // 替换为实际的最大速度值
#endif

uint8_t i;

int main(void)
{
    robot_Init();   // 初始化机器人
    makerobo_run(MAX_SPEED / 20, 10000);  // 前进两秒，速度为最大速度的一半
    while(1);
}

