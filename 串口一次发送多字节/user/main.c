#include "stm32f10x.h"
#include  <stdio.h>
#include "delay.h"
void  USART1_Init();
void LED_Init();
//void  USART1_Sendbytes(USART_TypeDef *USARTx,int *p,int  Size);
int main(void)
{USART1_Init();
	LED_Init();
//	int pp[]={1,2,3,4,5};
//	 USART1_Sendbytes(USART1,pp,5);
	//printf("231305009nihao\r\n");
//	uint32_t  d=GetTick();
//		uint32_t h=d%1000;
//		d=d/1000;
//		uint32_t m=d%60;
//		d=d/60;
//		uint32_t f=d%60;
//		d=d/60;
//	uint32_t s=d;
//		printf("%02d:%02d:%02d.%03d",s,f,m,h);
	while(1)
	{
//		uint32_t  d=GetTick();
//		uint32_t h=d%1000;
//		d=d/1000;
//		uint32_t m=d%60;
//		d=d/60;
//		uint32_t f=d%60;
//		d=d/60;
//	uint32_t s=d;
//		printf("%02d:%02d:%02d.%03d",s,f,m,h);
//		Delay (100);
//		
//		int d=GetTick();
//		int h=d%1000;
//		d=d/1000;
//		int m=d%60;
//		d=d/60;
//		int f=d%60;
//		d=d/60;
//		int s=d;
//		printf("%02d:%02d:%02d.%03d",s,f,m,h);
//		Delay (100);
//		

while(USART_GetFlagStatus (USART1,USART_FLAG_RXNE )==RESET );
uint8_t dd=USART_ReceiveData (USART1);

   if(dd=='0'){
	 GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
	 } 
	 if(dd=='1'){
	 
	  GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
	 }

	}
}
void  USART1_Init(){

  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB |RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig (GPIO_Remap_USART1 ,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_6;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB,&a);
	a.GPIO_Pin =GPIO_Pin_7 ;
	a.GPIO_Mode =GPIO_Mode_IPU ;
		GPIO_Init (GPIOB,&a);
	USART_InitTypeDef b={0};
	b.USART_BaudRate =115200;
	b.USART_Mode =USART_Mode_Rx |USART_Mode_Tx ;
	b.USART_Parity =USART_Parity_No ;
	b.USART_StopBits=USART_StopBits_1 ;
	b.USART_WordLength =USART_WordLength_8b ;
	USART_Init (USART1,&b);
	USART_Cmd (USART1 ,ENABLE);
	
}
//void  USART1_Sendbytes(USART_TypeDef *USARTx,int *p,int   Size){
//  
//for(int   i=0;i<Size;i++){
//  while(USART_GetFlagStatus (USARTx,USART_FLAG_TXE )==RESET);
//	USART_SendData (USARTx,p[i]);

//}
//while(USART_GetFlagStatus (USARTx,USART_FLAG_TC)==RESET);
//}

//int fputc(int ch,FILE *f){
// while(USART_GetFlagStatus (USART1,USART_FLAG_TXE )==RESET);
//	USART_SendData (USART1,(uint8_t )ch);
//	return ch;

//}
void LED_Init(){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef  d={0};
	d.GPIO_Pin =GPIO_Pin_13;
	d.GPIO_Mode =GPIO_Mode_Out_OD ;
	d.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&d);
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );

}