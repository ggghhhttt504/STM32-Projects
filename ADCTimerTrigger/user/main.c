#include "stm32f10x.h"
#include "usart.h"

void App_USART1_Init(void);
void App_TIM1_Init(void);
void App_ADC_Init(void);

int main(void)
{
	App_USART1_Init();
	App_TIM1_Init();
	App_ADC_Init();
	
//	My_USART_SendString(USART1, "Hello world. \r\n");
	
	while(1)
	{
		// #1. 等待注入序列转换完成
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC) == RESET);
		
		// #2. 读取转换的结果
		uint16_t jdr1 = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
		
		// #3. 清除JEOC标志位
		ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
		
		// #4. 把结果转换成电压
		float voltage = jdr1 * (3.3f / 4095);
		
		// #5. 通过串口把结果发送出发去
		My_USART_Printf(USART1, "%.3f\n", voltage);
		
	}
}

void App_USART1_Init(void)
{
	// #1. 初始化IO引脚，PA9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// #2. 开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// #3. 配置串口的参数
	USART_InitTypeDef USART_InitStruct = {0};
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	// #4. 闭合串口的总开关
	USART_Cmd(USART1, ENABLE);
	
}

void App_TIM1_Init(void)
{
	// #1. 使能TIM1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	// #2. 配置TIM1的时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	// #3. TRGO:Update
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
	
	// #4. 开启定时器1的总开关
	TIM_Cmd(TIM1, ENABLE);
}

void App_ADC_Init(void)
{
	// #1. 初始化IO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// #2. 配置ADC模块的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	// #3. 配置ADC的基本参数
	
	ADC_InitTypeDef ADC_InitStruct = {0};
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// #4. 配置注入序列的额外参数
	ADC_InjectedSequencerLengthConfig(ADC1,1);
	
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_TRGO);
	
	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);
	
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	
	// #5. 闭合总开关
	ADC_Cmd(ADC1, ENABLE);
}

