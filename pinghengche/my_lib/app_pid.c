#include "app_pid.h"
#include "delay.h"

//
//对pid控制器初始化
//Kp 比例系数
//kd 微分系数
//Ki 积分系数 
//
void PID_Init(PID_TypeDef  *PID,float Kp,float Ki,float Kd ){

PID->Kp=Kp;
PID->Ki =Ki;
PID->Kd =Kd;
PID->SP =0.0f;
	PID->t_k_1 =0;
	PID->err_k_1=0.0f;
	PID->err_int_k_1  =0.0f;
	PID->UpperLimit =+3.4e+38f;
	//初始化值无限大表示没有限制
	PID->LowerLimit =-3.4e+38f;

}

//
//改变设定值
// Sp 是新的设定值
void  PID_ChangeSP(PID_TypeDef  *PID,float SP){

PID->SP =SP;

}
//
//执行一次PID的运算
//FB代表反馈的值，传感器采回的值
//返回值PID计算的结果

float PID_Compute(PID_TypeDef *PID,float FB){
  float  err=PID-> SP-FB;
	uint64_t  t_k=GetUs ();//当前时间 单位微妙
	float deltaT= (t_k-PID->t_k_1 )*1.0e-6f;//当前时间与上次的时间差 微妙转换成秒
float err_dev=0.0f;
		float  err_int=0.0f;
	 if(PID->t_k_1 !=0){
         err_dev=(err-PID->err_k_1 )/deltaT ;
	     
	 		 err_int=PID->err_int_k_1 +(err+PID->err_k_1 )*deltaT *0.5f;
	 }
	float COp=PID->Kp *err;
	float COi=PID->Ki *err_int ;
	float COd=PID->Kd *err_dev ;
  float CO=COp +COi +COd ;
	
	//更新
	PID->t_k_1 =t_k;
	PID->err_k_1 =err;
	PID->err_int_k_1 =err_int ;
	if(CO>PID->UpperLimit ) CO=PID->UpperLimit ;
	if(CO<PID->LowerLimit ) CO=PID->LowerLimit ;
	
	//限制积分项
	if(PID->err_int_k_1 >PID->UpperLimit ) PID->err_int_k_1 =PID->UpperLimit ;
	if(PID->err_int_k_1 <PID->LowerLimit ) PID->err_int_k_1 =PID->LowerLimit ;
	 
  return CO;	
}
//
//
//
void  PID_LimitConfig(PID_TypeDef *PID,float Upper,float lower){

PID->UpperLimit =Upper ;
	PID->LowerLimit =lower ;
}

//
//对PID控制器进行复位
//  对零时变量清零
void PID_Reset(PID_TypeDef  *PID){
PID->t_k_1 =0;
	PID->err_int_k_1 =0.0f;
	PID->err_k_1 =0.0f;

}