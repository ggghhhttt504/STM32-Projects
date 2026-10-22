#include "app_pwm.h"
#include "math.h"
static void STBY_Pin_Init(void);//对STBY初始化，控制电机启动还是休眠的  第一步
static void Motor_L_Init(void);//左电机的初始化
static void Motor_R_Init(void);//右电机的初始化
//
//对TB6612初始化
//
void  App_PWM_Init(void){
 STBY_Pin_Init();//STBY引脚初始化
	Motor_L_Init();//左电机初始化
	Motor_R_Init();//右电机初始化
}

//
//初始化STBY引脚
//PA1  Out-PP
//
static void STBY_Pin_Init(void){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_1;
	a.GPIO_Mode =GPIO_Mode_Out_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);

}
//
//简介： 控制TB6612进入休眠或者活动状态
//
//on=0时休眠，写低电压，0n非零时活动，写高电压，
void App_PWM_Cmd(uint8_t on){

if(on==0){
GPIO_WriteBit (GPIOA,GPIO_Pin_1,Bit_RESET );//休眠

}else{
GPIO_WriteBit (GPIOA,GPIO_Pin_1,Bit_SET );//活动

}
}
//
//设置左电机的占空比
//-100.0<duty <100.0  占空比 
void  App_PWM_Set_L(float Duty){

float sign;//符号 1正转，-1反转
	
	if(Duty>=0){
	   sign=1;
	}else{
	sign=-1;
	}
	Duty=fabs(Duty);
	
	if(sign<0){
	  GPIO_WriteBit (GPIOA,GPIO_Pin_9,Bit_SET );//AIN1 高
		GPIO_WriteBit (GPIOA,GPIO_Pin_10,Bit_RESET );//AIN2 低
	}else{
	
	 GPIO_WriteBit (GPIOA,GPIO_Pin_9,Bit_RESET );//AIN1 低
		GPIO_WriteBit (GPIOA,GPIO_Pin_10,Bit_SET );//AIN2 高
	
	}
	
	//算出ccr的值
	uint16_t  ccr=Duty/100.0f*999;
	//把ccr的值复制给定时器的通道1
	TIM_SetCompare1 (TIM1,ccr);
	
	
}
//
//设置右电机的占空比
void  App_PWM_Set_R(float Duty){

float sign;//符号 1正转，-1反转
	
	if(Duty>=0){
	   sign=1;
	}else{
	sign=-1;
	}
	Duty=fabs(Duty);
	
	if(sign>=0){
	  GPIO_WriteBit (GPIOB,GPIO_Pin_5,Bit_SET );//BIN1 高
		GPIO_WriteBit (GPIOB,GPIO_Pin_7,Bit_RESET );//BIN2 低
	}else{
	
	 GPIO_WriteBit (GPIOB,GPIO_Pin_5,Bit_RESET );//BIN1 低
		GPIO_WriteBit (GPIOB,GPIO_Pin_7,Bit_SET );//BIN2 高
	
	}
	
	//算出ccr的值
	uint16_t  ccr=Duty/100.0f*999;
	//把ccr的值复制给定时器的通道1
	TIM_SetCompare1 (TIM4,ccr);
	

}
//
////左电机的初始化   
//AIN1 PA9 Out-PP
//AIN2_PA10_out-pp
static void Motor_L_Init(void){
//初始化AIN1 和AIN2
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  a={0};
	a.GPIO_Pin =GPIO_Pin_9|GPIO_Pin_10;
	a.GPIO_Mode =GPIO_Mode_Out_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);
	
	//初始化PWM
	//PA8 AF-PP
	//TIM1_CH1
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	a.GPIO_Pin=GPIO_Pin_8;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);
	
	//对定时器1进行初始化 TIM1
	//开启定时器1的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1,ENABLE);
	//设置时基单元的参数
	TIM_TimeBaseInitTypeDef  b={0};
	b.TIM_CounterMode=TIM_CounterMode_Up ;
	b.TIM_Period=999;
	b.TIM_Prescaler=0;
	b.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit (TIM1,&b);
	
	//配置输出比较
	TIM_OCInitTypeDef  c={0};
	c.TIM_OCMode =TIM_OCMode_PWM1 ;//功能模式
	c.TIM_OCPolarity =TIM_OCPolarity_High ;//正常输出的极性
	c.TIM_OutputState =ENABLE;//正常输出的开关
	c.TIM_Pulse =0;
	TIM_OC1Init (TIM1,&c);
	
	
	//打开MMOE的开关
	
	TIM_CtrlPWMOutputs (TIM1 ,ENABLE);
	
	//闭合定时器的总开关
	TIM_Cmd (TIM1,ENABLE);
}
//
////右电机的初始化   
//BIN1 PB5 Out-PP
//BIN2_PB7_out-pp
static void Motor_R_Init(void){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef  a={0};
	a.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_7;
	a.GPIO_Mode =GPIO_Mode_Out_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOB,&a);
	//初始化PWM
	//PB6 AF-PP
	//TIM4_CH1
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	a.GPIO_Pin=GPIO_Pin_6;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOB,&a);
	
	//对定时器4进行初始化
	//开启定时器4的时钟
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);
	//设置时基单元的参数
	TIM_TimeBaseInitTypeDef  b={0};
	b.TIM_CounterMode=TIM_CounterMode_Up ;
	b.TIM_Period=999;
	b.TIM_Prescaler=0;
	b.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit (TIM4,&b);
	//配置输出比较
	TIM_OCInitTypeDef  c={0};
	c.TIM_OCMode =TIM_OCMode_PWM1 ;
	c.TIM_OCPolarity =TIM_OCPolarity_High ;
	c.TIM_OutputState =ENABLE;
	c.TIM_Pulse =0;
	TIM_OC1Init (TIM4,&c);
	
	
	//配置MOE的开关
	TIM_CtrlPWMOutputs (TIM4,ENABLE); //只有高级定时器才有，TIM1 TIM8 高级定时器
	
	//闭合定时器的总开关
	TIM_Cmd (TIM4,ENABLE);
}
