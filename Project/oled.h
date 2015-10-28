#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
#include "stdlib.h"
#include "stm32f10x.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

typedef u8 uint8_t;
typedef u32 uint32_t;

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS GPIOG, GPIO_Pin_0
//#define OLED_RST  PBout(14)//在MINISTM32上直接接到了STM32的复位脚！	
#define OLED_RS GPIOG, GPIO_Pin_1
#define OLED_WR GPIOD, GPIO_Pin_5		  
#define OLED_RD GPIOD, GPIO_Pin_4
#define OLED_VCC GPIOD, GPIO_Pin_5
#define OLED_GND GPIOD, GPIO_Pin_4

//PB0~7,作为数据线
#define DATAOUT(x) GPIOD->ODR=(GPIOD->ODR&0xff00)|(x&0x00FF); //输出
  
//使用4线串行接口时使用 
#define OLED_SCLK GPIOG, GPIO_Pin_2
#define OLED_SDIN GPIOG, GPIO_Pin_3
#define OLED_RST GPIOD, GPIO_Pin_1
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 
#endif  
	 



