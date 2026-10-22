#include "encoder_test.h"
#include "app_usart2.h"
#include "delay.h"
#include "app_encoder.h"

//
// @简介：对编码器的位置信号进行测试
//        他会通过调试用的串口把编码器的当前位置显示在Vofa上
//
void Encoder_Test(void) // main
{
	App_USART2_Init();
	App_Encoder_Init();
	
	while(1)
	{
		int64_t pos_l = App_Encoder_GetPos_L();
		int64_t pos_r = App_Encoder_GetPos_R();
		
		My_USART_Printf(USART2, "%d,%d\n", (int32_t)pos_l, (int32_t)pos_r);
		
		Delay(50);
	}
}
