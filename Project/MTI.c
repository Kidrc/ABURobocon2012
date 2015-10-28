#include "MTI.h"

const uint8_t FRAMEHEAD[4] = {0xFA,0xFF,0x32,0x0E};
uint8_t buffer[19];
mti_Angle init_Angle;

void MTI_Init(void)
{
	DMA_InitTypeDef DMA_Structure;

	LCD_Inform("MTI");

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	USART2_Init(115200);
 	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

	DMA_Structure.DMA_BufferSize = (uint32_t)0x0000;		 
	DMA_Structure.DMA_DIR = DMA_DIR_PeripheralSRC;			
	DMA_Structure.DMA_M2M = DMA_M2M_Disable;						
	DMA_Structure.DMA_MemoryBaseAddr = (uint32_t)buffer;		  
	DMA_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	  
	DMA_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  
	DMA_Structure.DMA_Mode = DMA_Mode_Circular;						  
	DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
	DMA_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Structure.DMA_Priority = DMA_Priority_High;

	DMA_Init(DMA1_Channel6, &DMA_Structure);
	DMA_SetCurrDataCounter(DMA1_Channel6, 0x13);
			
	DMA_Cmd(DMA1_Channel6, ENABLE);

	Delay(40);

	init_Angle = MTI_GetData();

	Delay(40);

	init_Angle = MTI_GetData();
}

mti_Angle MTI_GetData(void)
{
	int BtoD(uint8_t buffer[]);

	mti_Angle angle;
	int i = 0;
	char find = 0;
	
	while(!find)
	{
		if (buffer[i] == FRAMEHEAD[0])
		{
			if (buffer[i+1] == FRAMEHEAD[1])
			{
				if (buffer[i+2] == FRAMEHEAD[2])
				{
					if (buffer[i+3] == FRAMEHEAD[3])
					{
						find = 1;
						
						angle.roll = BtoD(buffer + (i+4) % 19);
						angle.pitch = BtoD(buffer + (i+8) % 19);
						angle.yaw = BtoD(buffer + (i+12) % 19);
					}
					else
						i = (i+4) % 19;
				}
				else
					i = (i+3) % 19;
			}
			else
				i = (i+2) % 19;
		}
		else
			i = (++i) % 19;
	}
	

	return angle;
}
mti_Angle MTI_GetAngle(void)
{
	mti_Angle angle;

	angle = MTI_GetData();

	angle.roll -= init_Angle.roll;
	angle.pitch -= init_Angle.pitch;
	angle.yaw -= init_Angle.yaw;
/*
	if (angle.roll >= 180)
	{
		angle.roll = angle.roll - 360;
	}
	else if (angle.roll < -180)
	{
		angle.roll = angle.roll + 360;
	}
	
	if (angle.pitch >= 180)
	{
		angle.pitch = angle.pitch - 360;
	}
	else if (angle.roll < -180)
	{
		angle.pitch = angle.pitch + 360;
	}
*/
	if (angle.yaw >= 180)
	{
		angle.yaw = angle.yaw - 360;
	}
	else if (angle.yaw < -180)
	{
		angle.yaw = angle.yaw + 360;
	}
		
	return angle;
}
int BtoD(uint8_t buffer[])
{
	int16_t temp;

	temp = (int16_t)buffer[0] << 8;
	temp |= (uint16_t)buffer[1];
	temp >>= 4;

	return temp;
}

void USART_SendInt(USART_TypeDef * USART, int a)
{
	if (a<0)
	{
		USART_SendData(USART, '-');
		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
		a= -a;
	}
	USART_SendData(USART, a/100+'0');
	//while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
	USART_SendData(USART, a%100/10+'0');
	while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
	USART_SendData(USART, a%10+'0');
	while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
}

void MTI_UpdateInitAngle(void)
{
	init_Angle = MTI_GetData();
}

double Delta_Angle(double a1, double a2)
{
	double a = a1-a2;

	if (a >= 180)
	{
		a -= 360;
	}
	else if (a < -180)
	{
		a += 360;
	}

	return a;
}