#ifndef __LCD_H
#define __LCD_H
//���������LCD��Ļ������������
/*OLED�Ľӿڳ���*/
/*
*	ռ��PG0~PG3���ţ��ֱ�Ϊ��
*	PG0-CS PG1-DC/RS PG2-SCLK/D0 PG3-SDIN/D1
*	��Ļ�����������ռ��ɡ�
*/
#include "oled.h"
#include "GeneralDef_CM3.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Write(int x, int y, char * str);
void LCD_Write_Int(int x, int y, int n);
void LCD_Inform(char *);

#endif
