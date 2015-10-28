/*
*	PWM���ģ�顣
*	ʹ��TIM1/TIM5��1/2/3/4ͨ����
*
*	Version 2.0
*	��Ӧ�ڶ�������ذ塣
*	׷���ˣ�
*	4������Ŀ���ģ�飬ʹ�ã�
*		TIM5��4��ͨ��
*	2��H�Ź�4·�Ŀ���ģ�飬ʹ�ã�
*		TIM1��4��ͨ����PE3~PE6
*	PSXʱ�ӵ��ж���24MHz/72MHz)	
*	2012/03/20	18:51
*
*	Version 1.1
*	������
*	PWM��ʼ������
*	PWM�������
*	PWMֹͣ����
*	2012/�ܾ���ǰ
*/
#ifndef __PWM_GEN_H
#define __PWM_GEN_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "GeneralDef_CM3.h"

#define PWM_CH1 0x01
#define PWM_CH2 0x02
#define PWM_CH3	0x04
#define PWM_CH4 0x08

typedef uint8_t pwm_Channel;

/*PWM*/
void PWM_Init(TIM_TypeDef * TIMx, pwm_Channel channel, uint16_t freq);
void PWM_Generate(TIM_TypeDef * TIMx, pwm_Channel channel, uint8_t ratio);
void PWM_Stop(TIM_TypeDef * TIMx, pwm_Channel channel);

/*H��*/
void HBridge_Init(void);
void HBridge_Ctrl(char hNode, char channel, char dir, char speed);

/*���*/
typedef enum {PA0, PA1, PA2, PA3} steer_Node;
void Steer_Init(void);
void Steer_Ctrl(steer_Node node, char pos);

#endif
