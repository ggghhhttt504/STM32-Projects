#include "app_usart2.h"
//
//@简介： 对串口二进行初始化，串口二作为平衡车的调式接口
//
void App_USART2_Init(void){
//USART2引脚TX RX分别接在PA2 和PA3；
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_2;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);
	
	a.GPIO_Pin =GPIO_Pin_3;
	a.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOA,&a);
	
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2 ,ENABLE);
	USART_InitTypeDef b={0};
	b.USART_BaudRate =115200;//平衡车选较高的波特率 不占时间
	
	b.USART_Mode=USART_Mode_Rx |USART_Mode_Tx ;
	b.USART_Parity=USART_Parity_No ;
	b.USART_StopBits=USART_StopBits_1 ;
	b.USART_WordLength=USART_WordLength_8b ;
	
	USART_Init (USART2,&b);
	USART_Cmd (USART2,ENABLE);
	
	
}