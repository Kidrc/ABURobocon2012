#include "PSX.h"
#include "Action.h"
#include "SetSpeed.h"

/*
*	�ֱ�����ģ�顣
*
*	Version 2.0
*	����ӳ�䣺
*	�������л���΢����(D1)	������������һ������	������ɲ��������ͣ��������ȼ�
*	��ҡ�ˡ�����ƽ�Ʒ���	��ҡ�ˡ�������ת����
*	L1������������ L2������������	R2�����ڿ��ٺ����ٵ����л�(D3/D2)
*	R1�����б�	��ҡ�˰�������Ͷ�Ҳ��ջ��ֱ�
*	Start--ȷ�ϴ������������������
*
*	ʹ��ʱ�ȵ���PSXCtrl_Init()������
*	��ҪPSX.h��֧�֡�
*	ռ�ã�
*		PA9, PA10����
*	 	USART1, EXTI0����
*	�ж�ʹ��1λ��ռ���ȼ���TIM2_IRQnΪ0��EXTI0_IRQnΪ1
*
*	2012/03/07	23:34
*	����ȫ��İ��Զ����ơ�
*	Version 1.5
*
*	2012/03/01 12:58
*	����3��ȫ��ĳ��ԣ���Ϊ���ֿ��ơ�
*	Version 1.4
*
*	2011/12/18	16:10
*	�����ϲ�������ܡ�
*	Version 1.3
*
*	2011/11/30 20:07
*	��Ϊ2�ְ汾
*	Version 1.2
*
*	2011/11/27 16:27
*	��TIM2�ж�����PSXģ���С�
*	Version 1.2
*	
*	2011/11/26 13:16
*	Version 1.0
*/
#ifndef __PSXCTRL_H
#define __PSXCTRL_H

extern void PSXCtrl_Init(void);

extern void Manual_Control(uint8_t data[]);	//�ֶ�����

extern void EXTI0_IRQHandler(void);

float Abs(float);
#endif
