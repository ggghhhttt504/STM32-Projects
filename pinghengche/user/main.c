#include "stm32f10x.h"
#include "bat_test.h"
#include "app_bat.h"
#include "app_button.h"
#include  "app_pwm.h"
#include "pwm_test.h"
#include "encoder_test.h"
#include "mpu6050_test.h"
#include "app_encoder.h"
#include "app_motor.h"
#include "delay.h"
#include "app_usart2.h"
#include "task.h"
#include "app_mpu6050.h"
#include "app_control.h"
#include "app_rc.h"
int main(void)
{
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_0 );//中断优先级
	
	
  //MPU6050_EularAngleTest();
	//MPU6050_TEST();
	//Encoder_T_Method_Test();
  // Encoder_M_Method_Test();
	//Encoder_Test ();
	//Bat_Tset();
	//PWM_Test();//测试转速
		App_USART2_Init();

		App_Button_Init ();//按钮初始化
	
  App_Encoder_Init ();//编码器初始化
	App_PWM_Init ();//电机初始化
	App_Bat_Init ();//初始化
	App_Motor_Init();
	App_MPU6050_Init ();
	App_Control_Init ();
	App_RC_Init ();
	
	while(1)
	{
 
				 App_Bat_Proc();
		App_Button_Proc ();//按钮进程函数
	
		App_Motor_Proc ();

		App_MPU6050_Proc ();
		APP_Control_Proc ();
		
		App_RC_Proc ();
		
 
	}
}

