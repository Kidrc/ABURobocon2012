#include "IL.h"

char il_Buffer[15];
void IL_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	USART1_Init(38400);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStructure.DMA_BufferSize = 0x0F;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)il_Buffer;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	
	DMA_SetCurrDataCounter(DMA1_Channel5, 15);

	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC5);
		
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);

	RCC_GPIOA();
	GPIO_Config(GPIO_Pin_4, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOA);
	
	DMA_Cmd(DMA1_Channel5, ENABLE);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	Delay(20);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void DMA1_Channel5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC5);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	Delay(21);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}
	
int Read_IL(void)
{
	int data = 0;
	
	data = il_Buffer[7] * 100 + il_Buffer[8] * 10 + il_Buffer[9] - '0' * 111;
	
	if (il_Buffer[6] == '-')
	{
		data = -data;
	}
	
	if (data >= 999 || data <= -999)
	{
		return 0;
	}

	return data;
}
	
	