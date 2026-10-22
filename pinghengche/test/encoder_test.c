#include "encoder_test.h"
#include "app_usart2.h"
#include "delay.h"
#include  "app_encoder.h"
//
//对编码器位置信号进行测试，通过调试用的串口把编码器的当前位置显示的vofa
//
void  Encoder_Test(void){

App_USART2_Init ();
	App_Encoder_Init ();
while(1){

	float   pos_l=App_Encoder_GetPos_L();
	float   pos_r=App_Encoder_GetPos_R();
	My_USART_Printf (USART2,"%f,%f\n",pos_l ,pos_r );


Delay(50);

}
}




static  float  last_pos_l=0.0f;
static  float  last_pos_r=0.0f;
//
//
//对M法测速进行测试
void Encoder_M_Method_Test(void){
App_Encoder_Init();
	App_USART2_Init ();

while(1){
	Delay(1);
	
	float   pos_l=App_Encoder_GetPos_L ();
	float    pos_r=App_Encoder_GetPos_R ();

	float M_l=pos_l -last_pos_l ;
	float M_r=pos_r -last_pos_r ;
	
	float omega_l=M_l/0.001f;
	float omega_r=M_r/0.001f; 
	
	My_USART_Printf (USART2,"%f,%f,%f,%f\n",pos_l ,pos_r,omega_l ,omega_r    );
	last_pos_l =pos_l ;
	last_pos_r =pos_r ;
  	

}

}
//
//T 法测速   通过串口把T法测速的Omegar值发送到VOFA
//
void Encoder_T_Method_Test(void){
App_USART2_Init ();
	App_Encoder_Init ();

while(1){

Delay(1);//每间隔1ms 打印
	
	float  omegar_l=App_Encoder_GetSpeed_L();
	float  omegar_r=App_Encoder_GetSpeed_R();
	
	
	My_USART_Printf (USART2,"%f,%f\n",omegar_l ,omegar_r );
	

}
}