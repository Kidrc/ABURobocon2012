
#ifndef _LCD5110_H
#define _LCD5110_H

//#include "stm32f10x.h"
//#include "Delay.h"
//宽高像素
#define LCD_X_RES		84
#define LCD_Y_RES		48//英文字母是6*8的，一行显示14个字母，可显示6行


/****设置单片机引脚和液晶引脚对应**************/		
#define gnd0   GPIO_ResetBits(GPIOD, GPIO_Pin_0)  
#define vcc1   GPIO_SetBits(GPIOD, GPIO_Pin_2)   //液晶电源初始化
#define sce0   GPIO_ResetBits(GPIOD, GPIO_Pin_6)  //片选
#define res0   GPIO_ResetBits(GPIOD, GPIO_Pin_7)  //复位,0复位
#define dc0    GPIO_ResetBits(GPIOD, GPIO_Pin_5)   //1写数据，0写指令
#define sdin0  GPIO_ResetBits(GPIOD, GPIO_Pin_4)   //数据
#define sclk0  GPIO_ResetBits(GPIOD, GPIO_Pin_3)   //时钟
#define backled0  GPIO_ResetBits(GPIOD, GPIO_Pin_1)   //背光
 
#define sce1   GPIO_SetBits(GPIOD, GPIO_Pin_6)  //片选
#define res1   GPIO_SetBits(GPIOD, GPIO_Pin_7)  //复位,0复位
#define dc1    GPIO_SetBits(GPIOD, GPIO_Pin_5)   //1写数据，0写指令
#define sdin1  GPIO_SetBits(GPIOD, GPIO_Pin_4)   //数据
#define sclk1  GPIO_SetBits(GPIOD, GPIO_Pin_3)   //时钟
#define backled1  GPIO_SetBits(GPIOD, GPIO_Pin_1)   //背光
/************************************************/

/**************控制液晶屏的函数******************/
void LCD_init(void);//初始化
void LCD_clear(void); //全部清屏
void LCD_set_XY(unsigned char X, unsigned char Y);//设置光标坐标
void LCD_write_byte(unsigned char dt, unsigned char command);//写数据
void LCD_write_String(unsigned char X,unsigned char Y,char *s);//写英文字符串
void LCD_write_char(unsigned char c);//	写英文字母
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c);//写汉字
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c); //写数字
/************************************************/

#endif
/*************未开发完的函数**************/
//void LCD_Write_ChStr(u8 X,u8 Y,u8 n);
//u32 mypow(u8 m,u8 n); 
//void LCD_Clear_part(u8 X,u8 Y,u8 L,u8 H);
//void LCD_SetContrast(u8 contrast);
//void LCD_Write_Char(u8 ascii);
/*宽高像素
#define LCD_X_RES		84
#define LCD_Y_RES		48
*/
