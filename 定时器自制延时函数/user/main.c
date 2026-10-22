#include "stm32f10x.h"

volatile uint32_t  currentTick =0;
void App_TIM3_TimeBaseInit();
void App_delay(uint32_t ms);
void  App_OnBoardLED_Init();
int main(void)
{
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	 App_TIM3_TimeBaseInit();
	App_OnBoardLED_Init();
	while(1)
	{
		GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
		App_delay (500);
			GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
		App_delay (500);
	}
}

   void TIM3_IRQHandler (){
	 if(TIM_GetFlagStatus (TIM3,TIM_FLAG_Update )==SET){
	 TIM_ClearFlag (TIM3,TIM_FLAG_Update );
		 currentTick ++;
	 }
	 } 
void App_delay(uint32_t ms){
uint32_t t=currentTick +ms;
	while(currentTick<t);
}
void App_TIM3_TimeBaseInit(){

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypeDef a={0};
	a.TIM_Prescaler =71;
	a.TIM_Period =999;
	a.TIM_CounterMode =TIM_CounterMode_Up ;
	a.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM3,&a);
	
	TIM_Cmd (TIM3,ENABLE);
	
	TIM_ITConfig (TIM3,TIM_IT_Update ,ENABLE);
	
	NVIC_InitTypeDef b={0};
  b.NVIC_IRQChannel =TIM3_IRQn ;
	b.NVIC_IRQChannelPreemptionPriority=0;
	b.NVIC_IRQChannelSubPriority=0;
  b.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init (&b);
}

void  App_OnBoardLED_Init(){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef c={0};
	c.GPIO_Pin =GPIO_Pin_13;
	c.GPIO_Mode=GPIO_Mode_Out_OD ;
	c.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&c);

}