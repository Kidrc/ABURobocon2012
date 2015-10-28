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
			//1长度决定启动位置，y方向由激光确定。
		 	Goto_Pos(2817, -191, 0, 1);	    //取币
			//x坐标由3-1.5长度确定，若长度变长则缩小x坐标。y坐标由2长度确定，若长度变长则增大y坐标绝对值。
TAKETOKENFAIL:
			while(!Press(R2));
			Goto_Pos(2873, 2304, 90, 2);	//投币前拐点
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

			
			//x坐标不动，由激光确定。y方向由3.5长度确定，若长度变长则增大y坐标绝对值。
			Goto_Pos(3498, 4050, 90, 3);	//投币

			//while(!Press(R2));

			//x坐标由激光确定，隧道中央，不动。y坐标由4长度确定，方法同上。
			Goto_Pos(2806, 6860, 90, 4);	//隧道出口
		
		//抬C重启
		case 2:
			//xy坐标由反光镜确定，不动。
			Touch_Switch(TOUCH_ENABLE);
			Goto_Pos(1705, 7333, 90, 5);	//拿C
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

			//xy坐标由C确定，不动。
			Goto_Pos(400, 7354, 0, 6);		//下桥处拐点

			//x坐标不动。y坐标由5长度确定，方法同上。
			Goto_Pos(400, 9265, -120, 7);	//篮子处拐点

			//x坐标由6长度确定，长度越长x坐标越大。y坐标由7长度确定，方法同上。
			Goto_Pos(3104, 10369, -179, 8);	//A区旁拐点
		
			//x坐标手调，不动。y坐标由激光确定，不动。
			Goto_Pos(2202, 11337, -179, 9);	//放C

		//带篮子重启
		case 5:
		//不带篮子重启
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
			
			//x坐标不动。y坐标由8长度确定，长度越长y坐标绝对值越小。
			Goto_Pos(2578, 9900, -179, 10);	//A区旁拐点
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
				//y坐标由5+7长度确定，长度越长y绝对值越大。
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

			//已给足够的空间，调试再看
			Goto_Pos(200, 7453, -10, 12);	 //靠近下桥处的拐点
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 3299;
				crd_Y = 7344;
				crd_A = -90;
				revised_A = 1;
				goto BASKETFAIL;
			}
			
			Goto_Pos(2509, 7453, 0, 13);	 //隧道出口
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
			//y方向由隧道长度确定，长度越长绝对值越小。
			Goto_Pos(3299, 4477, -90, 14);	 //投币处
			if (interrupted)
			{
				interrupted = 0;
				crd_X = 3299;
				crd_Y = 7344;
				crd_A = -90;
				revised_A = 1;
				goto BASKETFAIL;
			}

			//y方向由2长度确定，长度越长绝对值越小。
			Goto_Pos(2743, 1183, -90, 15);	 //第一个障碍处
			ELVTR_Goto(LIFTBASKET2);
			Goto_Pos(-350, -400, -179, 16);	 //岛下
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
