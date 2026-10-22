#include "stm32f10x.h"
void My_I2C_Init();
int My_I2C_SendBytes(I2C_TypeDef *I2Cx,uint8_t Addr,uint8_t *p,uint16_t  Size);
void My_OnBoardLED_Init(void);
int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pp, uint16_t Size);
int main(){
	My_I2C_Init();
	 My_OnBoardLED_Init();
uint8_t  pp[]={0x00,0x8d,0x14,0xaf,0xa5};
My_I2C_SendBytes (I2C1,0x78,pp,5);
uint8_t rcvd;
	
	My_I2C_ReceiveBytes(I2C1, 0x78, &rcvd, 1);
	
	if((rcvd & (0x01 << 6)) == 0)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	}
while(1){


}
}
void My_I2C_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig (GPIO_Remap_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef  a={0};
	a.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_9;
	a.GPIO_Mode =GPIO_Mode_AF_OD ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
GPIO_Init (GPIOB,&a);


RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1 ,ENABLE	);
	RCC_APB1PeriphResetCmd (RCC_APB1Periph_I2C1 ,ENABLE);
RCC_APB1PeriphResetCmd (RCC_APB1Periph_I2C1 ,DISABLE);
	I2C_InitTypeDef  b={0};
	b.I2C_ClockSpeed =400000;
	b.I2C_Mode =I2C_Mode_I2C ;
	b.I2C_DutyCycle =I2C_DutyCycle_2 ;
	I2C_Init (I2C1,&b);
	I2C_Cmd (I2C1,ENABLE);



}
int My_I2C_SendBytes(I2C_TypeDef *I2Cx,uint8_t Addr,uint8_t *p,uint16_t  Size){

  while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_BUSY )==SET);
	
	
	//·¢ËÍÆðÊ¼Î»
	I2C_GenerateSTART (I2Cx,ENABLE);
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_SB )==RESET);
	
	//Ñ°Ö·½×¶Î
	//Çå³ýAf
	I2C_ClearFlag (I2Cx,I2C_FLAG_AF);
	I2C_SendData (I2Cx,Addr &0xfe);
	
	
	while(1){
	
	if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -1; // Ñ°Ö·Ê§°Ü
		}
	if(I2C_GetFlagStatus (I2Cx,I2C_FLAG_ADDR )==SET){
	
	   break;
		
	}
	
	}
	
	
	I2C_ReadRegister (I2Cx,I2C_Register_SR1 );
	I2C_ReadRegister (I2Cx,I2C_Register_SR2 );
	
	
	for(uint16_t i=0;i<Size;i++){
	
	  while(1){
		
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -2; // Ñ°Ö·Ê§°Ü
		}
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) == RESET){
		  break;
		}
		
		}
		
	I2C_SendData(I2Cx, p[i]);
	}
	
	while(1){
	
		
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -2; // Ñ°Ö·Ê§°Ü
		}
		
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF ) == SET){
		  break;
		}
	
	}
	
	I2C_GenerateSTOP (I2Cx,ENABLE);
	return 0;
}
int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pp, uint16_t Size){

//·¢ËÍÆðÊ¼Î»
	I2C_GenerateSTART (I2Cx,ENABLE);
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_SB )==RESET);
	
I2C_ClearFlag (I2Cx,I2C_FLAG_AF);
	I2C_SendData (I2Cx,Addr |0x01);
	
while(1){
	
	if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -1; // Ñ°Ö·Ê§°Ü
		}
	if(I2C_GetFlagStatus (I2Cx,I2C_FLAG_ADDR )==SET){
	
	   break;
		
	}
	
	}
if(Size==1){
I2C_ReadRegister (I2Cx,I2C_Register_SR1 );
	I2C_ReadRegister (I2Cx,I2C_Register_SR2 );
	I2C_AcknowledgeConfig (I2Cx,DISABLE );
	I2C_GenerateSTOP (I2Cx,ENABLE);
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_RXNE )==RESET);
	pp[0]=I2C_ReceiveData (I2Cx);
	
	


}
if(Size==2){
I2C_ReadRegister (I2Cx,I2C_Register_SR1 );
	I2C_ReadRegister (I2Cx,I2C_Register_SR2 );
	I2C_AcknowledgeConfig (I2Cx,ENABLE );
	
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_RXNE )==RESET);
	pp[0]=I2C_ReceiveData (I2Cx);
	
	I2C_AcknowledgeConfig (I2Cx,DISABLE );
	I2C_GenerateSTOP (I2Cx,ENABLE);
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_RXNE )==RESET);
	pp[1]=I2C_ReceiveData (I2Cx);
	
}
if(Size>2){
I2C_ReadRegister (I2Cx,I2C_Register_SR1 );
	I2C_ReadRegister (I2Cx,I2C_Register_SR2 );
	I2C_AcknowledgeConfig (I2Cx,ENABLE );
	
	for(uint16_t i=0;i<Size;i++){
	
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_RXNE )==RESET);
	pp[i]=I2C_ReceiveData (I2Cx);
	}
I2C_AcknowledgeConfig (I2Cx,DISABLE );
	I2C_GenerateSTOP (I2Cx,ENABLE);
	while(I2C_GetFlagStatus (I2Cx,I2C_FLAG_RXNE )==RESET);
	pp[Size-1]=I2C_ReceiveData (I2Cx);

}
return 0;
}
void My_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

