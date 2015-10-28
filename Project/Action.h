 #ifndef __ACTION_H
#define __ACTION_H
 
#include "GeneralDef_CM3.h"
#include "PWM.h"
#include "LCD_STD.h"
/*
*	2012/03/23
*	对应新的主控板和1.75版的M。
*	使用GeneralDef_CM3。
*	Version 2.0
*/
/*
*	模块分配：
*		H桥(H)：电梯(ELVTR)-CH1
*		刹车(BRaKe)：BRK-继电器6
*		投币(ToKeN)：TKN1-继电器4，TKN2-继电器5
*	占用1个H桥、3个继电器（2*12+1*24)、SPI、LCD
*/

#define ELVTR_ENABLE 1
#define ELVTR_DISABLE 0

typedef enum {LOADC1, LIFTC1, UNLOADC, LOADBASKET, LIFTBASKET1, LIFTBASKET2, LOADC2, LIFTC2, LIFTBUN, PUTBUN} elvtr_Pos;

extern void Action_Init(void);
void Com_Init(void);
void Touch_Init(void);

extern void ELVTR_Up(void);
extern void ELVTR_Down(void);
extern void ELVTR_Stop(void);
extern void ELVTR_Goto(elvtr_Pos y);
extern void ELVTR_Check(void);
extern char ELVTR_Arrived(void);
extern uint16_t ELVTR_Pos(void);
extern void ELVTR_Manual(char a);
extern char ELVTR_IsManual(void);

extern void TKN_Patch(void);
extern void TKN_Dispatch(void);

extern void ARM_Out(void);
extern void ARM_In(void);

extern void ARM_Up(void);
extern void ARM_Down(void);

extern void Com_On(void);
extern void Com_Off(void);

void AbsEncoder_Init(void);
uint16_t Read_AbsEncoder(void);

void PrdIT_Init(void);

#endif
