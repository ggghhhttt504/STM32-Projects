#include "pwm_test.h"
#include "app_pwm.h"
#include "delay.h"

//
//测试PWM模块
//先使能电机
//让电机正转30% 2秒
//          60% 2秒
//          90%  2秒 
//关闭电机
void PWM_Test(void){
App_PWM_Init ();
	

	App_PWM_Cmd (1);//唤醒电机
	App_PWM_Set_L(30);
		App_PWM_Set_R(30);
	Delay(2000);//2S
	
	App_PWM_Set_L(60);
		App_PWM_Set_R(60);
	Delay(2000);//2S
	
	App_PWM_Set_L(60);
		App_PWM_Set_R(60);
	Delay(2000);//2S
	
	App_PWM_Cmd (0);//休眠电机

while(1){


}
}