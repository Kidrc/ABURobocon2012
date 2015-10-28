#include "stm32f10x.h"
#include "GeneralDef_CM3.h"
#include "PSX.h"
#include "PSXCtrl.h"
#include "Action.h"
#include "Encoder.h"
#include "MTI.h"
#include "SetSpeed.h"
#include "Navigate.h"
#include "IL.h"

char stage = 0;

int main(void)
{
	int i;
	LCD_Init();
							  
   	MTI_Init();
	Encoder_Init();	
	IL_Init();

	USART3_Init(115200);
 	Action_Init();


	PSX_Init(100); 
	Clock_Init();
 	PSXCtrl_Init();
	ELVTR_Goto(LOADC1);

	stage = GetReady();

	Clock_Start();
	switch(stage)
	{
		case 1:
			//1���Ⱦ�������λ�ã�y�����ɼ���ȷ����
		 	Goto_Pos(2817, -191, 0, 1);	    //ȡ��
			//x������3-1.5����ȷ���������ȱ䳤����Сx���ꡣy������2����ȷ���������ȱ䳤������y�������ֵ��
TAKETOKENFAIL:
			while(!Press(R2));
			Goto_Pos(2873, 2304, 90, 2);	//Ͷ��ǰ�յ�
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 2817;
				crd_Y = -191;
				crd_A = 0;
				revised_A = 1;

				TKN_Patch();
				goto TAKETOKENFAIL;
			}

			
			//x���겻�����ɼ���ȷ����y������3.5����ȷ���������ȱ䳤������y�������ֵ��
			Goto_Pos(3498, 4050, 90, 3);	//Ͷ��

			//while(!Press(R2));

			//x�����ɼ���ȷ����������룬������y������4����ȷ��������ͬ�ϡ�
			Goto_Pos(2806, 6860, 90, 4);	//�������
		
		//̧C����
		case 2:
			//xy�����ɷ��⾵ȷ����������
			Touch_Switch(TOUCH_ENABLE);
			Goto_Pos(1705, 7333, 90, 5);	//��C
			if (Press(CRCL))
			{
				Adjust(1);
			}
			Touch_Switch(TOUCH_DISABLE);
			Com_On();
			ELVTR_Goto(LIFTC1);
			while(ELVTR_Pos() <= 3)
			{
				//Wheel_Ctrl(0, -1000, 0);
				if (ELVTR_Pos() >= 2)
				{
					Wheel_Ctrl(0, -1000, 0);
				}
			}

			//xy������Cȷ����������
			Goto_Pos(400, 7354, 0, 6);		//���Ŵ��յ�

			//x���겻����y������5����ȷ��������ͬ�ϡ�
			Goto_Pos(400, 9265, -120, 7);	//���Ӵ��յ�

			//x������6����ȷ��������Խ��x����Խ��y������7����ȷ��������ͬ�ϡ�
			Goto_Pos(3104, 10369, -179, 8);	//A���Թյ�
		
			//x�����ֵ���������y�����ɼ���ȷ����������
			Goto_Pos(2202, 11337, -179, 9);	//��C

		//����������
		case 5:
		//������������
		case 7:
			Adjust(2);

			if (!retry_WithBasket)
			{
				ELVTR_Goto(UNLOADC);
				while(!ELVTR_Arrived())
				{
					if (Press(R1))
					{
						break;
					}
				}
			}
			
			//x���겻����y������8����ȷ��������Խ��y�������ֵԽС��
			Goto_Pos(2578, 9900, -179, 10);	//A���Թյ�
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 600 + 500;
				crd_Y = 10094;
				crd_A = -179;
				revised_A = 1;
				goto POSFAIL;
			}

			if (!retry_WithBasket)
			{
				ARM_Down();
				ELVTR_Goto(LOADBASKET);
			}
			if (!retry_WithBasket)
			{
				//y������5+7����ȷ��������Խ��y����ֵԽ��
				Goto_Pos(600, 10094, -179, 11);
			}
			else
			{
				Goto_Pos(600, 9900, -179, 11);
			}
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 600 + 500;
				crd_Y = 10094;
				crd_A = -179;
				revised_A = 1;
			}
POSFAIL:
			Com_Off();
			ELVTR_Goto(LIFTBASKET1);
			LCD_Write(0, 0, "LOADED");

			//�Ѹ��㹻�Ŀռ䣬�����ٿ�
			Goto_Pos(200, 7453, -10, 12);	 //�������Ŵ��Ĺյ�
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 3299;
				crd_Y = 7344;
				crd_A = -90;
				revised_A = 1;
				goto BASKETFAIL;
			}
			
			Goto_Pos(2509, 7453, 0, 13);	 //�������
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 3299;
				crd_Y = 7344;
				crd_A = -90;
				revised_A = 1;
				goto BASKETFAIL;
			}
			Goto_Pos(3299, 7324, -90, 23);
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 3299;
				crd_Y = 7344;
				crd_A = -90;
				revised_A = 1;
				goto BASKETFAIL;
			}
	  case 6:
BASKETFAIL:
			//y�������������ȷ��������Խ������ֵԽС��
			Goto_Pos(3299, 4477, -90, 14);	 //Ͷ�Ҵ�
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 3299;
				crd_Y = 7344;
				crd_A = -90;
				revised_A = 1;
				goto BASKETFAIL;
			}

			//y������2����ȷ��������Խ������ֵԽС��
			Goto_Pos(2743, 1183, -90, 15);	 //��һ���ϰ���
			ELVTR_Goto(LIFTBASKET2);
			Goto_Pos(-350, -400, -179, 16);	 //����
UNLOADFAIL:
			ELVTR_Goto(LIFTBASKET2);

			Adjust(4);
			Clock_Start();
			ELVTR_Goto(LOADC2);
			while(ELVTR_Pos() >= 13);
			Goto_Pos(551, 500, -179, 17);
			if (interrupted)
			{
				interrupted = 0;
				goto UNLOADFAIL;
			}
		case 3:
			ARM_Up();
			TKN_Patch();

			Goto_Pos(80, 1181, -179, 18);
LOADFAIL:
			Touch_Switch(TOUCH_ENABLE);
			Adjust(5);
			Touch_Switch(TOUCH_DISABLE);
			ELVTR_Goto(LIFTC2);
			while(ELVTR_Pos() < 10);	 
			Goto_Pos(145, 1181, -179, 19);
			if (interrupted)
			{
				interrupted = 0;
				goto LOADFAIL;
			}
			Adjust(6);
			Com_Off();
			//for (i = 0; i <= 100000; i++);
			for (i = 0; i < 100; i++)
			{
				Wheel_Ctrl(0, 0, 0);
			}
			ELVTR_Goto(LIFTBUN);

			while(!ELVTR_Arrived());
			Adjust(6);
			Goto_Pos(906, 1101, -147, 20);
			Adjust(7);
			Com_On();
		break;

		default:
		break;
	}

	Clock_Stop(); 

	while (1)
	{
	   Read_Pos();
	} 
}
