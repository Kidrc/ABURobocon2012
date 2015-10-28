#ifndef __LCD_H
#define __LCD_H
//在这里包含LCD屏幕的驱动函数库
/*OLED的接口程序*/
/*
*	占用PG0~PG3引脚，分别为：
*	PG0-CS PG1-DC/RS PG2-SCLK/D0 PG3-SDIN/D1
*	屏幕其余引脚悬空即可。
*/
#include "oled.h"
#include "GeneralDef_CM3.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Write(int x, int y, char * str);
void LCD_Write_Int(int x, int y, int n);
void LCD_Inform(char *);

#endif
