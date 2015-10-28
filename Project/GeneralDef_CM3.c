#include "GeneralDef_CM3.h"
uint16_t seconds = 0;

/*
*	函数名：LED_On
*	功能：打开指定LED
*	参数：无符号16位整数LED号，可以用GeneralDef.h里的宏定义，也可以是GPIO_Pin
*	返回值：无
*
*	2011/11/21
*	Version 1.0
*/
void LED_On(uint16_t LED)
{
	GPIO_ResetBits(GPIOB, LED);
}
/*
*	函数名：LED_On
*	功能：打开指定LED
*	参数：无符号16位整数LED号，可以用GeneralDef.h里的宏定义，也可以是GPIO_Pin
*	返回值：无
*
*	2011/11/21
*	Version 1.0
*/
void LED_Off(uint16_t LED)
{
	GPIO_SetBits(GPIOB, LED);
}										   

/*
*	函数名：GPIO_Config
*	功能：对指定GPIO口进行初始化
*	参数：GPIO引脚号GPIO_Pin;
*			引脚模式 GPIO_Mode;
*			速度Speed;
*			端口GPIOx.
*	返回值：无
*
*	2011/11/26
*	Version 1.0
*/

void GPIO_Config(uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed, GPIO_TypeDef * GPIOx)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void LED_Init(void)
{
	RCC_GPIOB();
	
	GPIO_Config(LED1|LED2|LED3|LED4, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);

	LED_Off(LED1|LED2|LED3|LED4);
}

void itoa(char * buffer, int num, int length)
{
	int i = 0;
	int temp = num;
	
	if (temp == 0)
	{
		for (i = 0; i < length; i++)
		{
			buffer[i] = '0';
		}

		buffer[i] = 0;
	}
	else
	{
		if (temp < 0)
		{
			buffer[0] = '-';
			
			temp = -temp;
			
			++i;
		}
		else
			buffer[0] = '0';
		
		while(temp != 0)
		{
			temp /= 10;
			
			++i;
		}

		if (length > i)
			i = length;

		buffer[i] = 0;
		--i;
		temp = num;
		
		if (temp < 0)
			temp = -temp;

		while(temp != 0)
		{
			buffer[i] = temp % 10 + '0';
			
			temp /= 10;
			
			--i;
		}

		while( i >= 0 && buffer[i] != '-')
		{
			buffer[i] = '0';
			
			--i;
		}
	}
}

#ifndef __stdlib_h

int atoi(char * buffer)
{
	int i;
	int negative = 0;
	int result = 0;
	
	for (i = 0; buffer[i] == '0'; i++);
	
	if (buffer[i] == 0)
		return 0;
	else
	{
		if (buffer[i] == '-')
		{
			negative = 1;
			++i;
		}
		
		for (; buffer[i]; i++)
		{
			result *= 10;
			result += buffer[i] - '0';
		}
	}
	
	if (negative)
		result = -result;
		
	return result;
}

#endif

#ifdef __STM32F10x_SPI_H			//SPI相关

void GPIO_SPI1_Init(void)
{
	GPIO_Config(SPI1_MOSI|SPI1_SCK, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOA);
	GPIO_Config(SPI1_NSS, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOA);
	GPIO_Config(SPI1_MISO, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOA);
}

void GPIO_SPI2_Init(void)
{
	GPIO_Config(SPI2_MOSI|SPI2_SCK, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOB);
	GPIO_Config(SPI2_NSS, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
	GPIO_Config(SPI2_MISO, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOB);
}
#endif

#ifdef __STM32F10x_USART_H

void USART1_Init(unsigned int BaudRate)
{
	USART_InitTypeDef USART_Structure;
	
	RCC_GPIOA();
	RCC_USART1();

	GPIO_Config(USART1_Tx, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOA);
	GPIO_Config(USART1_Rx, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOA);

	USART_Structure.USART_BaudRate = BaudRate;										
	USART_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	USART_Structure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;									
	USART_Structure.USART_Parity = USART_Parity_No;
	USART_Structure.USART_StopBits = USART_StopBits_1;
	USART_Structure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_Structure);											   

	USART_Cmd(USART1, ENABLE);	
}

void USART2_Init(unsigned int BaudRate)
{
	USART_InitTypeDef USART_Structure;
	
	RCC_USART2();
	
	RCC_GPIOD();
	RCC_AFIO();
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
	
	GPIO_Config(USART2_Tx, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOD);
	GPIO_Config(USART2_Rx, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOD);

	USART_Structure.USART_BaudRate = BaudRate;										
	USART_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	USART_Structure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;									
	USART_Structure.USART_Parity = USART_Parity_No;
	USART_Structure.USART_StopBits = USART_StopBits_1;
	USART_Structure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &USART_Structure);											   

	USART_Cmd(USART2, ENABLE);	
}

void USART3_Init(unsigned int BaudRate)
{
	USART_InitTypeDef USART_Structure;
	
	RCC_USART3();
	
	RCC_GPIOD();
	RCC_AFIO();
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	
	GPIO_Config(USART3_Tx, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, GPIOD);
	GPIO_Config(USART3_Rx, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOD);

	USART_Structure.USART_BaudRate = BaudRate;										
	USART_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	USART_Structure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;									
	USART_Structure.USART_Parity = USART_Parity_No;
	USART_Structure.USART_StopBits = USART_StopBits_1;
	USART_Structure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART3, &USART_Structure);											   

	USART_Cmd(USART3, ENABLE);	
}

void USART_Transmit(USART_TypeDef * USARTx, char * str)
{
	int i;

	for (i = 0;str[i] != 0; i++)
	{
		USART_SendData(USARTx, str[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		{
		}
	} 

	USART_SendData(USARTx, 0x0D);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
	{
	}  
}

#endif

#ifdef __STM32F10x_TIM_H

void GPIO_TIM3_Init(void)
{
	RCC_GPIOA();
	
	GPIO_Config(GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOA);
}

void GPIO_TIM4_Init(void)
{
	RCC_GPIOB();
	GPIO_Config(GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOB);
}

#endif	  

void Relay_Init(void)
{
	RCC_GPIOG();
	RCC_GPIOE();
	GPIO_Config(GPIO_Pin_1|GPIO_Pin_2, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOE);
	GPIO_Config(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOG);
}
void Relay_Switch(char node, char state)
{
	switch(node)
	{
		case 1:
			if (state == ON)
			{
				GPIO_SetBits(GPIOG, GPIO_Pin_4);
			}
			else
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_4);
			}
		break;
		
		case 2:
			if (state == ON)
			{
				GPIO_SetBits(GPIOG, GPIO_Pin_5);
			}
			else
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_5);
			}
		break;
		
		case 3:
			if (state == ON)
			{
				GPIO_SetBits(GPIOG, GPIO_Pin_6);
			}
			else
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_6);
			}
		break;
	
		case 4:
			if (state == ON)
			{
				GPIO_SetBits(GPIOG, GPIO_Pin_7);
			}
			else
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_7);
			}
		break;
		
		case 5:
			if (state == ON)
			{
				GPIO_SetBits(GPIOG, GPIO_Pin_8);
			}
			else
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_8);
			}
		break;
		
		case 6:
			if (state == ON)
			{
				GPIO_SetBits(GPIOG, GPIO_Pin_9);
			}
			else
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_9);
			}
		break;
		
		case 7:
			if (state == ON)
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
			}
			else
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			}
		break;

		case 8:
			if (state == ON)
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
			}
			else
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
			}
		break;
			
		default:
		break;
	}
}

void Sensor_Init(void)
{
	RCC_GPIOF();
	RCC_GPIOD();
	
	GPIO_Config(GPIO_Pin_All, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOF);
	GPIO_Config(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOD);
}

char Sensor_Active(sensor_Node node)
{
	char filter = 0;
	char delay_Time = 50;

	switch(node)
	{
		case F0:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0);
			}
			else
			{
				return 0;
			}
			
		case F1:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1);
			}
			else
			{
				return 0;
			}
		case F2:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2);
			}
			else
			{
				return 0;
			}
		case F3:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3);
			}
			else
			{
				return 0;
			}
		case F4:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4);
			}
			else
			{
				return 0;
			}
		case F5:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5);
			}
			else
			{
				return 0;
			}
		case F6:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6);
			}
			else
			{
				return 0;
			}
		case F7:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7);
			}
			else
			{
				return 0;
			}
		case F8:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);
			}
			else
			{
				return 0;
			}
		case F9:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9);
			}
			else
			{
				return 0;
			}
		case F10:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10);
			}
			else
			{
				return 0;
			}
		case F11:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11);
			}
			else
			{
				return 0;
			}
		case F12:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_12);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_12);
			}
			else
			{
				return 0;
			}
		case F13:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_13);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_13);
			}
			else
			{
				return 0;
			}
		case F14:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_14);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_14);
			}
			else
			{
				return 0;
			}
		case F15:
			filter = !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_15);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_15);
			}
			else
			{
				return 0;
			}
		case D0:
			filter = !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
			}
			else
			{
				return 0;
			}
		case D1:
			filter = !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
			}
			else
			{
				return 0;
			}
		case D2:
			filter = !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
			}
			else
			{
				return 0;
			}
		case D3:
			filter = !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
			
			if (filter)
			{
				Delay(delay_Time);
				
				return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
			}
			else
			{
				return 0;
			}
	
		default:
			return 0;
	}
}
			
void Switch_Init(void)
{
	RCC_GPIOG();
	
	GPIO_Config(GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOG);
}

char Switch_On(switch_Node node)
{
	char filter = 0;
	
	switch(node)
	{
		case G10:
			filter = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10);
			
			if (filter)
			{
				Delay(10);
				
				return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10);
			}
			else
			{
				return 0;
			}
		case G11:
			filter = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11);
			
			if (filter)
			{
				Delay(10);
				
				return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11);
			}
			else
			{
				return 0;
			}
		case G12:
			filter = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_12);
			
			if (filter)
			{
				Delay(10);
				
				return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_12);
			}
			else
			{
				return 0;
			}
		case G13:
			filter = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13);
			
			if (filter)
			{
				Delay(10);
				
				return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13);
			}
			else
			{
				return 0;
			}
		case G14:
			filter = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14);
			
			if (filter)
			{
				Delay(10);
				
				return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14);
			}
			else
			{
				return 0;
			}
		case G15:
			filter = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15);
			
			if (filter)
			{
				Delay(10);
				
				return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15);
			}
			else
			{
				return 0;
			}

		default:
			return 0;
	}
}

void Delay(uint16_t ms)	   //0.1ms的延迟，可能偏小
{
	char i = 0;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	if (ms > 1)
	{
		RCC_TIM7();
	
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		  	 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = ms-1;
		if (PSX_SWITCH == 1)
		{
			TIM_TimeBaseStructure.TIM_Prescaler = 2399;
		}
		else
		{
			TIM_TimeBaseStructure.TIM_Prescaler = 7199;
		}
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);  		

		while(i<1)
		{
			TIM_Cmd(TIM7, ENABLE);			  //当内层嵌套的Delay结束时，会使得外层Delay卡死，故将开关写在循环里面。

			if (TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) == SET)
			{
				i++;
				TIM_ClearFlag(TIM7, TIM_FLAG_Update);
			}
				
		}

		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		TIM_Cmd(TIM7,DISABLE);
	}
}
			
void Clock_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_Structure;

	RCC_TIM5();

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		  	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000;
	if (PSX_SWITCH == 1)
	{
		TIM_TimeBaseStructure.TIM_Prescaler = 2399;
	}
	else
	{
		TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	}
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);	
	
	TIM5->CNT = 0;
				
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);  		
	
	NVIC_Structure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Structure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_Init(&NVIC_Structure);

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM5, DISABLE);	
}

void Clock_Start(void)
{
	seconds = 0;
	TIM_Cmd(TIM5, ENABLE);
}

void Clock_Stop(void)
{
	TIM_Cmd(TIM5, DISABLE);
}

void TIM5_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	++seconds;
}