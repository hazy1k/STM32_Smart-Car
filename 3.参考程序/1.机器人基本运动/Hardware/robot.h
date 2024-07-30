#ifndef __ROBOT_H
#define __ROBOT_H

void robot_Init(void);
void robot_speed(uint8_t left1_speed,uint8_t left2_speed,uint8_t right1_speed,uint8_t right2_speed);
// 基本运动
void makerobo_brake(uint16_t time);             // 机器人停止
void makerobo_Left(int8_t speed,uint16_t time); // 左转函数
void makerobo_Spin_Left(int8_t speed,uint16_t time);  // 左旋转函数
void makerobo_Right(int8_t speed,uint16_t time);      //右转函数
void makerobo_Spin_Right(int8_t speed,uint16_t time); //右旋转函数
void makerobo_back(int8_t speed,uint16_t time);      //后退函数

void makerobo_run(uint16_t speed, uint32_t time);
void makerobo_run_infinite(uint16_t speed);

#define MAX_SPEED 10000  // 确保定义了最大速度

#endif


