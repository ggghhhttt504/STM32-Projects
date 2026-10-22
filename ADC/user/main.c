#include "stm32f10x.h"
#include "usart.h"
void  App_USART_Init();
void  App_TIM1_Init();
void App_ADC_Init();
int main(void)
{
	
	App_USART_Init();
	App_TIM1_Init();
	App_ADC_Init();
	while(1)
	{
		
		while(ADC_GetFlagStatus (ADC1,ADC_FLAG_JEOC )==RESET);
		ADC_ClearFlag (ADC1 ,ADC_FLAG_JEOC );
		uint16_t  dd=ADC_GetInjectedConversionValue (ADC1,ADC_InjectedChannel_1 );
		uint16_t  dd1=ADC_GetInjectedConversionValue (ADC1,ADC_InjectedChannel_2 );
		float t1,t2;
		t1=dd*(3.3f/4095);
		t2=dd1*(3.3f/4095);
		//把结果发送出去  串口
		My_USART_Printf (USART1,"%.3f,%.3f\n",t1,t2);
	}
}

void  App_USART_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_9;
	a.GPIO_Mode =GPIO_Mode_AF_PP ;
	a.GPIO_Speed =GPIO_Speed_2MHz ;
	GPIO_Init (GPIOA,&a);
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 ,ENABLE);
	USART_InitTypeDef b={0};
	b.USART_BaudRate =115200;
	b.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;
	b.USART_Mode =USART_Mode_Tx ;
	b.USART_Parity =USART_Parity_No ;
	b.USART_StopBits =USART_StopBits_1 ;
	b.USART_WordLength =USART_WordLength_8b ;
	USART_Init (USART1,&b);
	USART_Cmd(USART1,ENABLE);

}
void  App_TIM1_Init(){

RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1 ,ENABLE);
	TIM_TimeBaseInitTypeDef a={0};
	a.TIM_CounterMode =TIM_CounterMode_Up ;
	a.TIM_Period =999;
	a.TIM_Prescaler =71;
	a.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM1,&a);
	
  TIM_ARRPreloadConfig (TIM1,ENABLE);
	
	//将 TRGIO设置为Update模式
	TIM_SelectOutputTrigger (TIM1,TIM_TRGOSource_Update );
	TIM_Cmd (TIM1,ENABLE);
}
void App_ADC_Init(){
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef a={0};
	a.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;
	a.GPIO_Mode =GPIO_Mode_AIN ;
	GPIO_Init (GPIOA,&a);
	
	RCC_ADCCLKConfig (RCC_PCLK2_Div6 );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1 ,ENABLE);
	ADC_InitTypeDef b={0};
	b.ADC_ContinuousConvMode =DISABLE;//连续
	b.ADC_DataAlign =ADC_DataAlign_Right ;
	b.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None ;//常规序列不需要
	b.ADC_Mode =ADC_Mode_Independent ;//双ADC模式   关闭
	b.ADC_NbrOfChannel =1;//常规随便选
	b.ADC_ScanConvMode =ENABLE;//扫描模式  本次能用到开启  
ADC_Init (ADC1,&b);
	
	//配置注入序列
	ADC_InjectedSequencerLengthConfig (ADC1,2);
	
	//把通道1，2填入注入序列
	ADC_InjectedChannelConfig (ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5  );
		ADC_InjectedChannelConfig (ADC1,ADC_Channel_1,2,ADC_SampleTime_13Cycles5  );
	
	
	//把定时器1TRGO选为外部触发信号
	ADC_ExternalTrigInjectedConvConfig (ADC1,ADC_ExternalTrigInjecConv_T1_TRGO );
	//闭合外部触发开关
	ADC_ExternalTrigInjectedConvCmd (ADC1,ENABLE);
	//闭合总开关
	ADC_Cmd (ADC1,ENABLE); 
	
	

}