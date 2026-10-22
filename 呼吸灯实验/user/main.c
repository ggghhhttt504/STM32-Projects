#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

// 函数声明
void App_PWM_Init(void);

/**
 * @brief 主函数
 * @param 无
 * @retval 无
 */
int main(void)
{
    // 初始化PWM
    App_PWM_Init();

    // 主循环：生成正弦波呼吸灯
    while(1)
    {
        // 获取当前系统运行时间（单位：秒）
        float t = GetTick() * 1.0e-3f;
        
        // 计算正弦波占空比：范围 0~1
        // sin(2*π*t + 1) 范围 -1~1，+1 后范围 0~2，*0.5 后范围 0~1
        float duty = sin((2 * 3.14 * t) + 1) * 0.5;
        
        // 将占空比转换为 CCR 值：0~1000
        int ccr1 = duty * 1000;
        
        // 设置 TIM1 通道1 的比较值（改变占空比）
        TIM_SetCompare1(TIM1, ccr1);
    }
}

/**
 * @brief PWM 初始化函数
 * @param 无
 * @retval 无
 */
void App_PWM_Init(void)
{
    // ================================================
    // 第一步：初始化 GPIO 引脚
    // ================================================
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能 GPIOA 和 GPIOB 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 PA8 为 TIM1_CH1（主通道输出）
    // PA8 作为 PWM 主通道输出引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;    // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;   // 2MHz 速度
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置 PB13 为 TIM1_CH1N（互补通道输出）
    // PB13 作为 PWM 互补通道输出引脚（与 PA8 波形相反）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;    // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;   // 2MHz 速度
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // ================================================
    // 第二步：配置定时器时基单元
    // ================================================
    
    // 使能 TIM1 的时钟（TIM1 是高级定时器，挂在 APB2 总线上）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // 配置时基单元参数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_Period = 999;        // 自动重装载值 ARR = 999
                                                    // PWM 周期 = (999+1) / 1MHz = 1ms
                                                    // PWM 频率 = 1kHz
    TIM_TimeBaseInitStruct.TIM_Prescaler = 71;      // 预分频器 PSC = 71
                                                    // 计数时钟 = 72MHz / (71+1) = 1MHz
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // 重复计数器（高级定时器特有）
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
    
    // 使能 ARR 自动重装载预加载寄存器（保证更新时不会产生毛刺）
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    // 使能定时器（启动计数）
    TIM_Cmd(TIM1, ENABLE);

    // ================================================
    // 第三步：配置输出比较通道（PWM 输出）
    // ================================================
    
    // 配置 TIM1 通道1 为 PWM 输出
    TIM_OCInitTypeDef TIM_OCInitStruct = {0};
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;           // PWM 模式1
                                                             // 向上计数时：CNT < CCR 时输出有效电平
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;   // 主通道输出极性：高电平有效
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High; // 互补通道输出极性：高电平有效
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;       // 主通道输出使能
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;     // 互补通道输出使能
    TIM_OCInitStruct.TIM_Pulse = 0;                          // 初始 CCR 值为 0（占空比 0%）
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);

    // ================================================
    // 第四步：闭合 MOE 主输出使能开关
    // ================================================
    
    // 高级定时器特有：必须使能 MOE（Main Output Enable）才能输出 PWM
    // 这是高级定时器的安全机制，防止上电时引脚输出不确定状态
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    // ================================================
    // 第五步：开启 CCR 预加载功能
    // ================================================
    
    // 使能 CCR 寄存器的预加载（更新事件发生时才更新 CCR 值）
    // 防止在 PWM 周期中间改变占空比导致波形不完整
    TIM_CCPreloadControl(TIM1, ENABLE);
}