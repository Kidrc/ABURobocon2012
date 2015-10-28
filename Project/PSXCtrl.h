#include "PSX.h"
#include "Action.h"
#include "SetSpeed.h"

/*
*	手柄控制模块。
*
*	Version 2.0
*	按键映射：
*	△——切换至微调挡(D1)	×——进行下一个动作	↑——刹车，电梯停，最高优先级
*	左摇杆——车平移方向	右摇杆——车自转方向
*	L1——电梯向上 L2——电梯向下	R2——在快速和慢速挡间切换(D3/D2)
*	R1——夹币	右摇杆按键——投币并收回手臂
*	Start--确认传感器无误后启动程序
*
*	使用时先调用PSXCtrl_Init()函数。
*	需要PSX.h的支持。
*	占用：
*		PA9, PA10引脚
*	 	USART1, EXTI0外设
*	中断使用1位抢占优先级，TIM2_IRQn为0，EXTI0_IRQn为1
*
*	2012/03/07	23:34
*	四轮全向的半自动控制。
*	Version 1.5
*
*	2012/03/01 12:58
*	进行3轮全向的尝试，改为三轮控制。
*	Version 1.4
*
*	2011/12/18	16:10
*	增加上层机构功能。
*	Version 1.3
*
*	2011/11/30 20:07
*	改为2轮版本
*	Version 1.2
*
*	2011/11/27 16:27
*	将TIM2中断移至PSX模块中。
*	Version 1.2
*	
*	2011/11/26 13:16
*	Version 1.0
*/
#ifndef __PSXCTRL_H
#define __PSXCTRL_H

extern void PSXCtrl_Init(void);

extern void Manual_Control(uint8_t data[]);	//手动控制

extern void EXTI0_IRQHandler(void);

float Abs(float);
#endif
