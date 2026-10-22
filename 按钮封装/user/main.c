#include "stm32f10x.h"
#include "usart.h"
#include "button.h"
Button_TypeDef  button1;
void App_USART_Init();
void App_button_Init();
	void button_clicked_cb(uint8_t clicks);
		void button_long_pressed_cb(uint8_t ticks); 
int cnt=0;
int main(){
 App_USART_Init();
 App_button_Init();
while(1){
My_Button_Proc(&button1);

}

}
void App_USART_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_9;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init(GPIOA,&a);
	a.GPIO_Pin =GPIO_Pin_10;
	a.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init(GPIOA,&a);
	USART_InitTypeDef b={0};
	b.USART_BaudRate =115200;
	b.USART_Mode =USART_Mode_Rx |USART_Mode_Tx ;
	b.USART_Parity =USART_Parity_No ;
	b.USART_StopBits =USART_StopBits_1 ;
	b.USART_WordLength =USART_WordLength_8b ;
	USART_Init (USART1,&b);
	USART_Cmd (USART1,ENABLE);
}
void App_button_Init(){
Button_InitTypeDef  b={0};
b.GPIOx =GPIOA;
b.GPIO_Pin =GPIO_Pin_0;
b.ClickInterval =0;
b.LongPressTickInterval =0;
b.LongPressTime =0;
b.button_clicked_cb =button_clicked_cb;
b.button_long_pressed_cb =button_long_pressed_cb;
b.button_pressed_cb=0;
b.button_released_cb =0;
My_Button_Init(&button1,&b);
}
void button_clicked_cb(uint8_t clicks){

if(clicks==1){
cnt++;
	 My_USART_Printf(USART1,"%d",cnt);

}
if(clicks==2){
cnt=0;
	 My_USART_Printf(USART1,"%d",cnt);
}
}
	void button_long_pressed_cb(uint8_t ticks){
	cnt++;
		
	 My_USART_Printf(USART1,"%d",cnt);
	
	}
