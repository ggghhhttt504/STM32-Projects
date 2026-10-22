#ifndef APP_PWM_H
#define  APP_PWM_H


#include "stm32f10x.h"
void  App_PWM_Init(void);

void App_PWM_Cmd(uint8_t on);//外部控制STYB引脚状态，on 等于0时TB2休眠，1活动  按钮控制
void  App_PWM_Set_L(float Duty);//范围-100.0-100.0  
void  App_PWM_Set_R(float Duty);//设置右电机的占空比
#endif