#include "stm32f10x.h"
#include "si2c.h"
SI2C_TypeDef si2c;
//#include  "i2c.h"
//void My_Init1();
void  My_LED_Init();
int main(void)
	
{ //My_Init1();
	My_LED_Init ();
	si2c.SCL_GPIOx=GPIOB;
	si2c.SCL_GPIO_Pin =GPIO_Pin_6;
	si2c.SDA_GPIOx =GPIOB;
	si2c.SDA_GPIO_Pin =GPIO_Pin_7;
	My_SI2C_Init (&si2c);
	uint8_t tt[]={0x00,0x8d,0x14,0xaf,0xa5};
  My_SI2C_SendBytes(&si2c,0x78,tt,5);
	//My_I2C_SendBytes(I2C1,0x78,tt,5);
	uint8_t ttt;
	 My_SI2C_ReceiveBytes(&si2c,0x78,&ttt,1);
	//My_I2C_ReceiveBytes(I2C1,0x78,&ttt,1);
	
	if((ttt&(0x01<<6))==0){
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
	
	}else{
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );}
	while(1)
	{
	}
}
//void My_Init1(){
//RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitTypeDef  a={0};
//	a.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7;
//	a.GPIO_Mode =GPIO_Mode_AF_OD ;
//	a.GPIO_Speed =GPIO_Speed_2MHz ;
//	GPIO_Init(GPIOB,&a);
//	RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1 ,ENABLE);
//	RCC_APB1PeriphResetCmd (RCC_APB1Periph_I2C1,ENABLE);
//	RCC_APB1PeriphResetCmd (RCC_APB1Periph_I2C1,DISABLE);
//	I2C_InitTypeDef b={0};
//	b.I2C_ClockSpeed =400000;
//	b.I2C_Mode =I2C_Mode_I2C ;
//	b.I2C_DutyCycle =I2C_DutyCycle_2 ;
//	I2C_Init(I2C1,&b);
//	I2C_Cmd (I2C1,ENABLE);
//	
//	
//	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
//	GPIO_InitTypeDef  c={0};
//	c.GPIO_Pin =GPIO_Pin_13;
//	c.GPIO_Mode =GPIO_Mode_Out_OD ;
//	c.GPIO_Speed =GPIO_Speed_2MHz ;
//	GPIO_Init(GPIOC,&c);
//	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );


//}
void My_LED_Init(){

//	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
//	GPIO_InitTypeDef  c={0};
//	c.GPIO_Pin =GPIO_Pin_13;
//	c.GPIO_Mode =GPIO_Mode_Out_OD ;
//	c.GPIO_Speed =GPIO_Speed_2MHz ;
//	GPIO_Init(GPIOC,&c);
//	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );

}