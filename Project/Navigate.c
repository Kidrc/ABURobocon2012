#include "Navigate.h"

/*
	蓝场：crd_X正向从起点到夹币；
			crd_Y正向从起点到L1；
			crd_A正向逆时针。
		vX正向与crd_X相反；
		vY正向与crd_Y相反；
		W正向逆时针。
*/

/*全为负值*/
const double DIS_TAKETKN = -161;
const double DIS_PUTTKN = -110;
const double DIS_TUNNEL = -441;
const double DIS_PUTC = -73;
const double DIS_LOADC = -37;

extern char data_EN;
volatile char interrupted = 0;
char round = 0;
char retry_WithBasket = 0;

char touch_EN = 1;

double crd_X = 0;
double crd_Y = 0;
double crd_A = 0;
//double error_A = 0;

double tar_X = 0;
double tar_Y = 0;
double tar_A = 0;

double last_X = 0;
double last_Y = 0;
double last_A = 0;

char cur_Part = 0;

volatile char revised_A = 0;

char il_EN = 0;

char GetReady(void)
{
	char stage = 0;
	char sensor_State = 0;

	while(stage == 0)
	{
		sensor_State = GV1_Active() | Basket_Active() << 1 | Reflect_Active() << 2 | Touched() << 3 | GV2_Active() << 4;
		LCD_Clear();

		LCD_Write_Int(0, 0, Read_IL());
		LCD_Write_Int(6, 0, sensor_State);
		LCD_Write_Int(0, 1, MTI_GetAngle().yaw);
		LCD_Write_Int(6, 1, Read_AbsEncoder());
		LCD_Write_Int(0, 2, Read_Encoder(1));
		LCD_Write_Int(0, 3, Read_Encoder(2));
	    LCD_Write(6, 3, "Blue");

		if (Press(STRT))
		{
			stage = 1;
		}
		else if (Press(L1))
		{
			stage = 2;
		}
		else if (Press(L2))
		{
			stage = 3;
		}
		else if (Press(JOYL))
		{
			stage = 6;
		}
		else if (Press(JOYR))
		{
			stage = 5;
		}
		else if (Press(LEFT))
		{
			stage = 7;
		}
	}
	MTI_UpdateInitAngle();
	Update_Encoder(1);
	Update_Encoder(2);

	switch(stage)
	{
		//启动
		case 1:
			crd_X = 0;
			crd_Y = 0;
			crd_A = 0;
		
			ARM_Out();
			TKN_Dispatch();
			ARM_Up();
		break;

		//抬C重启，y坐标由4阶段|tar_Y| + 300确定。
		case 2:
			crd_X = 2976;
			crd_Y = 7190;
			crd_A = 90;
			
			ARM_In();
			TKN_Dispatch();
			ARM_Up(); 
		break;

		case 3:
			crd_X = 100;
			crd_Y = 1181;
			crd_A = -179;

			
			ARM_In();
			TKN_Dispatch();
			ARM_Up();

			ELVTR_Goto(LOADC2);
		break;

		case 5:
			tar_X = crd_X = 2502;
			tar_Y = crd_Y = 11337;
			tar_A = crd_A = -179;

			ARM_Down();
			ELVTR_Goto(LIFTBASKET1);
			retry_WithBasket = 1;
		break;

		case 6:
			crd_X = 3299;
			crd_Y = 7344;
			crd_A = -90;

			ARM_Down();
			TKN_Dispatch();
			ARM_In();

			ELVTR_Goto(LIFTBASKET1);

			while(!Press(R2));
		break;

		case 7:
			tar_X = crd_X = 2202;
			tar_Y = crd_Y = 11337;
			tar_A = crd_A = -179;

			retry_WithBasket = 0;
		break;

		default:
		break;
	}
	revised_A = 1;

	return stage;
}

void Adjust(char part)
{
	static int accv = 600;				  

	static int manual_Vx = 0;
	static int manual_Vy = 0;
	static int manual_W = 0;
	static float ratio = 30;
	static float ratiow = 0.08;
	
	static float ratiovx = 1;
	static float ratiovy = 1;
	
	static int speedY_Limit = 7500;
	static int speedX_Limit = 7500;
	
	LCD_Write(0, 0, "Adjust");

	do
	{
		if (ELVTR_Arrived())
		{
			ELVTR_Manual(ELVTR_ENABLE);
		}
		if (part == 6)
		{
			if (Press(R1))
			{
				Com_On();
			}
		}
		Delay(100);
		Read_Pos();
															  
	//控制部分
		//慢速/快速切换
		 if (data_EN)
		 {
			switch(part)
			{
				case 1:
					ratio = 10;
					ratiow = 0.01;
					
					accv = 200;
				break;
	
				case 2:
					ratio = 7;
					ratiow = 0.01;
					
					accv = 40;
				break;
	
				case 3:
					ratio = 10;
					ratiow = 0.02;
			
					accv = 600;
				break;
	
				case 4:
					ratio = 14;
					ratiow = 0.01;
					
					accv = 60;
				break;
	
				case 5:
					ratio = 13;
					ratiow = 0.01;
				break;

				case 6:
					ratio = 6;
					ratiow = 0.01;
					accv = 30;
				break;

				case 7:
					ratio = 5;
					ratiow = 0.01;
				break;

				default:
					ratio = 10;
					ratiow = 0.02;
			
					accv = 200;
				break;
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
		
			Wheel_Ctrl(manual_Vx, manual_Vy, manual_W);
			
			if (ELVTR_IsManual())
			{
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
			} 
		}
	}
	while(!Press(R2) && !Touched() && !interrupted);

	switch(part)
	{
		case 1:
			Wheel_Ctrl(0, 0, 0);
			crd_X = tar_X;
			crd_Y = tar_Y + 100;
			//crd_A = tar_A;
			//revised_A = 1;
		break;

		case 2:
			crd_X = tar_X;
			crd_Y = tar_Y;
			crd_A = tar_A;

			revised_A = 1;
		break;

		case 4:
			crd_X = crd_Y = 0;
			if (Delta_Angle(179, crd_A) >= 15 || Delta_Angle(179, crd_A) <= -15)
			{
				crd_A = -179;
				revised_A = 1;
			}
			Update_Encoder(1);
			Update_Encoder(2);
		break;

		case 5:
			crd_X = 0;
			crd_Y = tar_Y;
			crd_A = tar_A;
			revised_A = 1;
		break;

		case 6:
			crd_A = tar_A;
			revised_A = 1;
		break;

		case 18:
		case 19:
		break;

		default:
		break;
	}
	ELVTR_Manual(ELVTR_DISABLE);
	LCD_Clear();
}
void Goto_Pos(int x, int y, int a, char part)
{
	//三个PID函数应该有更好的方法去整合到一起。
	double PID_X(double x, char part);
	double PID_Y(double y, char part);
	double PID_W(double a, char part);
	
	//直线模型的全场速度设定
	void Wheel_Ctrl_Global(double vX, double vY, double w, double crd_A);
	
	static double vX = 0;
	static double vY = 0;
	static double w = 0;
	char arrived = 0;
	char gv2 = 0;
	//这里应该有更优的方法去判断到达条件。目前的方法会在每一次循环都浪费掉一些时间去检查“是什么判定条件”。
	tar_X = x;
	tar_Y = y;
	tar_A = a;
	cur_Part = part;

	while(!arrived && !interrupted)
	{
		Delay(50);
			
		Read_Pos();
		Revise_Pos(part);
		
		vX = PID_X(x, part);
		vY = PID_Y(y, part);
		w = PID_W(a, part);
		
		Wheel_Ctrl_Global(vX, vY, w, crd_A);
		
		switch(part)
		{
			case 1:
				arrived = Press(R1);
			break;
			
			case 2:
				arrived = crd_Y >= y;
			break;

			case 31:
				arrived = crd_Y >= y;
			break;

			case 3:
				arrived =Press(R1);	
			break;
 						   
			case 4:
				arrived = crd_Y >= y;
			break;

			case 5:
				arrived = Touched() || Press(CRCL);
				//arrived = Press(R1);
			break;

			case 6:
				arrived = crd_X <= x;
			break;
			
			case 7:
				arrived = crd_Y >= y;
			break;

			case 8:
				arrived = crd_X >= x;
			break;

			case 9:
				arrived = Press(R1);
			break;
							     
			case 10:
				arrived = crd_Y <= y;
			break;

		   	case 11:
				//arrived = Press(R1);
				arrived = (!retry_WithBasket && Basket_Active()) || (retry_WithBasket && Press(R1));
			break;

			case 12:
				arrived = crd_Y <= y;
			break;

			case 13:
				//arrived = GV2_Active();
				arrived = (Read_IL() != 0 && Read_IL() > -800 && crd_A > -20) || Press(R1);
				//arrived = Press(R1);
			break;

			case 23:
				arrived = crd_A <= -80;
			break;

			case 14:
				arrived = crd_Y <= y;
			break;

			case 15:
				arrived = crd_Y <= y;
			break;

			case 16:
				arrived = Press(R1);
			break;

		   	case 17:
				arrived = crd_X >= x;
			break;

			case 18:
				arrived = Press(R1);
			break;

			case 19:
				//arrived = ELVTR_Arrived();
				arrived = crd_X > tar_X - 10;
			break;

			case 20:
				arrived = Press(R1);
			break;

			default:
			break;
		}
  	}

   	if (!interrupted)
	{
		switch(part)
		{	
			case 1:
				Wheel_Ctrl(0, 0, 0);
				crd_Y = y;
	
				TKN_Patch();			
			break;
	
			case 2:
			break;
	
	
			case 3:
				crd_X = x;			
				//crd_Y = y;
	
				Wheel_Ctrl(0, 0, 0);
	
				TKN_Dispatch();
				Delay(1000);
				ARM_In();
	
			break;
	
			case 4:
			break;
	
			case 5:
				crd_X = tar_X;
				crd_Y = tar_Y + 100;
				//crd_A = tar_A;
				//revised_A = 1;
				//Wheel_Ctrl(0, 0, 0);
			break;
	
			case 7:
				last_X = x;
			break;
	
			case 9:
				last_X = x;
				last_Y = y;
				last_A = a;
			break;
	
			case 11:
				crd_X = tar_X + 500;
				crd_Y = tar_Y;
			break;
	
			case 12:
				//crd_Y = y;
			break;
	
			case 13:
				crd_X = x;
				crd_Y = y;
			break;
	
			case 14:
				last_X = x;
				last_Y = y;
	
				crd_X = x;
			break;
	
			case 15:
				last_Y = y;
			break;
	
			case 19:
				Wheel_Ctrl(0, 0, 0);
			break;

			default:
			break;
		}
	}
}

void Read_Pos(void)
{
	const double LY = 200;
	const double LX = 155;

	static double dx = 0;
	static double dy = 0;
	static double da = 0;
	static double error_A = 0;
	static double temp_A = 0;

	static double c1 = 0;
	static double c2 = 0;

	static double last_crdA = 0;

 	temp_A = MTI_GetAngle().yaw;

	if (revised_A)
	{
		error_A = Delta_Angle(crd_A, temp_A);
			
		revised_A = 0;
	}

	last_crdA = crd_A;

	crd_A = temp_A + error_A;

	if (crd_A >= 180)
	{
		crd_A -= 360;
	}
	else if (crd_A < -180)
	{
		crd_A += 360;												  
	}	

	da = crd_A - last_crdA;

	if (da >= 180)
	{
		da -= 360;
	}
	else if (da < -180)
	{
		da += 360;												  
	}	
	da = da*PI / 180;

	dx = Read_Encoder(2) - da * LY;
	dy = Read_Encoder(1) - da * LX;
			   	
	c1 = cos(crd_A * PI/180);
	c2 = sin(crd_A * PI/180);

	crd_X += dx * c2 + dy * c1;
	crd_Y += -dx * c1 + dy * c2;
}

void Revise_Pos(char part)
{
	static char lastID = 0;
    static int il = 0;

	il = Read_IL();

	if (il != 0)
	{
		switch(part)
		{
			//夹币
			case 1:
				crd_Y = -il + tar_Y-(-DIS_TAKETKN) + 30;
			break;
	
			//投币
			case 3:
				if (il < -70)
				{
					crd_X = il + tar_X-DIS_PUTTKN + 30;
				}
			break;

			/*进隧道前的一小段，隧道中央*/	
			case 4:
				if (il_EN)
				{
					crd_X = il + tar_X-DIS_TUNNEL + 30;
				}
			break;
			
			case 5:
				//-756;
			break;

			case 7:
				/*if (crd_A > 89)
				{
					crd_X = -il + tar_X-(830);
				}*/
			break;

			//放C
			case 9:
				crd_Y = il + tar_Y-DIS_PUTC;
			break;

			//-11337替换为9阶段目标Y
			case 10: 
				if (il_EN)
				{
					crd_Y = il + (11337)-DIS_PUTC;
				}
			break;

			//
			case 11:
				/*if (!retry_WithBasket)
				{
					crd_Y = il + (10094) -(520);
				}
				else
				{
					crd_Y = il + (9900) - (-58);
				}*/
			break;

		   	case 13:
				/*if (crd_A < 10 && crd_A > -10)
				{
					crd_Y = il + tar_Y -(-828);
				}*/
			break;

			//出隧道，若不够则往大了改
			case 15:
				crd_X = -il + tar_X-(820);
			break;

			//抬C
			case 18:
				crd_Y = il + tar_Y -DIS_LOADC;
			break;

			//抓包子
			case 19:
				crd_Y = il + tar_Y -DIS_LOADC;
			break;
			
			//放
			case 20:
				//crd_Y = il + tar_Y - (-60);
			break;
			 
			default:
			break;
		}
	}
	if (lastID != dataID)
	{
		if (Press(UP))
		{
			if (crd_A > -10 && crd_A < 10)
			{
				crd_X -= 20;
			}
			else if (crd_A > -100 && crd_A < -80)
			{
				crd_Y += 20;
			}
			else if (crd_A > 170 || crd_A < -170)
			{
				crd_X += 20;
			}
			else if (crd_A > 80 && crd_A < 100)
			{
				crd_Y -= 20;
			}
		}
	
		if (Press(DOWN))
		{
			if (crd_A > -10 && crd_A < 10)
			{
				crd_X += 20;
			}
			else if (crd_A > -100 && crd_A < -80)
			{
				crd_Y -= 20;
			}
			else if (crd_A > 170 || crd_A < -170)
			{
				crd_X -= 20;
			}
			else if (crd_A > 80 && crd_A < 100)
			{
				crd_Y += 20;
			}
		}
		if (Press(LEFT))
		{
			if (crd_A > -10 && crd_A < 10)
			{
				if (part != 1)
				{
					crd_Y -= 20;
				}
				else
				{
					tar_Y -= 10;
				}
			}
			else if (crd_A > -100 && crd_A < -80)
			{
				crd_X -= 20;
			}
			else if (crd_A > 170 || crd_A < -170)
			{
				crd_Y += 20;
			}
			else if (crd_A > 80 && crd_A < 100)
			{
				if (part != 5)
				{
					if (part == 3)
					{
						tar_X += 10;
					}
					else
					{
						crd_X += 20;
					}
				}
				else
				{	
					crd_X += 10;
				}
			}
		}
		if (Press(RGHT))
		{
			if (crd_A > -10 && crd_A < 10)
			{
				if (part != 1)
				{
					crd_Y += 20;
				}
				else
				{
					tar_Y += 10;
				}
			}
			else if (crd_A > -100 && crd_A < -80)
			{
				crd_X += 20;
			}
			else if (crd_A > 170 || crd_A < -170)
			{
				crd_Y -= 20;
			}
			else if (crd_A > 80 && crd_A < 100)
			{
				if (part != 5)
				{
					if (part == 3)
					{
						tar_X -= 10;
					}
					else
					{
						crd_X -= 20;
					}
				}
				else
				{
					crd_X -= 10;
				}
			}
		}
	
		if (crd_A >= 180)
		{
			crd_A -= 360;
		}
		else if (crd_A < -180)
		{
			crd_A += 360;												  
		}
	}

	lastID = dataID;
}

double PID_X(double x, char part)
{
	static double pX = 16;
	static double iX = 0;
	static double lastError = 0;
	static double curError = 0;
	static double vX_Limit = 8000;
	static double vX = 0;
	
	static double eI = 0;

	static char reflected = 0;

	switch(part)
	{
		case 1:
			pX = 10;
			vX_Limit = 7500;
		break;

		case 3:
			pX = 10;
		break;

		case 4:
			pX = 10;
			vX_Limit = 4000;
		break;

		case 5:
			pX = 9;
			vX_Limit = 5000;
		break;

		case 6:
			pX = 12;
			vX_Limit = 4000;
		break;

		case 7:
			pX = 20;
			vX_Limit = 6000;
		break;

		case 8:
			pX = 12;
			vX_Limit = 6000;
		break;
	    
		case 9:
			pX = 7;
			vX_Limit = 2500;
		break;

		case 10:
			pX = 8;
			vX_Limit = 6000;
		break;

		case 11:
			pX = 10;	
			vX_Limit = 3700;
		break;

		case 12:
			pX = 12;
			vX_Limit = 4000;
		break;

		case 13:
			pX = 6;
			vX_Limit = 6000;
		break;

		case 23:
			pX = 7;
			vX_Limit = 1000;
		break;

		case 14:
			pX = 8;
			vX_Limit = 3000;
		break;

		case 15:
			pX = 8;
			vX_Limit = 7500;
		break;

		case 16:
			pX = 16;
			vX_Limit = 5000;
		break;

		case 17:
			pX = 10;
			vX_Limit = 3000;
		break;
		
		case 19:
			pX = 10;
			vX_Limit = 2500;
		break;

		case 20:
			pX = 8;
			vX_Limit = 3000;
		break;

		default:
		break;
	}
	lastError = curError;
	curError = crd_X - x;
	
	//vX += pX * (curError - lastError);
	//vX += iX * curError;
	
	eI += curError;

	vX = pX * curError + iX * eI;

	switch(part)
	{
		case 1:
			/*if (vX > -vX_Limit*1/4)
			{
				vX = -vX_Limit*1/4;
			}*/
		break;

		case 3:
			/*if (crd_X > tar_X)
			{
				vX = 0;
			}
			else if (vX > -vX_Limit*1/5)
			{
				vX = -vX_Limit*1/5;
			} */
		break;

		case 4:
			if (crd_Y > tar_Y - 400)
			{
				vX_Limit = 7500;

				vX = pX * (crd_Y - tar_Y + 400);
			}
		break;

		case 5:
			if (Reflect_Active() && !reflected)
			{
				reflected = 1;

				crd_X = tar_X;
				pX = 20;
			}
			else if (vX < vX_Limit * 0.6 && !reflected)
			{
				vX = vX_Limit * 0.6;
			}
		break;

		case 6:
			if (vX < vX_Limit*1/3)
			{
				vX = vX_Limit*1/3;
			}
		break;

		case 7:
			if (crd_Y > tar_Y - 450)
			{
				vX = -pX * (crd_Y - tar_Y + 450);
			}
		break;

		case 8:
			if (vX > -vX_Limit*1/3)
			{
				vX = -vX_Limit*1/3;
			}
		break;

		case 9:
			if (crd_Y < tar_Y - 200)
			{
				vX = vX_Limit * 1/6;
			}
		break;

		case 10:
			if (crd_Y < tar_Y + 500)
			{
				il_EN = 0;
				vX = -pX * (crd_Y - tar_Y - 500);
			}
			else
			{
				il_EN = 1;
			}
		break;
		
		case 11:
			if (vX < vX_Limit*1/2)
			{
				vX = vX_Limit*1/2;
			}

			if (il_EN && !retry_WithBasket)
			{
				vX = vX_Limit * 1/5;
			}
			//vX = vX_Limit;
		break;
		
		case 12:
			if (crd_A < -120 || crd_A > 0)
			{
				vX_Limit = 1000;
			}
			else
			{
				vX_Limit = 5000;
			}

			if (crd_Y < 8000)
			{
				pX = 14;
				vX = pX * (crd_Y - 8000);
			}
		break;

		case 13:
			/*if (crd_A < 20 && crd_A > -20)
			{
				vX = -vX_Limit;
			}
			else
			{
				vX = -vX_Limit * 2/3;
			}*/
			vX = -vX_Limit;
		break;

		case 23:
			if (crd_A < -40)
			{
				vX_Limit = 4000;
				pX = 14;
			}
		break;

		case 14:
		break;

	   	case 15:
			if (Read_IL() == 0)
			{
				vX = vX_Limit * 1/3;
			} 
		break;

		case 16:
			if (crd_A < -145)
			{
				vX_Limit = 7500;
			}
		break;

		case 17:
			if (vX > -vX_Limit * 1/2)
			{
				vX = -vX_Limit*1/2;
			}
		break;

		case 18:
			if (crd_Y < 1000)
			{
				vX_Limit = 2000;
			}
			else
			{
				vX_Limit = 3000;
			}
		break;

		case 19:
		break;

		case 20:
			if (crd_X > 200)
			{
				ELVTR_Goto(PUTBUN);
			}
		break;
		default:
		break;
	}

	if (vX > vX_Limit)
	{
		vX = vX_Limit;
	}
	else if (vX < -vX_Limit)
	{
		vX = -vX_Limit;
	}
	 
	return vX;
}

double PID_Y(double y, char part)
{
	static double pY = 10;
	static double iY = 0;
	static double lastError = 0;
	static double curError = 0;
	static double vY_Limit = 7000;
	static double vY = 0;
	
	static double eI = 0;

	static char reflected = 0;
	static char gv1 = 0;

	switch(part)
	{
		case 1:
			pY = 16;
			vY_Limit = 1125;
		break;

		case 2:
			pY = 16;
			vY_Limit = 7500;
		break;

		case 3:
			pY = 8;
			vY_Limit = 5250;
		break;

		case 4:
			pY = 14;
			vY_Limit = 7500;
		break;
		
		case 5:
			pY = 6;
			vY_Limit = 6000;
		break;

		case 6:
			pY = 10;
			vY_Limit = 4000;
		break;

		case 7:
			pY = 9;
			vY_Limit = 6000;
		break;

		case 8:
			pY = 10;
			vY_Limit = 4000;
		break;

		case 9:
			vY_Limit = 5000;
		break;
		
		case 10:
			pY = 6;
			vY_Limit = 7500;
		break;

		case 11:
			pY = 12;
			vY_Limit = 2500;
		break;

		case 12:
			pY = 8;
			vY_Limit = 7500;
		break;

		case 13:
			pY = 12;
			vY_Limit = 7000;
		break;

		case 23:
			pY = 10;
			vY_Limit = 4000;
		break;

		case 14:
			pY = 16;
			vY_Limit = 7500;
		break;

		case 15:
			vY_Limit = 7500;
		break;

		case 16:
			pY = 10;
			vY_Limit = 3000;
		break;

		case 18:
			pY = 8;
			vY_Limit = 4000;
		break;

		case 20:
			pY = 10;
			vY_Limit = 1000;
		break;

		default:
		break;
	}

	lastError = curError;
	
	curError = crd_Y - y;
	
	//vY += pY * (curError - lastError);

	//vY += iY * curError;
	
	eI += curError;

	vY = pY * curError + iY * eI;

	switch(part)
	{ 
		case 2:
			if (crd_A < 85)
			{
				vY = -vY_Limit * 2/3;
			}
			else
			{
				vY = -vY_Limit;
			}
		break;

		case 4:
			if (crd_X > tar_X + 300)
			{
				il_EN = 1;
				vY = -vY_Limit * 1/6;
			}
			else
			{
				il_EN = 0;

				if (vY > -vY_Limit * 1/2)
				{
					vY = -vY_Limit * 1/2;
				}
				if (crd_A > 92 || crd_A < 87)
				{
					vY = -vY_Limit * 1/2;
				}
			}
		break;

		case 5:
			if (Press(R1) || reflected)
			{
				vY = -vY_Limit * 0.3;
				reflected = 1;		    
			}
		break;

		case 6:
			if (crd_X < tar_X + 380)
			{
				vY = pY * (crd_X - tar_X - 380);
			}
		break;

		case 7:
			if (vY > -vY_Limit*1/3)
			{
				vY = -vY_Limit*1/3;
			}
			//vY = vY_Limit;

		break;

		case 8:
			if (crd_X > tar_X - 946)
			{
				vY = -pY * (crd_X - tar_X + 946);
			}
			else
			{
				//vY = -vY_Limit * 1/8;
				vY = 0;
			}
		break;

		case 9:
		break;

		case 10:		
			if (vY < vY_Limit * 1/5)
			{
				vY = vY_Limit*1/5;
			}
			
			if (crd_Y < 10100)
			{
				ARM_Down();
			}
		break;

		case 12:
			if (crd_A < -120 || crd_A > 0)
			{
				vY = vY_Limit * 2/3;
			}
			else if (vY < vY_Limit * 1/4)
			{
				vY = vY_Limit * 1/4;
			}
		break;

		case 13:
		break;

		case 23:
			if (crd_A < -40)
			{
				vY_Limit = 4000;
				vY = vY_Limit;
			}
			else
			{
				vY_Limit = 1000;
			}
		break;

		case 14:
			if (crd_A < -100)
			{
				vY = vY_Limit * 1/2;
			}
			else
			{
				vY = vY_Limit;
			}
		break;

		case 15:
			vY = vY_Limit;
		break;

		case 16:
			if (crd_A > 170 || crd_A < -170 || gv1)
			{
				if (!GV1_Active())
				{
					vY = vY_Limit * 3/4;
				}
				else
				{
					if (!gv1)
					{
						gv1 = 1;
						crd_Y = 0;
						pY = 12;
						vY_Limit = 4000;
					}
				}		
			}
		break;
		
		case 17:
			if (crd_X < 400)
			{
				vY_Limit = 500;
			}
			else
			{
				vY_Limit = 4000;
			}
		break;
		
		default:
		break;
	}

	if (vY > vY_Limit)
	{
		vY = vY_Limit;
	}
	else if (vY < -vY_Limit)
	{
		vY = -vY_Limit;
	}

	return vY;
}

double PID_W(double a, char part)
{
	static double pW = 0.3;
	static double lastError = 0;
	static double curError = 0;
	static int w_Limit = 10;
	static double w = 0;
	
	static double eI = 0;

	switch(part)
	{
		case 0:
			pW = 0.4;
			w_Limit = 3;
		break;

		case 2:
			pW = 0.4;
			w_Limit = 8;
		break;

	   	case 3:
			pW = 0.2;
			w_Limit = 3;
		break;

		case 4:
			pW = 0.5;
			w_Limit = 8;
		break;

		case 5:
			pW = 0.3;
			w_Limit = 5;
		break;

		case 6:
			pW = 0.3;
			w_Limit = 7;
		break;

		case 7:
			pW = 0.4;
			w_Limit = 7;
		break;

		case 8:
			pW = 0.4;
			w_Limit = 8;
		break;
		
		case 11:
			pW = 0.2;
			w_Limit = 3;
		break;

		case 12:
			pW = 0.2;
			w_Limit = 4;
		break;

		case 13:
			pW = 0.2;
			w_Limit = 10;
		break;

		case 23:
			pW = 0.15;
			w_Limit = 10;
		break;

		case 14:
			w_Limit = 4;
		break;

		case 16:
			pW = 0.4;
			w_Limit = 8;
		break;

		case 20:
			pW = 0.15;
			w_Limit = 4;
		break;

		default:
		break;
	}
	
	curError = crd_A - a;

	if (curError >= 180)
	{
		curError -= 360;
	}
	else if (curError < -180)
	{
		curError += 360;												  
	}	
	
	//w = pW * (curError - lastError);
	
	lastError = curError;
	
	w = pW * curError;

	if (w > w_Limit)
	{
		w = w_Limit;
	}
	else if (w < -w_Limit)
	{
		w = -w_Limit;
	}
	
	switch(part)
	{
		case 5:
		break;

		case 16:
			if (GV1_Active())
			{
				w = 0;
			}
		break;

		case 23:
		break;

		case 19:
			w = 0;
		break;

		case 20:
			//if (Press(L2))
		break;

		default:
		break;
	}
	return -w;
}

void Wheel_Ctrl_Global(double vX, double vY, double w, double crd_A)
{	
	static double vx = 0;
	static double vy = 0;
	static double c1 = 0;
	static double c2 = 0;
	
	c1 = cos(crd_A * PI/180);
	c2 = sin(crd_A * PI/180);
	vx = -vY * c1 + vX * c2;
	vy = vY * c2 + vX * c1;
	
	Wheel_Ctrl(vx, vy, w);
}

char GV1_Active(void)
{
	return Sensor_Active(F15);
}

char GV2_Active(void)
{
	return Sensor_Active(F14);
}

char Basket_Active(void)
{
	return Sensor_Active(D3);
}

char Reflect_Active(void)
{
	return Sensor_Active(F11);
}

char Touched(void)
{
	char filter = 0;

	if (!touch_EN)
	{
		return 0;
	}
	else
	{
		filter = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
		
		if (filter)
		{
			Delay(10);
	
			return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
		}
		else
		{
			return 0;
		}
	}
}

void Touch_Switch(char a)
{
	touch_EN = a;
}