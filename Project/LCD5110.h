
#ifndef _LCD5110_H
#define _LCD5110_H

//#include "stm32f10x.h"
//#include "Delay.h"
//�������
#define LCD_X_RES		84
#define LCD_Y_RES		48//Ӣ����ĸ��6*8�ģ�һ����ʾ14����ĸ������ʾ6��


/****���õ�Ƭ�����ź�Һ�����Ŷ�Ӧ**************/		
#define gnd0   GPIO_ResetBits(GPIOD, GPIO_Pin_0)  
#define vcc1   GPIO_SetBits(GPIOD, GPIO_Pin_2)   //Һ����Դ��ʼ��
#define sce0   GPIO_ResetBits(GPIOD, GPIO_Pin_6)  //Ƭѡ
#define res0   GPIO_ResetBits(GPIOD, GPIO_Pin_7)  //��λ,0��λ
#define dc0    GPIO_ResetBits(GPIOD, GPIO_Pin_5)   //1д���ݣ�0дָ��
#define sdin0  GPIO_ResetBits(GPIOD, GPIO_Pin_4)   //����
#define sclk0  GPIO_ResetBits(GPIOD, GPIO_Pin_3)   //ʱ��
#define backled0  GPIO_ResetBits(GPIOD, GPIO_Pin_1)   //����
 
#define sce1   GPIO_SetBits(GPIOD, GPIO_Pin_6)  //Ƭѡ
#define res1   GPIO_SetBits(GPIOD, GPIO_Pin_7)  //��λ,0��λ
#define dc1    GPIO_SetBits(GPIOD, GPIO_Pin_5)   //1д���ݣ�0дָ��
#define sdin1  GPIO_SetBits(GPIOD, GPIO_Pin_4)   //����
#define sclk1  GPIO_SetBits(GPIOD, GPIO_Pin_3)   //ʱ��
#define backled1  GPIO_SetBits(GPIOD, GPIO_Pin_1)   //����
/************************************************/

/**************����Һ�����ĺ���******************/
void LCD_init(void);//��ʼ��
void LCD_clear(void); //ȫ������
void LCD_set_XY(unsigned char X, unsigned char Y);//���ù������
void LCD_write_byte(unsigned char dt, unsigned char command);//д����
void LCD_write_String(unsigned char X,unsigned char Y,char *s);//дӢ���ַ���
void LCD_write_char(unsigned char c);//	дӢ����ĸ
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c);//д����
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c); //д����
/************************************************/

#endif
/*************δ������ĺ���**************/
//void LCD_Write_ChStr(u8 X,u8 Y,u8 n);
//u32 mypow(u8 m,u8 n); 
//void LCD_Clear_part(u8 X,u8 Y,u8 L,u8 H);
//void LCD_SetContrast(u8 contrast);
//void LCD_Write_Char(u8 ascii);
/*�������
#define LCD_X_RES		84
#define LCD_Y_RES		48
*/
