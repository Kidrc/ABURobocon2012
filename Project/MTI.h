#ifndef __MTI_H
#define __MTI_H
/*
*	文件包含MTi陀螺仪的接口程序。
*	占用PD5、PD6口。
*	使用DMA1的第六通道。
*/
#include "stm32f10x.h"
#include "GeneralDef_CM3.h"
#include "LCD_STD.h"

typedef struct
{
	int roll;
	int pitch;
	int yaw;
	
}mti_Angle;

void MTI_Init(void);
mti_Angle MTI_GetData(void);
mti_Angle MTI_GetAngle(void);
void USART_SendInt(USART_TypeDef *, int);
void MTI_UpdateInitAngle(void);

double Delta_Angle(double a1, double a2);

#endif