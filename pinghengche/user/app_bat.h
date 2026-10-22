#ifndef  APP_BAT_H
#define  APP_BAT_H
#include "stm32f10x.h"

void  App_Bat_Init(void);//对电压监测模块初始化
float  App_Bat_Get(void);//获取VBAT的值
void   App_Bat_Proc(void);//电源指示灯的切片

#endif