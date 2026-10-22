#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"


void App_TIM3_Init();
void App_USART1_Init();
void App_TIM1_Init();
int main(void)
{
	App_USART1_Init();
	App_TIM3_Init();
	 App_TIM1_Init();
	TIM_SetCompare1 (TIM3,200);
//	My_USART_SendString (USART1,"hello");
	while(1)
	{
		
		//清除Trigger标志位
		TIM_ClearFlag (TIM1,TIM_FLAG_Trigger );
		//等待Trigger 标志位从0变成1
		while(TIM_GetFlagStatus (TIM1,TIM_FLAG_Trigger )==RESET);
		//计算
     uint16_t ccr1 =TIM_GetCapture1 (TIM1);
     uint16_t  ccr2=TIM_GetCapture2 (TIM1);
		float  period =ccr1*1.0e-6f *1.0e3f;//单位ms
     float  duty =(float )ccr2/ccr1*100.0f;	
    My_USART_Printf (USART1,"周期=%.3fms,占空比=%2.f%%\r\n",period,duty);	
		Delay(100);
		
	}
}
void App_TIM3_Init(){
RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3 ,ENABLE);
	TIM_TimeBaseInitTypeDef a={0};
	a.TIM_CounterMode =TIM_CounterMode_Up ;
	a.TIM_Period =999;
	a.TIM_Prescaler =71;
	a.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM3,&a);
	TIM_ARRPreloadConfig (TIM3,ENABLE);
	TIM_Cmd (TIM3,ENABLE);
	//TIM3_CH1  PA6  AF-PP
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  b={0};
	b.GPIO_Pin =GPIO_Pin_6;
	b.GPIO_Mode =GPIO_Mode_AF_PP ;
	b.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&b);
	//配置输出捕获一的参数  OC1
	TIM_OCInitTypeDef  c={0};
	c.TIM_OCMode =TIM_OCMode_PWM1 ;
	c.TIM_OCPolarity =TIM_OCPolarity_High ;
	c.TIM_OutputState =ENABLE ;
	c.TIM_Pulse =0;
	
	TIM_OC1Init (TIM3,&c);
	//使能MoE
	TIM_CtrlPWMOutputs (TIM3,ENABLE);
	
	TIM_CCPreloadControl (TIM3,ENABLE);
	
	

}
void App_TIM1_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1 ,ENABLE);
	TIM_TimeBaseInitTypeDef a={0};
	a.TIM_CounterMode =TIM_CounterMode_Up ;
	a.TIM_Period =65535;
	a.TIM_Prescaler =71;
	a.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM1,&a);
	TIM_ARRPreloadConfig (TIM1,ENABLE);
	TIM_Cmd (TIM1,ENABLE);

	//初始化输入捕获通道1 PA8;
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef b={0};
	b.GPIO_Pin =GPIO_Pin_8;
	b.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_Init (GPIOA,&b);
	
	TIM_ICInitTypeDef  c={0};
	c.TIM_Channel =TIM_Channel_1 ;
	c.TIM_ICFilter =0;
	c.TIM_ICPolarity =TIM_ICPolarity_Rising ;
	c.TIM_ICPrescaler =TIM_ICPSC_DIV1 ;
	c.TIM_ICSelection =TIM_ICSelection_DirectTI ;
	TIM_ICInit (TIM1,&c);
	
	c.TIM_Channel =TIM_Channel_2 ;
	c.TIM_ICFilter=0;
	c.TIM_ICPolarity =TIM_ICPolarity_Falling ;
	c.TIM_ICPrescaler =TIM_ICPSC_DIV1 ;
	c.TIM_ICSelection =TIM_ICSelection_IndirectTI ;
	TIM_ICInit (TIM1,&c);

	
	
	//初始化从模式控制器
	TIM_SelectInputTrigger (TIM1,TIM_TS_TI1FP1 );
	TIM_SelectSlaveMode (TIM1,TIM_SlaveMode_Reset );//复位模式
	
	

}
void App_USART1_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_9;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);
		a.GPIO_Pin =GPIO_Pin_10;
	a.GPIO_Mode =GPIO_Mode_IPU  ;
	GPIO_Init (GPIOA,&a);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef b={0};
	b.USART_BaudRate =115200;
	b.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;
	b.USART_Mode =USART_Mode_Rx |USART_Mode_Tx ;
	b.USART_Parity =USART_Parity_No ;
	b.USART_StopBits =USART_StopBits_1 ;
	b.USART_WordLength =USART_WordLength_8b ;
	USART_Init (USART1,&b);
	USART_Cmd (USART1,ENABLE);
	
}