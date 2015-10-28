#include "LCD_STD.h"

void LCD_Init(void)
{
	//���������LCD��Ļ�ĳ�ʼ������
	RCC_GPIOG();

	GPIO_Config(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOG);

	OLED_Init();

	LCD_Write(0, 0, "LCD");
}
void LCD_Clear(void)
{
	//���������LCD��Ļ���������
	OLED_Clear();
}

void LCD_Write(int x, int y, char * str)
{
	//���������LCD��Ļ����ʾ����
	OLED_ShowString(x*8, y*16, str);
	OLED_Refresh_Gram();
}

void LCD_Write_Int(int x, int y, int n)
{
	char buffer[15];

	itoa(buffer, n, 1);

	LCD_Write(x, y, buffer);
}

void LCD_Inform(char * str)
{
	LCD_Clear();
	LCD_Write(0, 0, str);
}