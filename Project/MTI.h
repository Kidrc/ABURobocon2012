#ifndef __MTI_H
#define __MTI_H
/*
*	�ļ�����MTi�����ǵĽӿڳ���
*	ռ��PD5��PD6�ڡ�
*	ʹ��DMA1�ĵ���ͨ����
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