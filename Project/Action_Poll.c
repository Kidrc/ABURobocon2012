#include "Action_Poll.h"

int bkt_Enable = 1;

/*
*	函数名：Action_Init
*	功能：对动作执行程序的初始化，包括：
*		PWM波初始化（舵机和H桥）
*		IO口初始化（动作执行部分）
*		机构回到初始位置
*	参数：无
*	返回值：无
*/	
void Action_Init(void)
{
	LCD_Clear();
	LCD_Write(0, 0, "Action");

	PWM_Init(TIM5, PWM_CH1, 50);
	PWM_Init(TIM1, PWM_CH1|PWM_CH2, 1000);

	RCC_GPIOB();
	RCC_GPIOC();
						  
	GPIO_Config(GPIO_Pin_All, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
	GPIO_Config(S_BASKET, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOB);
	GPIO_Config(S_X|S_Y1|S_Y2|S_Y3|S_Y4|S_TOKEN|S_ARM, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz, GPIOC);

	Token_Dispatch();																	     
							 
	Basket_Dispatch();
	//Basket_Patch();
/*	*/
	//Basket_Up();
	
	Elevator_Goto_Y(ELVTR_Y1);
	
	/*						  
	Token_Down();							   
	Token_Dispatch();	  	 
							*/
}

/*
*	函数名：Elevator_Up
*	功能：让电梯上升
*		该函数必须先于Elevator_Down执行
*	参数：无
*	返回值：无
*/
void Elevator_Up(void)
{												   
	GPIO_SetBits(GPIOB, ELVTR_RUN);	//松开刹车
	GPIO_SetBits(GPIOB, ELVTR_DIR); //启动电机
}

/*
*	函数名：Elevator_Down
*	功能：让电梯下降
*		该函数必须在Elevator_Down执行
*	参数：无
*	返回值：无
*/
void Elevator_Down(void)
{
	GPIO_SetBits(GPIOB, ELVTR_RUN);	 //松开刹车
	GPIO_ResetBits(GPIOB, ELVTR_DIR); //启动电机
}

/*
*	函数名：Elevator_Left
*	功能：让电梯左移
*		H桥上，绕组红线在黑线左边时，实现左移
*	参数：无
*	返回值：无
*/
void Elevator_Left(void)
{
	GPIO_ResetBits(GPIOB, H_DIR1);	  //方向向左
	PWM_Generate(TIM1, PWM_CH1, 80);  //启动电机
}

/*
*	函数名：Elevator_Right
*	功能：让电梯右移
*		H桥上，绕组红线在黑线左边时，实现右移
*	参数：无
*	返回值：无
*/
void Elevator_Right(void)
{
	GPIO_SetBits(GPIOB, H_DIR1);
	PWM_Generate(TIM1, PWM_CH1, 80);
}

/*
*	函数名：Elevator_Stop_Y
*	功能：让电梯在竖直方向上停止
*	参数：无
*	返回值：无
*/
void Elevator_Stop_Y(void)
{
	GPIO_ResetBits(GPIOB, ELVTR_RUN);  //停止电机
	GPIO_SetBits(GPIOB, ELVTR_DIR);	   //启动刹车
}

/*
*	函数名：Elevator_Stop_X
*	功能：让电梯在水平方向上停止
*	参数：无
*	返回值：无
*/
void Elevator_Stop_X(void)
{
	PWM_Stop(TIM1, PWM_CH1);	//停止PWM波输出
}

/*
*	函数名：Elevator_Goto_X
*	功能：让电梯水平运动到指定位置
*		传感器触发处
*		初始位置必须在最右面
*	参数：elevator_X x --指定位置
*	返回值：无
*/
void Elevator_Goto_X(elevator_X x)
{
	static elevator_X pos_X = ELVTR_X0;		  //记录当前位置
	static int enable = 0;	   //是否可以计算当前位置的信号量（防止传感器有效时计算多次）

	int captured = 0;	//判断传感器是否触发

	while(pos_X != x)
	{
		captured = Sensor_Captured(SENSOR_X);	 //获取传感器的值
		
		if (enable && captured)	 //在传感器有效的时候  
		{
			enable = 0;		 //不能再计算了

			if (pos_X > x)	  
			{											  
				Elevator_Right();
				
				--pos_X;
			}
			else if (pos_X < x)
			{
				Elevator_Left();
				
				++pos_X;
			}
			
		}
		else if (!enable && !captured)	//在传感器无效的时候
		{ 	
			enable = 1;	   //准许计算

			if (pos_X > x)
			{
				Elevator_Right();
			}
			else if (pos_X < x)
			{
				Elevator_Left();
			}
			
		}
		else
		{
			if (pos_X > x)
			{
				Elevator_Right();
			}
			else if (pos_X < x)
			{
				Elevator_Left();
			}	
		}				
	}
	Elevator_Stop_X();	 //跳出循环后电梯停止
}

/*
*	函数名：Elevator_Goto_Y
*	功能：让电梯竖直运动到指定位置
*		指定传感器触发
*	参数：elevator_Y y -- 指定位置
*	返回值：无
*/
void Elevator_Goto_Y(elevator_Y y)
{
	static elevator_Y pos_Y = ELVTR_Y0;

   	LED_On(LED1);
	while(pos_Y != y)
	{
		if (Sensor_Captured(SENSOR_Y1))
		{
			pos_Y = ELVTR_Y1;	
		}
		else if (Sensor_Captured(SENSOR_Y2))
		{
			pos_Y = ELVTR_Y2;	
		}
		else if (Sensor_Captured(SENSOR_Y3))
		{
			pos_Y = ELVTR_Y3;	
		}
		/*else if (Sensor_Captured(SENSOR_Y4))
		{
			pos_Y = ELVTR_Y4;	
		}*/

		if (pos_Y < y)
		{
			Elevator_Up();
		}
		else if (pos_Y > y)
		{
			Elevator_Down();
		}

	/*	
		switch(y)
		{
			case ELVTR_Y1:
				sensor = SENSOR_Y1;
			break;
			
			case ELVTR_Y2:
				sensor = SENSOR_Y2;
			break;
			
			case ELVTR_Y3:
				sensor = SENSOR_Y3;
			break;
			
			case ELVTR_Y4:
				sensor = SENSOR_Y4;
			break;
			
			default:
				sensor = SENSOR_Y1;
			break;
		}
		if (Sensor_Captured(sensor))
		{
			Elevator_Stop_Y();
			
			pos_Y = y;
		}
		*/
	}
	Elevator_Stop_Y();
	LED_Off(LED1);	
}

/*
*	函数名：Basket_Patch
*	功能：抓住篮子
*	参数：无
*	返回值：无
*/
void Basket_Patch(void)
{
	PWM_Generate(TIM5, PWM_CH1, 5);

	Delay(2000);
}

/*
*	函数名：Basket_Dispatch
*	功能：放开篮子
*	参数：无
*	返回值：无
*/
void Basket_Dispatch(void)
{
	PWM_Generate(TIM5, PWM_CH1, 11);
	
	Delay(2000);
}

/*
*	函数名：Basket_Up
*	功能：收起拿篮子的机械臂
*		 不能在已经收起的时候使用，会伤电机和钢丝
*		 必须在Basket_Down之前使用
*	参数：无
*	返回值：无
*/
void Basket_Up(void)
{
	GPIO_ResetBits(GPIOB, BASKET_BRK);
	GPIO_ResetBits(GPIOB, H_DIR2);

	PWM_Generate(TIM1, PWM_CH2, 80);
	 
	while (!Sensor_Captured(SENSOR_ARM))
	{
	}

	Basket_Stop();
}

/*												   
*	函数名：Basket_Down
*	功能：放下拿篮子的机械臂
*		不能在已经放下的时候使用，会造成不可预知的后果
*		必须在Basket_Up之后使用
*	参数：无
*	返回值：无
*/
void Basket_Down(void)
{
	GPIO_ResetBits(GPIOB, BASKET_BRK);
	GPIO_SetBits(GPIOB, H_DIR2);

	PWM_Generate(TIM1, PWM_CH2, 30);

	Delay(3500);

	Basket_Stop();

	bkt_Enable = 1;
}

/*	函数名：Basket_Stop
*	功能：使拿篮子的机械臂停止
*	参数：无
*	返回值：无
*/
void Basket_Stop(void)
{
	PWM_Generate(TIM1, PWM_CH2, 0);	

	GPIO_SetBits(GPIOB, BASKET_BRK);
}	

/*
*	函数名：Basket_Scan
*	功能：检测篮子，若发现则拿住并提升电梯
*		轮询时使用
*	参数：无
*	返回值：无
*/
void Basket_Scan(void)
{
	if (bkt_Enable && Sensor_Captured(SENSOR_BASKET))
	{
		LCD_Clear();
		LCD_Write(0, 0, "Loading Basket");

		Basket_Patch();

		Elevator_Goto_Y(ELVTR_Y3);

		bkt_Enable = 0;

		LCD_Clear();
		LCD_Write(0, 0, "Basket Loaded.");
	}
}

/*
*	函数名：Sensor_Captured
*	功能：判断指定传感器是否有效
*	参数：action_Sensor sensor -- 指定传感器
*	返回值：uint8_t类型
		1 -- 有效
		0 -- 无效
*/

void Token_Scan(void)
{
	static uint8_t scanned = 0;
		
	if (!scanned && Sensor_Captured(SENSOR_TOKEN))		  
	{
		Token_Patch();

		scanned = 1;
	}
	
	if (scanned && !Sensor_Captured(SENSOR_TOKEN))
	{
		Token_Patch();
		
		scanned = 1;
	}					 

}
uint8_t Sensor_Captured(action_Sensor sensor)
{
	uint8_t filter = 0;

	switch(sensor)
	{
		case SENSOR_X:
			filter = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
			if (filter)
			{
				Delay(10);

				return !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
			}
			else
				return 0;
		break;
		
		case SENSOR_Y1:
			filter = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
			if (filter)
			{
				Delay(10);

				return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
			}
			else
				return 0;

		break;

		case SENSOR_Y2:
			filter = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
			if (filter)
			{
				Delay(10);

				return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
			}
			else
				return 0;
		break;

		case SENSOR_Y3:
			filter = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
			if (filter)
			{
				Delay(10);

				return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
			}
			else
				return 0;
		break;

		case SENSOR_Y4:
			filter = GPIO_ReadInputDataBit(GPIOC, S_Y4);
			if (filter)
			{
				Delay(10);

				return GPIO_ReadInputDataBit(GPIOC, S_Y4);
			}
			else
				return 0;
		break;

		case SENSOR_BASKET:
			filter = GPIO_ReadInputDataBit(GPIOB, S_BASKET);
			if (filter)
			{
				Delay(10);

				return GPIO_ReadInputDataBit(GPIOB, S_BASKET);
			}
			else
				return 0;
		break;

		case SENSOR_TOKEN:
			filter = GPIO_ReadInputDataBit(GPIOC, S_TOKEN);
			if (filter)
			{
				Delay(10);

				return GPIO_ReadInputDataBit(GPIOC, S_TOKEN);
			}
			else
				return 0;
		break;
		
		case SENSOR_ARM:
			filter = GPIO_ReadInputDataBit(GPIOC, S_ARM);
			if (filter)
			{
				Delay(10);

				//LED_On(LED1);
				return GPIO_ReadInputDataBit(GPIOC, S_ARM);
			}
			else
				return 0;
		break;
				
		default:
			return 0;
		break;
	}
}
