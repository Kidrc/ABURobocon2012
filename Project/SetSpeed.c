#include "SetSpeed.h"

/*	函数名：Wheel_Ctrl
*	
*	2012/03/01
*	功能：作为上层控制车的速度、姿态和底层轮子速度的接口。
*		三轮模型。
*	参数：vx,vy,w, 均为整型。以机器人自身中心为原点，原点与节点2轮子连线为y轴正向。w方向从y到x取小角度。
*	返回值：无
*	Version 1.0
*/
void Wheel_Ctrl(double vx, double vy, double w)
{ 
	const double R = 190;
    const double L1 = 330;
    const double L2 = 260;

		SetSpeed(1, (vx + L1*w)*1.3);
		SetSpeed(4, vy - R*w);
		SetSpeed(3, (L2*w - vx)*1.3);
		SetSpeed(2, -R*w - vy);
}
/*
*	函数名：SetSpeed
*
*	2011/12/2
*	删掉没用的注释，1.1版本的测试结果良好。
*	将itoa改为自己定义的版本。需要GeneralDef.h支持。
*	Version 1.2
*
*	2011/11/17
*	删去加速度参数，测试效果。
*	参数：驱动卡节点号node，8位无符号整数，取值在[0, 254]中，0表示无节点;
*		  速度speed, 16位整数
*	Version 1.1
*
*	2011/11/12
*	功能：以指定加速度设置目标电机的速度，博创电机驱动卡
*		  需要USART1和PA9使能
*	返回值：无
*
*	Version 1.0
*/
void SetSpeed(uint8_t node, int16_t speed)
{
	uint8_t speed_Len = 0;
	char speed_Buf[20];	
	
	if (node != 0)
	{
		itoa(speed_Buf, node, 1);
		
		for (speed_Len = 0; speed_Buf[speed_Len]; speed_Len++);
	}	
	
	speed_Buf[speed_Len++] = 'v';
	
	itoa(speed_Buf + speed_Len, speed, 5);

	USART_Transmit(USART3, speed_Buf);
}

