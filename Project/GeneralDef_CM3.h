#include "stm32f10x.h"
/*
*	�ѳ��ù��ܺ͹ܽŶ���ɺ꣬��������
*	����һЩ���ú��������ٴ�����
*	
*	Version 2.0
*	�°汾������
*	RCC�����򻯺�
*	����/�ַ�����ת����
*	GPIO�������ü򻯺���
*	SPI1/2���ź궨��ͳ�ʼ������
*	����1/2/3���ź궨�塢ģ���ʼ�������ͷ����ַ�������
*	TIM3/4���ų�ʼ������
*	LED��ʼ���Ϳ��غ���
*	�̵�����ʼ���Ϳ��غ���
*	��������ʼ���Ͷ�ȡ����
*	΢�����س�ʼ���Ͷ�ȡ����
*	Delay��ʱ��������ԭģ�����Ͻ�����
*	PSXʱ�ӿ��غ꣬��ʹ��PSXģ�����轫����1��������0
*	2012/03/19	20:15	
*/
#ifndef __GENDEF_H
#define __GENDEF_H
 
 #define ON 1
 #define OFF 0
 
#define PSX_SWITCH 1

/*RCC*/
#define RCC_GPIOA() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)		
#define RCC_GPIOB() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define RCC_GPIOC() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define RCC_GPIOD() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)
#define RCC_GPIOE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE)
#define RCC_GPIOF() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE)
#define RCC_GPIOG() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE)
#define RCC_AFIO()	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE)

/*����/�ַ�����ת*/
extern void itoa(char * buffer, int num, int length);

#ifndef __stdlib_h
	extern int atoi(char * buffer);
#endif

/*LED, GPIOB*/
#define LED1 GPIO_Pin_12
#define LED2 GPIO_Pin_13
#define LED3 GPIO_Pin_14			
#define LED4 GPIO_Pin_15

extern void LED_On(uint16_t LED);
extern void LED_Off(uint16_t LED);	
extern void LED_Init(void);

/*GPIO*/
extern void GPIO_Config(uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed, GPIO_TypeDef * GPIOx);

#ifdef __STM32F10x_SPI_H			//SPI���

	#define SPI1_MOSI GPIO_Pin_7		//GPIOA
	#define SPI1_MISO GPIO_Pin_6
	#define SPI1_SCK GPIO_Pin_5
	#define SPI1_NSS GPIO_Pin_4
	#define SPI2_NSS GPIO_Pin_12
	#define SPI2_SCK GPIO_Pin_13
	#define SPI2_MISO GPIO_Pin_14
	#define SPI2_MOSI GPIO_Pin_15
	
	#define RCC_SPI1() RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
	#define RCC_SPI2() RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)
	
	extern void	GPIO_SPI1_Init(void); 
	extern void GPIO_SPI2_Init(void);
	
#endif

/*USART*/
#ifdef __STM32F10x_USART_H

	#define USART1_Rx	GPIO_Pin_10
	#define USART1_Tx	GPIO_Pin_9
	#define USART2_Rx	GPIO_Pin_6
	#define USART2_Tx 	GPIO_Pin_5
	#define USART3_Rx	GPIO_Pin_9
	#define USART3_Tx	GPIO_Pin_8
	
	#define RCC_USART1()	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
	#define RCC_USART2()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)
	#define RCC_USART3()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE)
		
	extern void USART1_Init(unsigned int BaudRate);
	extern void USART2_Init(unsigned int BaudRate);
	extern void USART3_Init(unsigned int BaudRate);
	extern void USART_Transmit(USART_TypeDef * USARTx, char * str);

#endif

/*TIM*/
#ifdef __STM32F10x_TIM_H
   	
	#define RCC_TIM1() 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE)
	#define RCC_TIM2()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE)
	#define RCC_TIM3() 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE)
	#define RCC_TIM4()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)
	#define RCC_TIM5()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE)
	#define RCC_TIM6()  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE)
	#define RCC_TIM7()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE)
	#define RCC_TIM8()	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE)

	extern void GPIO_TIM3_Init(void);
	extern void GPIO_TIM4_Init(void);
	
/*Delay*/
void Delay(uint16_t ms);
/*Clock*/
extern uint16_t seconds;

void Clock_Init(void);
void Clock_Start(void);
void Clock_Stop(void);

#endif

/*�̵���Relay*/
	void Relay_Init(void);
	void Relay_Switch(char node, char state);
/*������Sensor*/
typedef enum{F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, D0, D1, D2, D3} sensor_Node;

	void Sensor_Init(void);
	char Sensor_Active(sensor_Node node);
/*΢������Switch������ΪOn*/
typedef enum{G10, G11, G12, G13, G14, G15} switch_Node;
	void Switch_Init(void);
	char Switch_On(switch_Node node);
	
#endif
