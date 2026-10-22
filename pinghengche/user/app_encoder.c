#include "app_encoder.h"
#include "delay.h"

static  volatile  int64_t    encoder_l=0;//左电机编码器的值

static  volatile  int64_t    encoder_r=0;//右电机编码器的值

static   volatile   int8_t  direction_l=1;//左电机旋转的方向 1正  -1反
	
static   volatile   int8_t  direction_r=1;//右电机旋转的方向 1正  -1反

static   volatile   uint64_t  t0_l=0,t1_l=0;//左电机编码器发生变化的时间

static   volatile   uint64_t  t0_r=0,t1_r=0;//右电机编码器发生变化的时间


static  void Encoder_L_Init(void);//左编码器初始化
static  void Encoder_R_Init(void);//右编码器初始化
//
//对编码器模块进行初始化
//
void App_Encoder_Init(void){

Encoder_L_Init();
	Encoder_R_Init();
}

//测试读取左轮胎角度  encoder_l是线
float   App_Encoder_GetPos_L(void){

	return encoder_l/22.0f/(30613.0f/1500.0f)*360.0f;


}

//测试读取右编码器位置
float   App_Encoder_GetPos_R(void){
 
	return encoder_r/22.0f/(30613.0f/1500.0f)*360.0f;
}
//左电机旋转的角速度  omegar  单位是弧度，之前成360是度，motor需要改成弧度

float   App_Encoder_GetSpeed_L(void){
	
	__disable_irq();//关闭单片机的总中断
	
	int8_t direction_cpy=direction_l;
	uint64_t t0_cpy=t0_l;
	uint64_t t1_cpy=t1_l;
	__enable_irq();//开启单片机的中断 
	
	if(direction_cpy==2||direction_cpy==-2){return 0.0f;}
	uint64_t  now=GetUs();
	
	float T;
	if(t0_cpy-t1_cpy>now-t0_cpy){
	T=(t0_cpy-t1_cpy)*1.0e-6f;
	
	}else{
	
	T=(now-t0_cpy)*1.0e-6f;
	
	}
	
	

return  direction_cpy/T/22.0f/(30613.0f/1500.0f)*6.2831853f;

}

float   App_Encoder_GetSpeed_R(void){
	
	__disable_irq();//关闭单片机的总中断
	
	int8_t direction_cpy=direction_r;
	uint64_t t0_cpy=t0_r;
	uint64_t t1_cpy=t1_r;
	__enable_irq();//开启单片机的中断 
	
	if(direction_cpy==2||direction_cpy==-2){return 0.0f;}
	uint64_t  now=GetUs();
	
	float T;
	if(t0_cpy-t1_cpy>now-t0_cpy){
	T=(t0_cpy-t1_cpy)*1.0e-6f;
	
	}else{
	
	T=(now-t0_cpy)*1.0e-6f;
	
	}
	
	

return  direction_cpy/T/22.0f/(30613.0f/1500.0f)*6.2831853f;

	
	

return  direction_r/T/22.0f/(30613.0f/1500.0f)*360.0f;


}//右电机旋转的角速度

//左编码器初始化
static  void Encoder_L_Init(void){

//初始化IO引脚  PB14 PB15   IPU
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_14|GPIO_Pin_15;
	a.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOB,&a);
	//EXTI 初始化
	//让EXTI14 监控PB14
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig (GPIO_PortSourceGPIOB ,GPIO_PinSource14 );//配置复用器，监控A

	//配置EXTI的参数
	EXTI_InitTypeDef b={0};
	b.EXTI_Line =EXTI_Line14 ;
	b.EXTI_LineCmd =ENABLE;
	b.EXTI_Mode =EXTI_Mode_Interrupt ;//中断模式
	b.EXTI_Trigger =EXTI_Trigger_Rising_Falling ;
	EXTI_Init (&b);
	//开启EXTI中断
	NVIC_InitTypeDef c={0};
	c.NVIC_IRQChannel =EXTI15_10_IRQn ;//中断编号
	c.NVIC_IRQChannelCmd=ENABLE;
	c.NVIC_IRQChannelPreemptionPriority =0;
	c.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(&c);
	
}
//右编码器初始化

static  void Encoder_R_Init(void){

//初始化IO引脚
	//PB3 PB4 IPU
	//关闭JIAG，开启swd
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO ,ENABLE);
	GPIO_PinRemapConfig ( GPIO_Remap_SWJ_JTAGDisable ,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4;
	a.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_Init (GPIOB,&a);
	
		//EXTI 初始化
	//让EXTI3 监控PB3  A项  B只用检测高低电平
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig (GPIO_PortSourceGPIOB ,GPIO_PinSource3 );
	//配置EXTI的参数
	EXTI_InitTypeDef b={0};
	b.EXTI_Line =EXTI_Line3 ;
	b.EXTI_LineCmd =ENABLE;
	b.EXTI_Mode =EXTI_Mode_Interrupt ;//中断模式
	b.EXTI_Trigger =EXTI_Trigger_Rising_Falling ;
	EXTI_Init (&b);
		//开启EXTI中断
	NVIC_InitTypeDef c={0};
	c.NVIC_IRQChannel =EXTI3_IRQn ;//中断编号
	c.NVIC_IRQChannelCmd=ENABLE;
	c.NVIC_IRQChannelPreemptionPriority =0;
	c.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(&c);
}
//
//EXTI 14 的中断响应函数 对应左编码器A相
//
void EXTI15_10_IRQHandler(void){
	if(EXTI_GetFlagStatus(EXTI_Line14)==SET){
		//10-15通道共用一个中断，所以要先判断是不死通道14触发的中断
		EXTI_ClearFlag (EXTI_Line14 );//对标准位进行清零
		t1_l=t0_l;
		t0_l=GetUs();//当前
	  uint8_t a=GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_14);  //相当于A的电压
		uint8_t b=GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_15);  //相当于B的电压
	   
	if((a==Bit_SET&&b==Bit_SET)||(a==Bit_RESET&&b==Bit_RESET)){//现在轮胎正转
   	encoder_l++;
		
		if(direction_l<0){ //之前轮胎是反转
		   direction_l=2;
		}else{
			direction_l =1;
		}
	
	}else{//现在轮胎是反转
	encoder_l--;
		if(direction_l>0){
		//之前轮胎是正转
			direction_l=-2;
		
		}else{
			direction_l =-1;
		}
	 
	}
	 
	
	}
 }
//
//EXTI 3 的中断响应函数 对应右编码器A相
//
void EXTI3_IRQHandler(void){
	
	EXTI_ClearFlag (EXTI_Line3 );//对标准位进行清零
		t1_r=t0_r;
		t0_r=GetUs();//当前
	  uint8_t a=GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_3);  //相当于A的电压
		uint8_t b=GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_4);  //相当于B的电压
	   
	
	if((a==Bit_SET&&b==Bit_RESET)||(a==Bit_RESET&&b==Bit_SET)){
	//现在轮胎是正转
	encoder_r++;
		
		if(direction_r<0){
		//之前轮胎是反转
			direction_r=2;
		}else{
			direction_r =1;
		}
	}else{
	//现在轮胎是反转
		
		encoder_r--;
		if(direction_r>0){
			//之前轮胎是正转
		    direction_r=-2;
		}else{
				direction_r =-1;
		}
	}
	
	}


