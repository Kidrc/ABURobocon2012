#include "Encoder.h"

/*2路对应B端口，X方向
*	1路对应A端口，Y方向
*/
uint16_t counter[2] = {0, 0};
const double PI = 3.14159265;
double maxCNT1 = 0;
double maxCNT2 = 0;

int C = 50*PI;
uint16_t RES1 = 1000;
uint16_t RES2 = 2000;

void Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	GPIO_TIM4_Init();
	GPIO_TIM3_Init();

	RCC_TIM3();
	RCC_TIM4();

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
 	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM4->CNT = 0x7FFF;
	TIM3->CNT = 0x7FFF;

	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

double Read_Encoder(char node)
{
	static double cnt = 0;

	if (node == 1)
	{
		cnt = (TIM4->CNT - 0x7FFF)/(double)RES1 * C/2;

		if (TIM4->CNT > 4000+0x7FFF || TIM4->CNT < -4000+0x7FFF)
		{
			TIM4->CNT = 0x7FFF;
			return 0;
		}

		if (cnt != 0)
		{
			TIM4->CNT = 0x7FFF;

		}
	}
	else
	{
		cnt = (TIM3->CNT - 0x7FFF)/(double)RES2 * C/2;

		if (TIM3->CNT > 4400+0x7FFF || TIM3->CNT < -4400+0x7FFF)
		{
			TIM3->CNT = 0x7FFF;
			return 0;
		}

		if (cnt != 0)
		{
			TIM3->CNT = 0x7FFF;
		}

	}
	
	return cnt;
}

void Update_Encoder(char node)
{
	if (node == 1)
	{
		TIM4->CNT = 0x7FFF;
	}
	else
	{
		TIM3->CNT = 0x7FFF;
	}
}