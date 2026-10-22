#include "stm32f10x.h"
#include  "usart.h"
#include  "delay.h"
void  App_USART1_Init();
void App_HCSR04_Init();
int main(void)
{
	 App_USART1_Init();
	 App_HCSR04_Init();
	My_USART_SendString (USART1,"hello world\r\n");
	while(1)
	{
		
		//向CNT写零
		TIM_SetCounter (TIM1,0);
		//清除cc1,cc2标志位
		TIM_ClearFlag (TIM1,TIM_FLAG_CC1);
		TIM_ClearFlag (TIM1,TIM_FLAG_CC2);
		//开启定时器
		TIM_Cmd (TIM1,ENABLE);
		
		//向TRTG发送10us脉冲
		GPIO_WriteBit (GPIOA,GPIO_Pin_0,Bit_SET );
		DelayUs(10);
		
		GPIO_WriteBit (GPIOA,GPIO_Pin_0,Bit_RESET );
		//等待测量完成
		while(TIM_GetFlagStatus  (TIM1,TIM_FLAG_CC1 )==RESET);
		while(TIM_GetFlagStatus  (TIM1,TIM_FLAG_CC2 )==RESET );
		//关闭定时器
		TIM_Cmd(TIM1,ENABLE);
		
		uint16_t  ccr1=TIM_GetCapture1 (TIM1);
			uint16_t  ccr2=TIM_GetCapture2 (TIM1);
			float  distance =(ccr2-ccr1)*1.0e-6f*340.0f/2;
			My_USART_Printf (USART1,"distance=%.4f\r\n",distance );
			Delay(100);
	}
}
 
void  App_USART1_Init(){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_9;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);
	a.GPIO_Pin =GPIO_Pin_10;
	a.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOA,&a);
	
	
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
void App_HCSR04_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1 ,ENABLE);
	TIM_TimeBaseInitTypeDef  b={0};
	b.TIM_CounterMode =TIM_CounterMode_Up ;
	b.TIM_Period =65535;
	b.TIM_Prescaler =71;
	b.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM1,&b);
	
 //初始话输入捕获  IO
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  c={0};
	c.GPIO_Pin =GPIO_Pin_8;
	c.GPIO_Mode =GPIO_Mode_IPD ;
	GPIO_Init (GPIOA,&c);
	
	//初始化输入捕获  1
	TIM_ICInitTypeDef  d={0};
	
	d.TIM_Channel =TIM_Channel_1 ;
  d.TIM_ICFilter =0;//不使用滤波器
	d.TIM_ICPolarity =TIM_ICPolarity_Rising ;
d.TIM_ICPrescaler =TIM_ICPSC_DIV1 ;//1分屏
	d.TIM_ICSelection =TIM_ICSelection_DirectTI ;
	
	TIM_ICInit (TIM1,&d);
	
	//通道2
	
	d.TIM_Channel =TIM_Channel_2 ;
	d.TIM_ICFilter =0;
	d.TIM_ICPolarity =TIM_ICPSC_DIV1 ;
	d.TIM_ICPrescaler =TIM_ICPolarity_Falling ;
	d.TIM_ICSelection =TIM_ICSelection_IndirectTI ;
	TIM_ICInit (TIM1,&d);
	
	//初始化Trig引脚  PA0；
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	c.GPIO_Pin =GPIO_Pin_0;
	c.GPIO_Mode =GPIO_Mode_Out_PP;
	c.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&c);
	

}