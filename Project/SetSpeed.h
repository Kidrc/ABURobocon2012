#ifndef __SETSPD_H
#define __SETSPD_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "GeneralDef_CM3.h"

void Wheel_Ctrl(double vx, double vy, double w);
void SetSpeed(uint8_t node, int16_t speed);					//Version 1.1

#endif
