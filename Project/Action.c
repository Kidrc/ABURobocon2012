#include "Action.h"

/*	
	收腿H
	放L
	打鱼竿H
	夹L
	放H
*/
/*
*	函数名：Action_Init
*	功能：对动作执行程序的初始化，包括：
*		PWM波初始化（H桥）
*		IO口初始化（动作执行部分）
*		机构回到初始位置
*	参数：无
*	返回值：无
*/

char elvtr_Arr = 0;
char elvtr_Man = 0;

char tar_Pos = 0;
char tar_Rolls = 1;

char elvtr_Dir = 0;
uint16_t init_Pos = 0;
 uint16_t rolls = 1;
 uint16_t pos = 0;

	
void Action_Init(void)
{
	LCD_Inform("Action");

	HBridge_Init();
	Sensor_Init();
	Relay_Init();
	Com_Init();
	AbsEncoder_Init();
	Touch_Init();

	ARM_In();
	ARM_Up();
	TKN_Dispatch();
	Com_Off();

	Delay(500);
	init_Pos = Read_AbsEncoder();
}
	  
void Com_Init(void)
{
	RCC_GPIOC();
	GPIO_Config(GPIO_Pin_13, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOC);
}

void Touch_Init(void)
{
	RCC_GPIOB();
	GPIO_Config(GPIO_Pin_8, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOB);
}
/*
*	函数名：Elevator_Up
*	功能：让电梯上升
*		该函数必须先于Elevator_Down执行
*	参数：无
*	返回值：无
*/
void ELVTR_Up(void)
{
	Relay_Switch(1, OFF);
	
	HBridge_Ctrl(1, 1, 1, 90);

	elvtr_Dir = 0;
}

/*
*	函数名：Elevator_Down
*	功能：让电梯下降
*	参数：无
*	返回值：无
*/
void ELVTR_Down(void)
{				
	Relay_Switch(1, OFF);
	
	HBridge_Ctrl(1, 1, 0, 90); 

	elvtr_Dir = 1;
}


/*
*	函数名：Elevator_Stop_Y
*	功能：让电梯在竖直方向上停止
*	参数：无
*	返回值：无
*/
void ELVTR_Stop(void)
{
	HBridge_Ctrl(1, 1, 0, 0);
	
	Relay_Switch(1, ON);

	elvtr_Arr = 1;
}

/*
*	函数名：Elevator_Goto_Y
*	功能：让电梯竖直运动到指定位置
*		指定传感器触发
*	参数：elevator_Y y -- 指定位置
*	返回值：无
*/
void ELVTR_Goto(elvtr_Pos y)
{
	switch(y)
	{
		case LOADC1:
			tar_Rolls = 1;
		break;

		case LIFTC1:
			tar_Rolls = 8;
		break;

		case UNLOADC:
			tar_Rolls = 1;
		break;

		case LOADBASKET:
			tar_Rolls = 1;
		break;

		case LIFTBASKET1:
			tar_Rolls = 9;
		break;

		case LIFTBASKET2:
			tar_Rolls = 16;
		break;

		case LOADC2:
			tar_Rolls = 7;
		break;

		case LIFTC2:
			tar_Rolls = 24;
		break;

		case LIFTBUN:
			tar_Rolls = 27;
		break;

		case PUTBUN:
			tar_Rolls = 9;
		break;

		default:
		break;
	}
	elvtr_Arr = 0;
}

void ELVTR_Check(void)
{
	static uint16_t last_Pos = 1024;

	pos = Read_AbsEncoder();

	if (last_Pos != 1024)
	{			
		if (pos > last_Pos + 450)// && elvtr_Dir == 0)
		{
			++rolls;
			elvtr_Arr = 0;
		}
		else if (pos + 450 < last_Pos)// && elvtr_Dir == 1)
		{
			--rolls;
			elvtr_Arr = 0;
		}	
	
		if (!elvtr_Arr && !elvtr_Man)
		{
			if (tar_Rolls > rolls)
			{
				ELVTR_Up();
			}
			else if (tar_Rolls < rolls)
			{
				ELVTR_Down();
			}
			else
			{
				ELVTR_Stop();
			}
		}
	}
	last_Pos = pos;
}

char ELVTR_Arrived(void)
{
	return elvtr_Arr;
}

uint16_t ELVTR_Pos(void)
{
	return rolls;
}

void ELVTR_Manual(char a)
{
	elvtr_Man = a;
}

char ELVTR_IsManual(void)
{
	return elvtr_Man;
}

void TKN_Patch(void)
{
	Relay_Switch(8, ON);
}

void TKN_Dispatch(void)
{
	Relay_Switch(8, OFF);
}
void ARM_Out(void)
{
	Relay_Switch(7, ON);
}

void ARM_In(void)
{
	Relay_Switch(7, OFF);
}

void ARM_Up(void)
{
	Relay_Switch(6, OFF);
}

void ARM_Down(void)
{
	Relay_Switch(6, ON);
}

void Com_On(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void Com_Off(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void AbsEncoder_Init(void)
{
	RCC_GPIOB();
	
	GPIO_Config(GPIO_Pin_10, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOB);

	GPIO_Config(GPIO_Pin_1|GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
		
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}
			
uint16_t Read_AbsEncoder(void)
{
	uint16_t abs_Data = 0;
	uint16_t dummy = 0;
	char i = 0;
	char state = 0;


	while(state != 16 && state != 17 && state != 18)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		for (dummy = 0; dummy < 20; dummy++);

		for (i = 0; i < 32; ++i)
		{
			if (i % 2)
			{
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_1);
			}
			for (dummy = 0; dummy < 10; dummy++);
	
	 		if (i % 2 != 0)
			{
				abs_Data <<= 1;
				abs_Data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
			} 
		}

		state = (abs_Data & 0x003E) >> 1;

		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		for (dummy = 0; dummy < 20; dummy++);
	}

	abs_Data = (abs_Data & 0xFFC0) >> 6;


	return abs_Data;
}

void PrdIT_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_Structure;

	RCC_TIM7();

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		  	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 100;
	TIM_TimeBaseStructure.TIM_Prescaler = 1199;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	
	
	TIM7->CNT = 0;
				
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);  		
	
	NVIC_Structure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Structure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_Init(&NVIC_Structure);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM7, ENABLE);	
}

void TIM7_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}
	