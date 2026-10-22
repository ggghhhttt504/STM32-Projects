#ifndef APP_ENCODER_H
#define APP_ENCODER_H

#include "stm32f10x.h"

void App_Encoder_Init(void);


float   App_Encoder_GetPos_L(void);//测试读取左编码器位置
float   App_Encoder_GetPos_R(void);//测试读取右编码器位置

float   App_Encoder_GetSpeed_L(void);//左电机旋转的角速度

float   App_Encoder_GetSpeed_R(void);//右电机旋转的角速度

#endif
