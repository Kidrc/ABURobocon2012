#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"
#include "GeneralDef_CM3.h"

extern double maxCNT1;
extern double maxCNT2;


extern const double PI;
extern void Encoder_Init(void);
extern double Read_Encoder(char node);
extern void Update_Encoder(char node);

#endif
