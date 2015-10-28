/*
*	2012/03/07
*	半自动控制。																							 。
*	2011/12/10
*	改成基础控制版本，控制w,v1,v2。
*/
#include "PSXCtrl.h"
#include "Navigate.h"

char ctrl_Mode = 0;
extern char data_EN;		 
extern char autoDisable;

void PSXCtrl_Init(void)
{
	NVIC_InitTypeDef NVIC_Structure;
	EXTI_InitTypeDef EXTI_Structure;

	LCD_Inform("Manual Module");
	
	EXTI_Structure.EXTI_Line = EXTI_Line0;
	EXTI_Structure.EXTI_LineCmd = ENABLE;
	EXTI_Structure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_Structure.EXTI_Trigger = EXTI_Trigger_Rising;
	
	EXTI_ClearITPendingBit(EXTI_Line0);

	EXTI_Init(&EXTI_Structure);																						    
	
	NVIC_Structure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Structure.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_Structure);
}

void Manual_Control(uint8_t data[])
{
	static int accv = 600;

	static int manual_Vx = 0;
	static int manual_Vy = 0;
	static int manual_W = 0;
	static float ratio = 30;
	static float ratiow = 0.08;
	
	static float ratiovx = 1;
	static float ratiovy = 1;
	
	static int speedY_Limit = 10000;
	static int speedX_Limit = 10000;
	
	static int i;
												  
//控制部分
	//慢速/快速切换

	if (ctrl_Mode == 1)
	{
		ratio = 17;
		ratiow = 0.04;
		
		accv = 200;
	}
	else if (ctrl_Mode == 0)
	{
		ratio = 17;
		ratiow = 0.03;

		accv = 600;
	}
	else
	{
		ratio = 3;
		ratiow = 0.01;

		accv = 150;
	}
						
	//前进/后退
	if (data[8]-0x7F <= stop_Area && data[8]-0x7F >= -stop_Area)
	{
		manual_Vy = 0;
	}
	else if (data[8] == 0xFF)
	{
		if (Press(JOYL))
		{
			if (manual_Vy < speedY_Limit && manual_Vy > -speedY_Limit)
			{
				manual_Vy += accv;
			}
		}
		else
		{
			manual_Vy = ratio * (data[8] - 0x7F);
		}

	}
	else if (data[8] == 0x00)
	{
		if (Press(JOYL))
		{
			if (manual_Vy < speedY_Limit && manual_Vy > -speedY_Limit)
			{
				manual_Vy -= accv;
			}
		}
		else
		{									   
			manual_Vy = ratio * (data[8] - 0x7F);
		}
	}
	else
	{ 
		if (Press(JOYL))
		{
			ratiovy =  ((float)data[8] - 0x7F) / (Abs(data[7] - 0x7F) + Abs(data[8] - 0x7F));

			if (manual_Vy < speedY_Limit && manual_Vy > -speedY_Limit)
			{
				manual_Vy += accv * ratiovy;
			}
		}
		else
		{
			manual_Vy = ratio * (data[8] - 0x7F);
		}
	}
	
	//左/右平移
	if (data[7]-0x7F <= stop_Area && data[7]-0x7F >= -stop_Area)
	{
		manual_Vx = 0;
	}
	else if (data[7] == 0xFF)
	{
		if (Press(JOYL))
		{
			if (manual_Vx < speedX_Limit && manual_Vx > -speedX_Limit)
			{
				manual_Vx -= accv;
			}
		}
		else
		{
			manual_Vx = -ratio * (data[7] - 0x7F);			 
		}
	}
	else if (data[7] == 0x00)
	{
		if (Press(JOYL))
		{
			if (manual_Vx < speedX_Limit && manual_Vx > -speedX_Limit)
			{
				manual_Vx += accv;
			}
		}
		else
		{
			manual_Vx = -ratio * (data[7] - 0x7F);
		}

	}
	else
	{
		if (Press(JOYL))
		{
			ratiovx =  ((float)data[7] - 0x7F) / (Abs(data[7] - 0x7F) + Abs(data[8] - 0x7F));
		
			if (manual_Vx < speedX_Limit && manual_Vx > -speedX_Limit)
			{
				manual_Vx -= accv * ratiovx;
			}
		}
		else
		{
			manual_Vx = -ratio * 0.6 * (data[7] - 0x7F);
		}
	}
	
	//自转 
	if (data[5]-0x7F <= stop_Area && data[5]-0x7F >= -stop_Area)
	{
		manual_W = 0;
	}
	else
	{
		manual_W = - ratiow * (data[5] - 0x7F);
	}															 

	//速度切换
	if (Press(SQR))
	{
		ctrl_Mode = 2;
	}

	if (Press(CRCL))
	{
		ctrl_Mode = 1;
	}

	if (Press(TRNGL))
	{
		ctrl_Mode = 0;
	}

	if (Press(L1))
	{
		ELVTR_Up();
	}
	else if (Press(L2))
	{
		ELVTR_Down();
	}
	else
	{
		ELVTR_Stop();
	}

	if (Press(R1))
	{
		Com_On();
	}
	else if (Press(R2))
	{
		Com_Off();
	}

	if (Press(DOWN))
	{
		TKN_Dispatch();
	}
	else if (Press(UP))
	{
		TKN_Patch();
	}

	if (Press(LEFT))
	{
		ARM_Down();
	}
	else if (Press(RGHT))
	{
		ARM_Up();
	}

	/*if (Press(UP))
	{
		manual_Vx = manual_Vy = manual_W = 0;

		ELVTR_Stop();
	}		   */

	Wheel_Ctrl(manual_Vx, manual_Vy, manual_W);  
}
 
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);

   	interrupted = 1;

	while(!Press(SLCT))
	{
		if (data_EN)
		{
			Manual_Control(data);
		}
		
		Read_Pos();

		if (ctrl_Mode == 1)
		{
			LCD_Write(6, 0, "D3");
		}
		else if (ctrl_Mode == 0)
		{
			LCD_Write(6, 0, "D2");
		}
		else
		{
			LCD_Write(6, 0, "D1");
		}
	}
	autoDisable = 0;
	ELVTR_Manual(ELVTR_DISABLE);							 
}

float Abs(float a)
{
	if (a > 0)
		return a;
	else
		return -a;											 
}