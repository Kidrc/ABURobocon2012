#include "PSX.h"
#include "Navigate.h"
#include "Action.h"

#define IS_BLACK 0

extern uint16_t rolls;
char autoDisable = 0;
char dataID = 0;
char data_EN = 0;

uint8_t data[9];
const uint8_t PSX_CONN[5] = {0x01,0x43,0x00,0x01,0x00};
const uint8_t PSX_MODE[9] = {0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};
const uint8_t PSX_EXIT[9] = {0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
const uint8_t PSX_POLL[9] = {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int stop_Area = 7;

button_Def button;

void PSX_Init(uint16_t PSX_Scan_Period)
{
	SPI_InitTypeDef SPI_Structure;
	NVIC_InitTypeDef NVIC_Structure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	LCD_Clear();
	LCD_Write(0, 0, "PSX");

	RCC_GPIOB();
	RCC_SPI2();
	RCC_TIM6();

	GPIO_SPI2_Init();

	SPI_Structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_Structure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_Structure.SPI_CPOL = SPI_CPOL_High;
	SPI_Structure.SPI_CRCPolynomial = 7;
	SPI_Structure.SPI_DataSize = SPI_DataSize_8b;
	SPI_Structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Structure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_Structure.SPI_Mode = SPI_Mode_Master;
	SPI_Structure.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI2, &SPI_Structure);
	SPI_Cmd(SPI2, ENABLE);

	Set_Ana();
		
	NVIC_Structure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Structure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_Init(&NVIC_Structure);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		  	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = PSX_Scan_Period - 1;							 //ARR寄存器置counter，计数器计数到ARR时在下一拍置零
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;						 //分频
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);					//应用时基设置到TIM2
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);	
}

void Set_Ana(void)
{
	uint8_t PSX_Ana = 0x00;
	uint8_t i;
	
	while(PSX_Ana != 0xF3)
	{
	    ATT_Low();

		for (i = 0; i <= 4; i++)
		{ 
			SPI_I2S_SendData(SPI2, PSX_CONN[i]); 
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
			SPI_I2S_ReceiveData(SPI2);
		}
		 
		ATT_High();
		Delay(20);
		ATT_Low();

		for (i = 0; i <= 8; i++)
		{ 
			SPI_I2S_SendData(SPI2, PSX_MODE[i]); 
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
			SPI_I2S_ReceiveData(SPI2);
		}

 	   	ATT_High();
		Delay(20);
		ATT_Low();

		for (i = 0; i <= 8; i++)
		{ 
			SPI_I2S_SendData(SPI2, PSX_POLL[i]); 
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
			SPI_I2S_ReceiveData(SPI2);
		}

		ATT_High();
		Delay(20);
		ATT_Low();

		for (i = 0; i <= 8; ++i)
		{
			SPI_I2S_SendData(SPI2, PSX_EXIT[i]); 
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 

			data[i] = SPI_I2S_ReceiveData(SPI2);
		}
			
		PSX_Ana = data[1+IS_BLACK];

		ATT_High();
		Delay(20);
	}
}
uint8_t Press(uint8_t button)
{
 	switch (button)
	{
		case SLCT:
			return (data[3+IS_BLACK] & 0x01) == 0x00;
		
		case JOYL:
			return (data[3+IS_BLACK] & 0x02) == 0x00;
			
		case JOYR:
			return (data[3+IS_BLACK] & 0x04) == 0x00;
				
		case STRT:
			return (data[3+IS_BLACK] & 0x08) == 0x00;
			
		case UP:
			return (data[3+IS_BLACK] & 0x10) == 0x00;
		
		case RGHT:
			return (data[3+IS_BLACK] & 0x20) == 0x00;
				
		case DOWN:
			return (data[3+IS_BLACK] & 0x40) == 0x00;
			
		case LEFT:
			return (data[3+IS_BLACK] & 0x80) == 0x00;
			
		case L2:
			return (data[4+IS_BLACK] & 0x01) == 0x00;
			
		case R2:
			return (data[4+IS_BLACK] & 0x02) == 0x00;
			
		case L1:
			return (data[4+IS_BLACK] & 0x04) == 0x00;
			
		case R1:
			return (data[4+IS_BLACK] & 0x08) == 0x00;
			
		case TRNGL:
			return (data[4+IS_BLACK] & 0x10) == 0x00;
			
		case CRCL:
			return (data[4+IS_BLACK] & 0x20) == 0x00;
			
		case CRSS:
			return (data[4+IS_BLACK] & 0x40) == 0x00;
			
		case SQR:
			return (data[4+IS_BLACK] & 0x80) == 0x00;
		default:
			return 0;
	}
}
			
uint8_t Is_Cmd(void)
{
	return !(data[4+IS_BLACK] == 0xFF && data[3+IS_BLACK] == 0xFF && data[5+IS_BLACK] <= 0x7F+stop_Area && data[5+IS_BLACK] >= 0x7F-stop_Area &&
													data[7+IS_BLACK] <= 0x7F+stop_Area && data[7+IS_BLACK] >= 0x7F-stop_Area &&
													data[8+IS_BLACK] <= 0x7F+stop_Area && data[8+IS_BLACK] >= 0x7F-stop_Area &&
													data[6+IS_BLACK] <= 0x7F+stop_Area && data[6+IS_BLACK] >= 0x7F-stop_Area);
}			
void TIM6_IRQHandler(void)
{
	int i;

	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

	ATT_Low(); 

	for (i = 0; i <= 8; i++)
	{
		SPI_I2S_SendData(SPI2, PSX_POLL[i]); 
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
		data[i] = SPI_I2S_ReceiveData(SPI2);
	}	

   	ATT_High();

	dataID = !dataID;

	data_EN = Data_Valid();

	if (Press(TRNGL))
	{ 
		autoDisable = 1;
		ELVTR_Manual(ELVTR_ENABLE);
		EXTI_GenerateSWInterrupt(EXTI_Line0);
	}

	if (Press(CRSS))
	{
		LCD_Clear();
		LCD_Write_Int(0, 0, crd_A);
		LCD_Write_Int(0, 1, crd_X);
		LCD_Write_Int(0, 2, crd_Y);
		LCD_Write_Int(0, 3, seconds);
		LCD_Write_Int(6, 3, cur_Part);
		LCD_Write_Int(6, 1, rolls);
		LCD_Write_Int(6, 2, Read_IL());
	}

	ELVTR_Check();
}

char Data_Valid(void)
{
	static char lastData = 0;

	if (data[5] == lastData)
	{
		return 1;
	}
	else
	{
		lastData = data[5];

		return 0;
	}
}
	