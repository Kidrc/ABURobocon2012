/*
*	PSX手柄的设置和使用。
*	使用该模块时先调用PSX_Init()函数。
*	一定要将系统时钟改成24MHz，并将GeneralDef_CM3.h中的PSX_SWITCH宏改成1
*	需要GeneralDef.h、Delay.h的支持
*	占用：
*		PB12, PB13, PB14, PB15引脚
*		SPI2, TIM6模块
*
*	Version 2.0
*	对应第二版的主控板。
*	将定时模块改为TIM6，因为它不占用任何引脚。
*	将SPI模块移至SPI2。
*	移除LED指示，因为可以用屏幕代替。
*	包含：
*	手柄初始化函数，使用：
*		SPI2，TIM6
*	按键判断函数
*	判断手柄是否操作的函数
*	2012/03/20
*
*	2011/11/27
*	将PSXCtrl中的TIM2中断写回该模块，因为用手柄就要接收指令。
*	Version 1.2
*	
*	2011/11/26
*	把常量定义改为枚举类型。
*	使用1.2版本的GeneralDef，重写了初始化部分，将外设初始化写进文件里。
*	分离控制部分到PSXCtrl。
*	Version 1.1
*
*	2011/11/20
*	Version 1.0
*/
#ifndef __PSX_H
#define __PSX_H

#include "stm32f10x.h"
#include "GeneralDef_CM3.h"
#include "LCD_STD.h"

#define ATT_High() GPIO_SetBits(GPIOB, SPI2_NSS)
#define ATT_Low()  GPIO_ResetBits(GPIOB, SPI2_NSS)

extern int stop_Area;
extern uint8_t data[9];
extern char dataID;
extern const uint8_t PSX_CONN[5];		//连接信号
extern const uint8_t PSX_MODE[9];		//模式设置信号
extern const uint8_t PSX_POLL[9];		//查询信号

typedef enum{SLCT, STRT, UP, RGHT, DOWN, LEFT, L2, R2, L1, R1, TRNGL, CRCL, CRSS, SQR, JOYL, JOYR} button_Def;
extern button_Def button;


extern void PSX_Init(uint16_t PSX_Scan_Period);

extern void Set_Ana(void);				//设置为模拟模式
extern uint8_t Press(uint8_t);					//判断按键是否按下
extern uint8_t Is_Cmd(void);

extern char Data_Valid(void);

extern void TIM6_IRQHandler(void);

#endif
