 #ifndef __ACTION_H
#define __ACTION_H
 
#include "stm32f10x.h"
#include "GeneralDef.h"
#include "PWM.h"
#include "Delay.h"
#include "LCD_STD.h"
/*
*	IO口分配：
*		舵机(STeeR)：PWM-PA0*-23
*		H桥(H)：DIR1-PB2-37, PWM1-PE9*-40, DIR2-PB1-36, PWM2-PE11*-42
*		电梯用继电器(ELeVaToR)：BRK-PB5-91, DIR-PB7-93
*		篮子用继电器(BASKET)；BRK-PB9-96
*		投币(ToKeN)：CLDR-PB11-48
*		传感器(Sensor)：	Sensor_Basket-PB15-54				   
*				Sensor_X-PC0-15
*				Sensor_Y1-PC1-16, Sensor_Y2-PC2-17, Sensor_Y3-PC3-18, Sensor_Y4-PC10-78
*				Sensor_Token-PC5-34
*				Sensor_Arm-PC9-66
*	占用TIM1、TIM5、USART1、SPI1外设
*		带‘*’的引脚为开启复用功能的引脚，在开启新的外设时注意避免冲突
*/

/*引脚宏定义*/
#define STR_PWM GPIO_Pin_0	//PA0
#define STR_GND GPIO_Pin_0	//PB0
#define H_DIR1	GPIO_Pin_2	//PB2
#define H_PWM1	GPIO_Pin_9	//PE9
#define H_DIR2	GPIO_Pin_1	//PB1
#define H_PWM2	GPIO_Pin_11	//PE11
#define H_GND	GPIO_Pin_4	//PB4
#define ELVTR_RUN	GPIO_Pin_5	//PB5
#define ELVTR_VCCB	GPIO_Pin_6	//PB6
#define ELVTR_DIR	GPIO_Pin_7	//PB7
#define ELVTR_VCCD	GPIO_Pin_8	//PB8
#define BASKET_BRK	GPIO_Pin_9	//PB9
#define BASKET_VCC	GPIO_Pin_10	//PB10
#define TKN_CLDR	GPIO_Pin_12	//PB12
#define S_BASKET	GPIO_Pin_15	//PB15
#define S_X	GPIO_Pin_0	//PC0
#define S_Y1	GPIO_Pin_1	//PC1
#define S_Y2	GPIO_Pin_2	//PC2
#define S_Y3	GPIO_Pin_3	//PC3
#define S_Y4	GPIO_Pin_10	//PC10
#define S_TOKEN	GPIO_Pin_5	//PC5
#define S_ARM	GPIO_Pin_9	//PC9
/*传感器类型定义*/
typedef enum {SENSOR_Y1, SENSOR_Y2, SENSOR_Y3, SENSOR_Y4, SENSOR_X, SENSOR_BASKET, SENSOR_TOKEN, SENSOR_ARM} action_Sensor;
/*电梯竖直位置定义*/
typedef enum {ELVTR_Y0, ELVTR_Y1, ELVTR_Y2, ELVTR_Y3, ELVTR_Y4} elevator_Y;
/*电梯水平位置定义*/
typedef enum {ELVTR_X0, ELVTR_X1, ELVTR_X2, ELVTR_X3, ELVTR_X4} elevator_X;

/*准许拿取篮子的信号量*/
extern int bkt_Enable;

void Action_Init(void);

void Elevator_Up(void);
void Elevator_Down(void);
void Elevator_Left(void);
void Elevator_Right(void);

void Elevator_Stop_X(void);
void Elevator_Stop_Y(void);

// uint8_t Basket_Found(void);
void Basket_Patch(void);
void Basket_Dispatch(void);
void Basket_Up(void);
void Basket_Down(void);
void Basket_Stop(void);
extern void Basket_Scan(void);

extern void Token_Scan(void);

extern void Elevator_Goto_Y(elevator_Y y);
extern void Elevator_Goto_X(elevator_X x);
uint8_t Sensor_Captured(action_Sensor sensor);

/*拿取投币*/
#define Token_Patch()	GPIO_SetBits(GPIOB, TKN_CLDR)				//15

/*放开投币*/
#define Token_Dispatch()	GPIO_ResetBits(GPIOB, TKN_CLDR)

#endif
