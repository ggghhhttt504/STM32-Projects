#include "app_motor.h"
#include "app_pid.h"
#include "task.h"
#include "app_encoder.h"
#include "app_pwm.h  "
#include "app_bat.h"
static  PID_TypeDef   pid_motor_l;//左电机调速系统的PID控制器
static  PID_TypeDef   pid_motor_r;//左电机调速系统的PID控制器
//
//对电机调速系统初始化
//
void App_Motor_Init(void){
PID_Init (&pid_motor_l ,0.5,7,0);
	  		PID_LimitConfig(&pid_motor_l ,+8.4f,-8.4f);
	PID_Init (&pid_motor_r ,0.5,7,0);
		PID_LimitConfig(&pid_motor_r ,+8.4f,-8.4f);
	


}
//
//电机调速系统的进程函数
//
void App_Motor_Proc(void){

PERIODIC (1)//每间隔1MS执行一次
	
	//获取左右电机的角速度
	float omegar_l= App_Encoder_GetSpeed_L ();//就是FB
	float omegar_r=App_Encoder_GetSpeed_R ();
	//2.计算PID控制器的输出
float ua_l=	PID_Compute (&pid_motor_l,omegar_l  );
	 float ua_r=	PID_Compute (&pid_motor_r,omegar_r  );
	//3.将电压UA设置到电机两端
	//获取电池电压vbat
	float  vbat=App_Bat_Get ();
	float duty_l=ua_l /vbat *100.0f;
		float duty_r=ua_r /vbat *100.0f;
	//设置左右电机的占空比
	App_PWM_Set_L (duty_l );
		App_PWM_Set_R (duty_r );

 }
//设置左电机的转速 Omega的值，转速，单位弧度每秒
 void App_Motor_SetOmega_L(float omega){
 
PID_ChangeSP (&pid_motor_l ,omega );
 
 }

void App_Motor_SetOmega_R(float omega){
PID_ChangeSP (&pid_motor_r ,omega );

}
//
//电机开关
//On控制电机的开关，0关闭，非零开启
void App_Motor_Cmd(uint8_t on){
App_PWM_Cmd (on);
	//电机开关的同时需要对PID控制器进行复位
	PID_Reset(&pid_motor_l );
		PID_Reset(&pid_motor_r );
}