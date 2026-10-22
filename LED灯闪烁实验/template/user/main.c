#include "stm32f10x.h"

int main(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef a={0};

	a.GPIO_Pin =GPIO_Pin_1;
	a.GPIO_Mode =GPIO_Mode_IPU  ;
	GPIO_Init (GPIOA,&a);
	
	GPIO_InitTypeDef b={0};
	b.GPIO_Pin =GPIO_Pin_13;
	b.GPIO_Mode =GPIO_Mode_Out_OD ;
	b.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&b);
	while(1){
	if(GPIO_ReadInputDataBit (GPIOA,GPIO_Pin_1)==Bit_RESET ){
	//°´ÏÂ
		 GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET);
	
	}else{
	
	  GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET);
	}

}
	}