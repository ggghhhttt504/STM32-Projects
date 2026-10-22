#include "stm32f10x.h"
void App_ADC_Init();
void  App_LED_Init();
int main(){
 App_ADC_Init();
	App_LED_Init(); 
while(1){

ADC_ClearFlag (ADC1,ADC_FLAG_EOC );//	清除EOC标志位
	//通过软件启动的方式发送脉冲
	ADC_SoftwareStartConvCmd (ADC1,ENABLE);
	//等待常规序列发送完成
	while(ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC)==RESET);
	//读取转换的结果
	
	uint16_t ttt=ADC_GetConversionValue (ADC1);
	//把结果转换成电压
	float tt=ttt*(3.3f/4095);
	
	if(tt>1.5){
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET );
	}else{
		GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_RESET );
	}
	
}

}
void App_ADC_Init(){
//初始化PA0引脚
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_0;
	a.GPIO_Mode =GPIO_Mode_AIN ;//模拟输入
	GPIO_Init(GPIOA,&a);
	
	RCC_ADCCLKConfig (RCC_PCLK2_Div6 );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1 ,ENABLE);
	ADC_InitTypeDef b={0};
	b.ADC_ContinuousConvMode =DISABLE;//关闭连续模式
	b.ADC_DataAlign =ADC_DataAlign_Right ;
	b.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None ;//软件触发
	b.ADC_Mode =ADC_Mode_Independent ;//独立模式
	b.ADC_NbrOfChannel =1;//常规序列1个通道
	b.ADC_ScanConvMode =DISABLE ;
	
	ADC_Init (ADC1,&b);
	//配置常规序列
	ADC_RegularChannelConfig (ADC1,ADC_Channel_0 ,1,ADC_SampleTime_13Cycles5 );
  ADC_ExternalTrigConvCmd (ADC1,ENABLE);//控制外部触发信号开关	
	//闭合ADC总开关
	ADC_Cmd (ADC1,ENABLE);
}
void  App_LED_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef  a={0};
	a.GPIO_Pin =GPIO_Pin_13;
	a.GPIO_Mode =GPIO_Mode_Out_OD ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOC,&a);



}