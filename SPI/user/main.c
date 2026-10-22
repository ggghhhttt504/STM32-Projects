#include "stm32f10x.h"
#include  "button.h"
void App_SPI_Init();
void App_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx,const uint8_t *pptx,uint8_t *pprx,uint16_t Size);
Button_TypeDef  button1;
void App_W25Q64_SaveByte(uint8_t Byte);
uint8_t  App_W25Q64_LoadByte();
void App_OnBoardLED_Init();
void App_Button_Init();
	void button_clicked_cb(uint8_t clicks); 
int main(void)
{App_SPI_Init();
	App_OnBoardLED_Init();
	App_Button_Init();
//	App_SPI_MasterTransmitReceive();
//	App_W25Q64_SaveByte(0x12);
	//a=  App_W25Q64_LoadByte();
	uint8_t byte=App_W25Q64_LoadByte();
	if(byte==0){
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
	}else{
		GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
	
	}
	
	while(1)
	{
		My_Button_Proc(&button1);
	}
}
void App_OnBoardLED_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef  s={0};
	s.GPIO_Pin =GPIO_Pin_13;
	s.GPIO_Mode =GPIO_Mode_Out_OD ;
	s.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&s);
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );

}
void App_Button_Init(){

Button_InitTypeDef  m={0};
m.GPIOx=GPIOA;
m.GPIO_Pin=GPIO_Pin_0;
m.button_clicked_cb = button_clicked_cb;

My_Button_Init (&button1,&m);



}
void button_clicked_cb(uint8_t clicks){
     if(clicks ==1){
		 if(GPIO_ReadOutputDataBit (GPIOC,GPIO_Pin_13)==Bit_SET ){
		  GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
			 App_W25Q64_SaveByte(0x01);
		 }else{
		  GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
	          App_W25Q64_SaveByte(0x00);
		 }
			 
		 }
}
void App_SPI_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO ,ENABLE);
	GPIO_PinRemapConfig (GPIO_Remap_SPI1 ,ENABLE);
	GPIO_PinRemapConfig (GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitTypeDef  b={0};
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
	//SCK PB3,AF-PP,2MHz;
	
	b.GPIO_Pin =GPIO_Pin_3;
	b.GPIO_Mode =GPIO_Mode_AF_PP ;
	b.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOB,&b);
   //MISO  输入上拉，PB4;
	
	b.GPIO_Pin =GPIO_Pin_4;
	b.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOB,&b);
	
	  //MOSI PB5  AF-PP,2MHz
	
	
	b.GPIO_Pin =GPIO_Pin_5;
	b.GPIO_Mode =GPIO_Mode_AF_PP ;
	b.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOB,&b);
   
	 //PA15  Out-PP  2MHZ;
    b.GPIO_Pin =GPIO_Pin_15;
b.GPIO_Mode =GPIO_Mode_Out_PP ;
b.GPIO_Speed =GPIO_Speed_2MHz ;
GPIO_Init (GPIOA,&b);
GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );
RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1 ,ENABLE);
SPI_InitTypeDef c={0};
c.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_64 ;
c.SPI_CPHA=SPI_CPHA_1Edge ;
c.SPI_CPOL=SPI_CPOL_Low ;
c.SPI_DataSize=SPI_DataSize_8b ;
c.SPI_Direction=SPI_Direction_2Lines_FullDuplex ;
c.SPI_FirstBit=SPI_FirstBit_MSB ;
c.SPI_Mode=SPI_Mode_Master ;
c.SPI_NSS=SPI_NSS_Soft ;
SPI_Init (SPI1,&c);
SPI_NSSInternalSoftwareConfig (SPI1,SPI_NSSInternalSoft_Set );
}
void App_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx,const uint8_t *pptx,uint8_t *pprx,uint16_t Size){
SPI_Cmd(SPIx,ENABLE);
SPI_I2S_SendData (SPIx,pptx[0]);
for(uint16_t i=0;i<Size-1;i++){
   while(SPI_I2S_GetFlagStatus (SPIx,SPI_I2S_FLAG_TXE )==RESET);
	SPI_I2S_SendData (SPIx,pptx[i+1]);
	 while(SPI_I2S_GetFlagStatus (SPIx,SPI_I2S_FLAG_RXNE )==RESET);
	pprx[i]=SPI_I2S_ReceiveData (SPIx);

}	
 while(SPI_I2S_GetFlagStatus (SPIx,SPI_I2S_FLAG_RXNE )==RESET);
	pprx[Size-1]=SPI_I2S_ReceiveData (SPIx);
SPI_Cmd(SPIx,DISABLE);
	
}

void App_W25Q64_SaveByte(uint8_t Byte){
	uint8_t buffer[10];
	buffer[0]=0x06;
	//写使能
	   	GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_RESET );//选中
	App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,1);
		GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );//取消选中
	//扇区清除
	buffer[0]=0x20;
	buffer[1]=0x00;
	buffer[2]=0x00;
	buffer[3]=0x00;
	GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_RESET );//选中
	App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,4);
			GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );//取消选中
	//等待
	while(1){
	GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_RESET );//选中
	//写0x05
		buffer[0]=0x05;
		
	App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,1);
		//都状态寄存器1
		buffer[0]=0xff;
		App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,1);
	
			GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );//取消选中
		if((buffer[0] &0x01)==0) break;
	}
	//写使能
	buffer[0]=0x06;
	GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_RESET );//选中
	App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,1);
		GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );//取消选中
	//页编程
	GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_RESET );//选中
	buffer [0]=0x02;
	buffer [1]=0x00;
	buffer [2]=0x00;
	buffer [3]=0x00;
	buffer [4]=Byte;
	App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,5);
	
		GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );//取消选中
	//等待
	while(1){
	GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_RESET );//选中
	//写0x05
		buffer[0]=0x05;
		
	App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,1);
		//都状态寄存器1
		buffer[0]=0xff;
		App_SPI_MasterTransmitReceive (SPI1,buffer ,buffer ,1);
	
			GPIO_WriteBit (GPIOA,GPIO_Pin_15,Bit_SET );//取消选中
		if((buffer[0] &0x01)==0) break;
	}
}
uint8_t  App_W25Q64_LoadByte(){
uint8_t buffer[10];
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET); // 选中
	
	buffer[0] = 0x03;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	
	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 4);
	
	buffer[0] = 0xff;
	
	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET); // 取消选中
	
	return buffer[0];
}
