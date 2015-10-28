/*
*	M的自动控制。
*/
#ifndef __NAVG_H
#define __NAVG_H

#include "GeneralDef_CM3.h"
#include "MTI.h"
#include "Encoder.h"
#include "SetSpeed.h"
#include "PSX.h"
#include "Action.h"
#include "PSXCtrl.h"
#include "IL.h"

#define TOUCH_ENABLE 1
#define TOUCH_DISABLE 0
#include <math.h>

extern double crd_X;
extern double crd_Y;
extern double crd_A;
extern double error_A; 
extern char cur_Part;
extern volatile char revised_A;
extern volatile char interrupted;
extern char retry_WithBasket;

extern char GetReady(void);
extern void Adjust(char part);
extern void Goto_Pos(int x, int y, int a, char part);

extern void Read_Pos(void);
void Revise_Pos(char part);

char GV1_Active(void);
char GV2_Active(void);
char Basket_Active(void);
char Reflect_Active(void);
char Touched(void);
void Touch_Switch(char a);

#endif
