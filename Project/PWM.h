/*
*	PWM输出模块。
*	使用TIM1/TIM5的1/2/3/4通道。
*
*	Version 2.0
*	对应第二版的主控板。
*	追加了：
*	4个舵机的控制模块，使用：
*		TIM5的4个通道
*	2个H桥共4路的控制模块，使用：
*		TIM1的4个通道，PE3~PE6
*	PSX时钟的判定（24MHz/72MHz)	
*	2012/03/20	18:51
*
*	Version 1.1
*	包含：
*	PWM初始化函数
*	PWM输出函数
*	PWM停止函数
*	2012/很久以前
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

/*H桥*/
void HBridge_Init(void);
void HBridge_Ctrl(char hNode, char channel, char dir, char speed);

/*舵机*/
typedef enum {PA0, PA1, PA2, PA3} steer_Node;
void Steer_Init(void);
void Steer_Ctrl(steer_Node node, char pos);

#endif
