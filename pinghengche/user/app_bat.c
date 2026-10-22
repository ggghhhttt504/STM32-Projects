#include "app_bat.h"
#include "delay.h"

static volatile float tt = 0.0f;//电池电压值
static uint32_t stage = 0;//上次LED切换的时间
static uint32_t lastTime = 0;//当前的状态，0灭1亮

static void TIM2_TRGO_Init(void);
static void ADC1_Init(void);
static void LED_Init(void);



void App_Bat_Init(void)
{
	TIM2_TRGO_Init();
	ADC1_Init();
	LED_Init();
}

 

void App_Bat_Proc(void)
{
	if(tt > 7.6){
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	}else if(tt > 7.4){
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	}else if(tt > 7.0){
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	}else if(tt > 6.5){
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	}else{
		uint32_t sss = GetTick();
		if(sss - lastTime > 100){
			switch(stage){
				case 0:
					GPIO_WriteBit(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, Bit_SET);
					stage = 1;
					break;
				case 1:
					GPIO_WriteBit(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, Bit_RESET);
					stage = 0;
					break;
			}
			lastTime = sss;
		}
	}
}

//
//对电源指示灯进行初始化
//PA，PA5，PA6  OUT-PP
static void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef a = {0};
	a.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	a.GPIO_Mode = GPIO_Mode_Out_PP;
	a.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &a);
}
//初始化ADC
static void ADC1_Init(void)
{
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置为六分频
  //因为通道8接PB0  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef b = {0};
	b.GPIO_Pin = GPIO_Pin_0;
	b.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &b);
 //开启ADC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitTypeDef c = {0};
	c.ADC_ContinuousConvMode = DISABLE;//连续转换模式，用不到
	c.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	c.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//常规序列外部触发信号
	c.ADC_Mode = DISABLE;//双ADC模式
	c.ADC_NbrOfChannel = 0;
	c.ADC_ScanConvMode = DISABLE;//扫描模式，对多个通道转换，这里用不到
	ADC_Init(ADC1, &c);
//设置注入序列参数
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T2_TRGO);//定时器2的TRGO作为注入序列的外部触发
	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);//闭合外部触发开关
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_1Cycles5);
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);//闭合JEOC中断的开关
//配置中断
	NVIC_InitTypeDef d = {0};
	d.NVIC_IRQChannel = ADC1_2_IRQn;//配置中断号
	d.NVIC_IRQChannelPreemptionPriority = 0;
	d.NVIC_IRQChannelSubPriority = 2;
	d.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&d);
	ADC_Cmd(ADC1, ENABLE);//闭合ADC总开关
}

static void TIM2_TRGO_Init(void)
{//初始化TIM2_TRGO  让他每10ms产生一个脉冲
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef a = {0};
	a.TIM_CounterMode = TIM_CounterMode_Up;
	a.TIM_Period = 9999;
	a.TIM_Prescaler = 71;
	a.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &a);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//主机设置为Update模式
	TIM_Cmd(TIM2, ENABLE);
}


//中断响应函数
void ADC1_2_IRQHandler(void)
{
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC) == SET){
		ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
		uint16_t dd = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);//读取第一个寄存器，
		tt = (float)dd / 4095.0f * 8.4f;//VBAT的值，电池的电压
	}
}


//
//@简介：获取电池的当前电压的值，单位是V  测试用的
//
float App_Bat_Get(void)
{
	return tt;
}
