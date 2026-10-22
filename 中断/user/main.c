#include "stm32f10x.h"
#include  "delay.h"
void App_OnBoardLED_Init();
void App_USART_Init();
uint32_t  t=1000;
int main(void)
{App_OnBoardLED_Init();
	App_USART_Init();
	//中断优先级分组
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	while(1)
	{
		GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
		Delay (t);
		GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
		Delay(t);
	}
}
void USART1_IRQHandler (){
	

	
	if(USART_GetFlagStatus (USART1,USART_FLAG_RXNE )==SET){
		 uint8_t ttt = USART_ReceiveData(USART1);
	
	if(ttt=='0'){
	
	 t=1000;
	}
	else if(ttt=='1'){
	
	t=200;
	}else if(ttt=='2'){
	
	t=50;
	
	}
	
	
	}
}

	
	
	
	
void App_OnBoardLED_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_13;
	a.GPIO_Mode =GPIO_Mode_Out_OD  ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&a);

}
void App_USART_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitTypeDef  b={0};
	b.GPIO_Pin =GPIO_Pin_9 ;
	b.GPIO_Mode =GPIO_Mode_AF_PP ;
	b.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&b);
	
	b.GPIO_Pin =GPIO_Pin_10;
	b.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOA,&b);
	
  USART_InitTypeDef c={0};
	c.USART_BaudRate=115200;
	c.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
	c.USART_Mode=USART_Mode_Rx |USART_Mode_Tx ;
  c.USART_Parity=USART_Parity_No ;
	c.USART_StopBits=USART_StopBits_1 ;
	c.USART_WordLength=USART_WordLength_8b ;
	USART_Init (USART1,&c);
	USART_Cmd (USART1,ENABLE);

//配置中断
	USART_ITConfig (USART1,USART_IT_RXNE ,ENABLE);
	//配置中断
	
	NVIC_InitTypeDef d={0};
	d.NVIC_IRQChannel =USART1_IRQn ;
	
	d.NVIC_IRQChannelPreemptionPriority=0;
	d.NVIC_IRQChannelSubPriority=0;
	d.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init (&d);
	
}