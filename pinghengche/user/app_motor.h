#ifndef APP_MOTOR_H
#define APP_MOTOR_H

#include "stm32f10x.h"
void App_Motor_Init(void);//对电机调速系统初始化
void App_Motor_Proc(void);
void App_Motor_SetOmega_L(float omega);//设置左电机的转速

void App_Motor_SetOmega_R(float omega);//设置右电机的转速
void App_Motor_Cmd(uint8_t on); //控制电机的开关

#endif