#include "PWM.h"
/*
*	函数名：PWM_Init
*	功能：初始化PWM模块。
*	参数：
		外设号TIMx;
		通道号channel;
		频率freq
*	无返回值
*/
void PWM_Init(TIM_TypeDef * TIMx, pwm_Channel channel, uint16_t freq)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	if (TIMx == TIM8)
	{
		RCC_TIM8();
		RCC_GPIOC();
		
		if (channel & 0x01)
		{
			GPIO_Config(GPIO_Pin_6, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOC);
		}
		if (channel & 0x02)
		{
			GPIO_Config(GPIO_Pin_7, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOC);
		}
		if (channel & 0x04)
		{
			GPIO_Config(GPIO_Pin_8, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOC);
		}
		if (channel & 0x08)
		{
			GPIO_Config(GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOC);
		}
	}
	else if (TIMx == TIM5)
	{
		RCC_TIM5();
		RCC_GPIOA();
		
		if (channel & 0x01)
		{
			GPIO_Config(GPIO_Pin_0, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOA);
		}
		if (channel & 0x02)
		{
			GPIO_Config(GPIO_Pin_1, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOA);
		}
		if (channel & 0x04)
		{
			GPIO_Config(GPIO_Pin_2, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOA);
		}
		if (channel & 0x08)
		{
			GPIO_Config(GPIO_Pin_3, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOA);
		}
	}
	else if (TIMx == TIM1)
	{
		RCC_TIM1();
		RCC_GPIOE();
		RCC_AFIO();

		GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
		
		if (channel & 0x01)
		{
			GPIO_Config(GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOE);
		}
		if (channel & 0x02)
		{
			GPIO_Config(GPIO_Pin_11, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOE);
		}
		if (channel & 0x04)
		{
			GPIO_Config(GPIO_Pin_13, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOE);
		}
		if (channel & 0x08)
		{
			GPIO_Config(GPIO_Pin_14, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOE);
		}
	}

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 999;
	
	if (PSX_SWITCH == 1)
	{
		TIM_TimeBaseStructure.TIM_Prescaler = 24000/freq-1;
	}
	else
	{
		TIM_TimeBaseStructure.TIM_Prescaler = 72000/freq-1;
	}
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}

/*
*	函数名：PWM_Generate
*	功能：使用TIM1定时器，从PA8引脚输出指定频率和占空比的PWM波。
*	参数：分频系数prescaler（时钟频率为默认72MHz，可从system_stm32f10x.c里改）；
		  周期period；
		  比较值pulse（占空比 = pulse / period）；
*	返回值：无
*
*	2011/12/16
*	改成控制电机的模式：
*	使用TIM8定时器，CH1-PC6，CH2-PC7, CH3-PC8, CH4-PC9
*	参数改为：
*		通道号channel，枚举类型
*		频率freq
*		占空比ratio
*
*	2011/11/11
*	Version 1.0
*/

void PWM_Generate(TIM_TypeDef * TIMx, pwm_Channel channel, uint8_t ratio)
{
	TIM_OCInitTypeDef TIM_OCStructure;
	
	TIM_OCStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStructure.TIM_Pulse = ratio * 10;

	TIM_Cmd(TIMx, DISABLE);
	
	if (channel & 0x01)
	{

		TIM_OC1Init(TIMx, &TIM_OCStructure);

		TIM_CtrlPWMOutputs(TIMx, ENABLE);

		TIM_Cmd(TIMx, ENABLE);
	}
	if (channel & 0x02)
	{
		TIM_OC2Init(TIMx, &TIM_OCStructure);

		TIM_CtrlPWMOutputs(TIMx, ENABLE);
		TIM_Cmd(TIMx, ENABLE);
	}
	if (channel & 0x04)
	{
		TIM_OC3Init(TIMx, &TIM_OCStructure);

		TIM_CtrlPWMOutputs(TIMx, ENABLE);
		TIM_Cmd(TIMx, ENABLE);
	}
	if (channel & 0x08)
	{
		TIM_OC4Init(TIMx, &TIM_OCStructure);

		TIM_CtrlPWMOutputs(TIMx, ENABLE);
		TIM_Cmd(TIMx, ENABLE);
	}
	
}

void PWM_Stop(TIM_TypeDef * TIMx, pwm_Channel channel)
{
	PWM_Generate(TIMx, channel, 0);
}

void HBridge_Init(void)
{
	PWM_Init(TIM1, PWM_CH1|PWM_CH2|PWM_CH3|PWM_CH4, 1000);
	
	GPIO_Config(GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOE);
}

void HBridge_Ctrl(char hNode, char channel, char dir, char speed)
{

	if (hNode == 2)
	{
		if (channel == 1)
		{
			//这里的顺序是不合逻辑的。但是按正常顺序写的话什么输出都没有，不能解决，只好这样了。
			PWM_Generate(TIM1, PWM_CH1, speed);	 
			  
			if (dir == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			}
			else
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_3);
			} 
		}
		if (channel == 2)
		{
			if (dir == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
			}
			else
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_4);
			}
			PWM_Generate(TIM1, PWM_CH2, speed);
		}
	}
	else if (hNode == 1)
	{

		PWM_Generate(TIM1, PWM_CH3, speed);
		if (channel == 1)
		{
			if (dir == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
			}
			else
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
			}
		}
		else if (channel == 2)
		{
			if (dir == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
			}
			else
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_6);
			}
			PWM_Generate(TIM1, PWM_CH4, speed);
		}
	}
}

void Steer_Init(void)
{
	PWM_Init(TIM5, PWM_CH1|PWM_CH2|PWM_CH3|PWM_CH4, 50);
}

void Steer_Ctrl(steer_Node node, char pos)
{
	switch(node)
	{
		case PA0:
			PWM_Generate(TIM5, PWM_CH1, pos);
		break;
		
		case PA1:
			PWM_Generate(TIM5, PWM_CH2, pos);
		break;
		
		case PA2:
			PWM_Generate(TIM5, PWM_CH3, pos);
		break;
		
		case PA3:
			PWM_Generate(TIM5, PWM_CH4, pos);
		break;
		
		default:
		break;
	}
}
	
			
		
	
			
			