#include "stm32f10x.h"
#include "delay.h"
void App_OnBoardLED(void );
void App_Button(void);
int main(void)
{App_OnBoardLED();
	App_Button();
	uint8_t  zq=Bit_SET ,xz=Bit_SET ;
	while(1)
	{
		
		zq=xz;
		xz=GPIO_ReadInputDataBit (GPIOA,GPIO_Pin_0);
		if(zq!=xz){
		   if(xz==Bit_RESET){
			 //按下
			 if(GPIO_ReadOutputDataBit (GPIOC,GPIO_Pin_13)==Bit_SET ){//读取上一次的/值
			 
			   GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
			 }else{
			 
			  GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
			 }
		
		
		
		}
		Delay(10);
	}
	}
}
void App_OnBoardLED(void ){
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_13;
	a.GPIO_Mode =GPIO_Mode_Out_OD ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&a);
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
}
void App_Button(void ){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef b={0};
	b.GPIO_Pin =GPIO_Pin_0;
	b.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOA,&b);


}