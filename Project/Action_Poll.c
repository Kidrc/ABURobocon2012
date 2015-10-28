#include "Action_Poll.h"

int bkt_Enable = 1;

/*
*	��������Action_Init
*	���ܣ��Զ���ִ�г���ĳ�ʼ����������
*		PWM����ʼ���������H�ţ�
*		IO�ڳ�ʼ��������ִ�в��֣�
*		�����ص���ʼλ��
*	��������
*	����ֵ����
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
*	��������Elevator_Up
*	���ܣ��õ�������
*		�ú�����������Elevator_Downִ��
*	��������
*	����ֵ����
*/
void Elevator_Up(void)
{												   
	GPIO_SetBits(GPIOB, ELVTR_RUN);	//�ɿ�ɲ��
	GPIO_SetBits(GPIOB, ELVTR_DIR); //�������
}

/*
*	��������Elevator_Down
*	���ܣ��õ����½�
*		�ú���������Elevator_Downִ��
*	��������
*	����ֵ����
*/
void Elevator_Down(void)
{
	GPIO_SetBits(GPIOB, ELVTR_RUN);	 //�ɿ�ɲ��
	GPIO_ResetBits(GPIOB, ELVTR_DIR); //�������
}

/*
*	��������Elevator_Left
*	���ܣ��õ�������
*		H���ϣ���������ں������ʱ��ʵ������
*	��������
*	����ֵ����
*/
void Elevator_Left(void)
{
	GPIO_ResetBits(GPIOB, H_DIR1);	  //��������
	PWM_Generate(TIM1, PWM_CH1, 80);  //�������
}

/*
*	��������Elevator_Right
*	���ܣ��õ�������
*		H���ϣ���������ں������ʱ��ʵ������
*	��������
*	����ֵ����
*/
void Elevator_Right(void)
{
	GPIO_SetBits(GPIOB, H_DIR1);
	PWM_Generate(TIM1, PWM_CH1, 80);
}

/*
*	��������Elevator_Stop_Y
*	���ܣ��õ�������ֱ������ֹͣ
*	��������
*	����ֵ����
*/
void Elevator_Stop_Y(void)
{
	GPIO_ResetBits(GPIOB, ELVTR_RUN);  //ֹͣ���
	GPIO_SetBits(GPIOB, ELVTR_DIR);	   //����ɲ��
}

/*
*	��������Elevator_Stop_X
*	���ܣ��õ�����ˮƽ������ֹͣ
*	��������
*	����ֵ����
*/
void Elevator_Stop_X(void)
{
	PWM_Stop(TIM1, PWM_CH1);	//ֹͣPWM�����
}

/*
*	��������Elevator_Goto_X
*	���ܣ��õ���ˮƽ�˶���ָ��λ��
*		������������
*		��ʼλ�ñ�����������
*	������elevator_X x --ָ��λ��
*	����ֵ����
*/
void Elevator_Goto_X(elevator_X x)
{
	static elevator_X pos_X = ELVTR_X0;		  //��¼��ǰλ��
	static int enable = 0;	   //�Ƿ���Լ��㵱ǰλ�õ��ź�������ֹ��������Чʱ�����Σ�

	int captured = 0;	//�жϴ������Ƿ񴥷�

	while(pos_X != x)
	{
		captured = Sensor_Captured(SENSOR_X);	 //��ȡ��������ֵ
		
		if (enable && captured)	 //�ڴ�������Ч��ʱ��  
		{
			enable = 0;		 //�����ټ�����

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
		else if (!enable && !captured)	//�ڴ�������Ч��ʱ��
		{ 	
			enable = 1;	   //׼�����

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
	Elevator_Stop_X();	 //����ѭ�������ֹͣ
}

/*
*	��������Elevator_Goto_Y
*	���ܣ��õ�����ֱ�˶���ָ��λ��
*		ָ������������
*	������elevator_Y y -- ָ��λ��
*	����ֵ����
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
*	��������Basket_Patch
*	���ܣ�ץס����
*	��������
*	����ֵ����
*/
void Basket_Patch(void)
{
	PWM_Generate(TIM5, PWM_CH1, 5);

	Delay(2000);
}

/*
*	��������Basket_Dispatch
*	���ܣ��ſ�����
*	��������
*	����ֵ����
*/
void Basket_Dispatch(void)
{
	PWM_Generate(TIM5, PWM_CH1, 11);
	
	Delay(2000);
}

/*
*	��������Basket_Up
*	���ܣ����������ӵĻ�е��
*		 �������Ѿ������ʱ��ʹ�ã����˵���͸�˿
*		 ������Basket_Down֮ǰʹ��
*	��������
*	����ֵ����
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
*	��������Basket_Down
*	���ܣ����������ӵĻ�е��
*		�������Ѿ����µ�ʱ��ʹ�ã�����ɲ���Ԥ֪�ĺ��
*		������Basket_Up֮��ʹ��
*	��������
*	����ֵ����
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

/*	��������Basket_Stop
*	���ܣ�ʹ�����ӵĻ�е��ֹͣ
*	��������
*	����ֵ����
*/
void Basket_Stop(void)
{
	PWM_Generate(TIM1, PWM_CH2, 0);	

	GPIO_SetBits(GPIOB, BASKET_BRK);
}	

/*
*	��������Basket_Scan
*	���ܣ�������ӣ�����������ס����������
*		��ѯʱʹ��
*	��������
*	����ֵ����
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
*	��������Sensor_Captured
*	���ܣ��ж�ָ���������Ƿ���Ч
*	������action_Sensor sensor -- ָ��������
*	����ֵ��uint8_t����
		1 -- ��Ч
		0 -- ��Ч
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
