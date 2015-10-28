
#include "LCD5110.h"
#include "Pixel_EN.h"
#include "Pixel_CH.h"
#include "Delay.h"

//-------------------------------------------------------------------
//	Nokia5510指令集
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0   1   0   0   PD  V   H
//                      |   |   |_______1->扩展指令集  0->基本指令集
//                      |   |___________1->垂直寻址    0->水平寻址 
//                      |_______________1->低功耗模式  0->正常模式
//-------------------------------------------------------------------
//	显示模式
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0   0   0   1   D   0   E
//                      |       |_______0    0    1    1
//                      |_______________0    1    0    1
//                                      白屏 正常 全显 反转
//-------------------------------------------------------------------
// 1.向LCD发送数据:command=0,dt作为显示内容；command=1,dt作为指令
void LCD_write_byte(unsigned char dt, unsigned char command)
{
	unsigned char i;  	
    sce0; 	
	if(command)
		dc1;
	else
		dc0;		
	for(i=0;i<8;i++)
	{ 	sclk0;
		if(dt&0x80)
			sdin1;
		else
			sdin0;
		dt=dt<<1;	
		 			
		sclk1; 		  
		sclk0;//		
	}	  
}
// 2.LCD初始化：液晶上电，复位，clk使能
void LCD_init(void)
{
    gnd0;
	vcc1;  
	sce1;
	res1;
	Delay(10);
	res0;  	
  	Delay(100);
  	res1;  
	Delay(10);
	LCD_write_byte(0x21,0);//LCD功能设置：芯片活动，水平寻址，使用扩展指令
	LCD_write_byte(0xd0,0);//设置VOP值，室温下的编程范围为3.00-10.68
	LCD_write_byte(0x20,0);//LCD功能设置：芯片活动，水平寻址，使用基本指令
	LCD_write_byte(0x0C,0);//设定显示配置:普通模式
	sce1;
	backled1;
}
//3.LCD全部清屏（耗时8.4毫秒！！）
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0);
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);	 						
		} 
	}
	sce1;
}
//4.设置LCD当前坐标
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// column
	LCD_write_byte(0x80 | X, 0);// row
	sce1;
} 
// 5.向LCD写一个英文字符
void LCD_write_char(unsigned char c)
{
	unsigned char line;
	c-= 32;
	for (line=0; line<6; line++)
	LCD_write_byte(font6x8[c][line], 1);
}
// 6.向LCD写英文字符串
void LCD_write_String(unsigned char X,unsigned char Y,char *s)
{
	LCD_set_XY(X,Y);
	while (*s) 
	{
		LCD_write_char(*s);
		s++;
	}
} 
//7.显示一个数字
//x,y:起点坐标
//num:数值(0~65536); 
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<8;i++) 
	{
		LCD_write_byte(shuzi[c*16+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=8; i<16;i++) 
	{
		LCD_write_byte(shuzi[c*16+i],1);
	}
	sce1;
}
// 8.向LCD写中文字
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<16;i++) 
	{
		LCD_write_byte(han[c*32+i],1); 
	}

    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=16; i<32;i++) 									   
	{
		LCD_write_byte(han[c*32+i],1);
	}	
	sce1;
}
/* 设置LCD对比度(对比度范围: 0~127)
void LCD_SetContrast(u8 contrast)
{
    LCD_Send(0x21, DC_CMD);
    LCD_Send(0x80 | contrast, DC_CMD);
    LCD_Send(0x20, DC_CMD);
}
*/
/*LCD部分清屏
void LCD_Clear_part(u8 X,u8 Y,u8 L,u8 H)
{
	u16 i,j;
	
	LCD_Send(0x0c, DC_CMD);
    LCD_Send(0x80, DC_CMD);
	
	for (i=Y; i<Y+H+1; i++)
          for(j=X;j<X+L+1;j++)
          { LCD_SetXY(j,i);
	    LCD_Send(0, DC_DATA);	
          }
} */
/*
void LCD_Write_Char(u8 ascii)
{
    u8 n;
	
    ascii -= 32; //ASCII码-32
	
    for (n=0; n<6; n++)
		LCD_Send(Font6x8[ascii][n], DC_DATA);
}
*/
/*m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}*/	
