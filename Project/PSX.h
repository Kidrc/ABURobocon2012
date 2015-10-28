/*
*	PSX�ֱ������ú�ʹ�á�
*	ʹ�ø�ģ��ʱ�ȵ���PSX_Init()������
*	һ��Ҫ��ϵͳʱ�Ӹĳ�24MHz������GeneralDef_CM3.h�е�PSX_SWITCH��ĳ�1
*	��ҪGeneralDef.h��Delay.h��֧��
*	ռ�ã�
*		PB12, PB13, PB14, PB15����
*		SPI2, TIM6ģ��
*
*	Version 2.0
*	��Ӧ�ڶ�������ذ塣
*	����ʱģ���ΪTIM6����Ϊ����ռ���κ����š�
*	��SPIģ������SPI2��
*	�Ƴ�LEDָʾ����Ϊ��������Ļ���档
*	������
*	�ֱ���ʼ��������ʹ�ã�
*		SPI2��TIM6
*	�����жϺ���
*	�ж��ֱ��Ƿ�����ĺ���
*	2012/03/20
*
*	2011/11/27
*	��PSXCtrl�е�TIM2�ж�д�ظ�ģ�飬��Ϊ���ֱ���Ҫ����ָ�
*	Version 1.2
*	
*	2011/11/26
*	�ѳ��������Ϊö�����͡�
*	ʹ��1.2�汾��GeneralDef����д�˳�ʼ�����֣��������ʼ��д���ļ��
*	������Ʋ��ֵ�PSXCtrl��
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
extern const uint8_t PSX_CONN[5];		//�����ź�
extern const uint8_t PSX_MODE[9];		//ģʽ�����ź�
extern const uint8_t PSX_POLL[9];		//��ѯ�ź�

typedef enum{SLCT, STRT, UP, RGHT, DOWN, LEFT, L2, R2, L1, R1, TRNGL, CRCL, CRSS, SQR, JOYL, JOYR} button_Def;
extern button_Def button;


extern void PSX_Init(uint16_t PSX_Scan_Period);

extern void Set_Ana(void);				//����Ϊģ��ģʽ
extern uint8_t Press(uint8_t);					//�жϰ����Ƿ���
extern uint8_t Is_Cmd(void);

extern char Data_Valid(void);

extern void TIM6_IRQHandler(void);

#endif
