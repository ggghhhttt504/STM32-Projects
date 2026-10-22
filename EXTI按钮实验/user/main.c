#include "stm32f10x.h"
void App_OnBoardLED_Init();
void App_Button_Init();
int main(void)
{
	App_OnBoardLED_Init();
	 App_Button_Init();
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	while(1)
	{
	}
}
void EXTI9_5_IRQHandler(){

if(EXTI_GetFlagStatus (EXTI_Line5)==SET){
  EXTI_ClearFlag (EXTI_Line5 );
GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET);
}

if(EXTI_GetFlagStatus (EXTI_Line6)==SET){
  EXTI_ClearFlag (EXTI_Line6 );
GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET);
}


}
void App_OnBoardLED_Init(){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_13;
	a.GPIO_Mode =GPIO_Mode_Out_OD ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&a);
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
}
void App_Button_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef b={0};
	b.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_6;
	b.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOA,&b);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig (GPIO_PortSourceGPIOA ,GPIO_PinSource5 );
	GPIO_EXTILineConfig (GPIO_PortSourceGPIOA ,GPIO_PinSource6 );
	
	EXTI_InitTypeDef c={0};
	c.EXTI_Line=EXTI_Line5 ;
	c.EXTI_Mode=EXTI_Mode_Interrupt ;
	c.EXTI_Trigger=EXTI_Trigger_Rising ;
	c.EXTI_LineCmd=ENABLE;
	EXTI_Init (&c);
	
		c.EXTI_Line=EXTI_Line6 ;
	c.EXTI_Mode=EXTI_Mode_Interrupt ;
	c.EXTI_Trigger=EXTI_Trigger_Rising ;
	c.EXTI_LineCmd=ENABLE;
	EXTI_Init (&c);
	
	NVIC_InitTypeDef  d={0};
	d.NVIC_IRQChannel =EXTI9_5_IRQn ;
	d.NVIC_IRQChannelPreemptionPriority=0;
	d.NVIC_IRQChannelSubPriority=0;
	d.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init (&d);
	
	
	



}