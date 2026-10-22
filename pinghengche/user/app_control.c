#include "app_control.h"
#include "app_pid.h"
#include "task.h"
#include  "app_mpu6050.h"
#include "qmath.h"
#include "delay.h"
#include "app_motor.h"
#include "app_encoder.h"

static  PID_TypeDef  pid_velocity;//速度环的PID控制器
static PID_TypeDef  pid_theta;//c他环，控制角度的
static  const  float g=9.8;//重力加速度  单位m/s^2
static  PID_TypeDef  pid_theta_dot;//C他点，就是角速度
static  const float lp=0.062; //平衡车轮胎中心到电池中心的距离
static   const  float rw=0.032;//轮胎的半径 单位m
static  PID_TypeDef  pid_turn ;//转向环   
static  float omega_ref=0.0f;
static  uint64_t lastTime=0.0f;//上一次的时间
//
//负责初始化平衡车控制系统
//
void App_Control_Init(void){
	
	
	PID_Init (&pid_velocity ,10.0f,1.0f,0.0f);//初始化速度环的PID
	
	PID_LimitConfig (&pid_velocity ,0.5*g,-0.5*g);//将速度环的PID控制器的输出限制在+-0.5g之间
   PID_Init (&pid_theta ,4.0f,0.0f,0.0f);
	PID_LimitConfig (&pid_theta ,+12.57f,-12.57f);//将c他环pid控制器输出限制到+-4pi之间 角速度
	   PID_Init (&pid_theta_dot ,10.0f,10.0f,0.0f);
	PID_LimitConfig (&pid_theta_dot ,125.7f,-125.7f);//将c他环求导pid控制器输出限制到+-40pi之间  角加速度
	
	PID_Init(&pid_turn,1.0f,0.0f,0.0f);
	PID_LimitConfig(&pid_turn,+15.0f,-15.0f);
}//
//平衡车控制系统的进程函数，需要在while循环中调用
//
void APP_Control_Proc(void){

PERIODIC (5)//控制程序5ms执行一次
	  uint64_t  now=GetUs();
	   float deltaT =( now-lastTime )*1.0e-6f;
	
	//第-1步 改变速度环的设定值
	//PID_ChangeSP (&pid_velocity ,0.0f);
	
	//-2  读取传感器的值
	
	float  omega=0.5*(App_Encoder_GetSpeed_L ()+App_Encoder_GetSpeed_R ());//单位弧度/s
	float theta=App_MPU6050_GetPitch ()*0.0174533;//原本是度，现在要用弧度，就是/180*2Pi
	float theta_dot=App_MPU6050_GetGx ()*0.0174533;  //绕轴旋转的角速度
	//-3计算速度环的反馈值，算出来x_dot
	
	float omega2=theta_dot*(lp+rw)/rw;
	
	float  omega1=omega-omega2;
	
	float x_dot=omega1*rw;
	
	//-4 执行PID运算，速度环的 ，就是最外面的
	float theta_ref=	qatan(PID_Compute(&pid_velocity,x_dot)/g);  //括号里的是x_dot_dot:PID_Compute(&pid_velocity,x_dot)/
	
	//1.将外环的设定值SP等于0 就是角度为0，让车不到
	PID_ChangeSP (&pid_theta ,theta_ref);
	
	
	
	//3.计算外环PID的输出
	
	float  theta_dot_ref=PID_Compute (&pid_theta ,theta );
	
	//4.改变内环的设定值SP
	PID_ChangeSP (&pid_theta_dot ,theta_dot_ref );
	
	//5.计算内环pid的输出
float theta_dot_dot_ref=	PID_Compute (&pid_theta_dot ,theta_dot );
	
	
	//6.倒立摆的逆结算   直线速度x dao dao
	
	float x_dot_dot_ref=(g*qsin(theta)-theta_dot_dot_ref*lp)/qcos(theta );
	
	//7.计算轮胎转速
	
	if(lastTime!=0){
	
   omega_ref +=1.0f/rw*x_dot_dot_ref *deltaT ;}
	//等于0时代表没有运行过
	 
	 
	 //转向环
	 float gz=App_MPU6050_GetGz()*0.017453f;//度/s-->弧度每秒
	 
	 float omega_diff=PID_Compute(&pid_turn,gz);
	 
	 
   //8设定轮胎的转速
	 App_Motor_SetOmega_L (omega_ref +omega_diff);
	 App_Motor_SetOmega_R (omega_ref -omega_diff);
	lastTime =now;
}
//
//对控制系统复位
//
void  App_Control_Reset(void){
//1.暂存的值，
lastTime=0;
	omega_ref=0;
	//2.PID 控制器复位
	
	PID_Reset(&pid_velocity);
	
		PID_Reset(&pid_theta);
		PID_Reset(&pid_theta_dot);
	
}
//
// 改变平横车移动的速度
// 参数MoveSpeed 速度 单位m/s  最大速度0.7m/s
void App_Control_SetMoveSpeed(float MoveSpeed){
   PID_ChangeSP(&pid_velocity,MoveSpeed);//速度环
	
	
}
//
//改变平衡车转向的速度
//参数  转向速度，单位弧度每秒。最大转速15弧度每秒
void App_Control_SetTurnSpeed(float TurnSpeed){


 PID_ChangeSP(&pid_turn,TurnSpeed);
}

