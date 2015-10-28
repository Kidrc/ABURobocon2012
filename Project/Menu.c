#include "Menu.h"
/*
*	文件包含调试菜单的数据结构定义和调试函数。
*	该文件实现调节速度和Delay时间的功能。
*
*	2011/11/28 22:56
*	成功测试调试功能，修改速度。
*	Version 1.2
*
*	2011/11/25 21:29
*	将文件改为单片机版本，使用LCD5110.
*	Version 1.1
*
*	2011/11/25 6:24
*	Version 1.0
*/
const int MENU_BACK = -1;
const int MAX_LEN = 3;

menu_Tree menu = {0, 0, Initialize};

uint16_t speed_Y = 400;
uint16_t speed_W = 400;
uint16_t delay_Time = 50;

void Para_Config(void)
{
	menu.Init();

	menu.root->Proceed();

	LCD_clear();

	LCD_write_String(0, 0, "Accepted.");

	LCD_write_String(0, 2, "Running...");
}
void Display_Child(void)
{
	void Display(menu_Node * current);

	Display(menu.current);
}

void Display(menu_Node * current)
{
	int i;
	int choice = 0;

	do
	{
		for (i = 0; i < 6; i++)
		{
			LCD_write_String(0, i, "              ");
		}

		for (i = 0; i < current->child_Len; i++)
		{
			LCD_write_String(0, i * 2, current->child[i].name);
		}

	//	printf("Enter your choice:\n");

	//	scanf("%d",&choice);

		do
		{
			LCD_write_String(0, 2 * choice + 1, "^");

			//Delay(5000);

			if (Press(R2))
			{
				//Delay(500);

				LCD_write_String(0, 2 * choice + 1, " ");

				choice = MENU_BACK;
			}
			else if (Press(DOWN))
			{
				//Delay(500);

				LCD_write_String(0, 2 * choice + 1, " ");

				choice = (choice + 1) % current->child_Len;
			}
			else if (Press(UP))
			{
				//Delay(500);

				LCD_write_String(0, 2 * choice + 1, " ");

				choice = (choice + current->child_Len - 1) % current->child_Len;
			}

			Delay(700);

		}while(choice != MENU_BACK && !Press(R1));

		if (choice != MENU_BACK && choice < current->child_Len)
		{
			menu.current = current->child + choice;

			menu.current->Proceed();
		}
		/*else if (choice == 6)
		{
			printf("Enter the name:\n");
	//		scanf("%s",name);

			current->Add_Child(name, Display_Child);
		} */
	}while(choice != MENU_BACK);
}

void Initialize(void)
{
	menu.root = (menu_Node*)malloc(sizeof(menu_Node));

	menu.root->child = (menu_Node*)malloc(sizeof(menu_Node)*MAX_LEN);
	menu.root->child_Len = 0;
	menu.root->Proceed = Display_Child;
	menu.root->Add_Child = Add_Child;
	menu.root->Remove_Child = Remove_Child;

	menu.current = menu.root;

	menu.current->Add_Child("Config Vy",Speed_Config);
	menu.current->Add_Child("Config w", Delay_Config);

	menu.current = menu.root; 
}

void Add_Child(char * name, void (*Function)(void))
{
	int i;

	menu.current->child[menu.current->child_Len].child = (menu_Node*)malloc(sizeof(menu_Node) * MAX_LEN);
	menu.current->child[menu.current->child_Len].child_Len = 0;

	for (i = 0; name[i]; i++)
	{
		menu.current->child[menu.current->child_Len].name[i] = name[i];
	}
	menu.current->child[menu.current->child_Len].name[i] = 0;

	menu.current->child[menu.current->child_Len].Proceed = Function;
	menu.current->child[menu.current->child_Len].Add_Child = Add_Child;
	menu.current->child[menu.current->child_Len].Remove_Child = Remove_Child;

	++(menu.current->child_Len);
}

void Remove_Child(int node)
{
	int i;
	int node_Temp = node - 1;
	menu_Node * current_Temp = menu.current;

	if ((menu.current->child)[node_Temp].child_Len == 0)
	{
		free(menu.current->child[node_Temp].child);

		for (i = node_Temp + 1; i < menu.current->child_Len; i++)
		{
			(menu.current->child)[i-1] = (menu.current->child)[i];
		}
	}
	else
	{	
		menu.current = menu.current->child + node_Temp;

		for (i = 0; i < (menu.current->child)[node_Temp].child_Len; i++)
		{
			Remove_Child(i);
		}

		menu.current = current_Temp;
	}
	--menu.current->child_Len;
}

void Speed_Config(void)
{
	char buffer[6];
	uint8_t i = 4;
	uint8_t exit = 0;

	itoa(buffer, speed_Y, 5);

	LCD_clear();

	LCD_write_String(0, 0, "New Vy:");
	LCD_write_String(0, 1, buffer);
	LCD_write_String(24, 2, "^");

	Delay(1000);
	while(exit == 0)
	{
		if (Press(R1))
		{
			exit = 1;
		}
		else if (Press(R2))
		{
			exit = 2;
		}
		else if (Press(TRNGL))
		{
			if (buffer[i] == '9')
			{
				buffer[i] = '0';
			}
			else
			{
				++buffer[i];
			}

			LCD_write_String(0, 1, buffer);
		}
		else if (Press(CRSS))
		{
			if (buffer[i] == '0')
			{
				buffer[i] = '9';
			}
			else
			{
				--buffer[i];
			}

			LCD_write_String(0, 1, buffer);
		}
		else if (Press(SQR))
		{
			LCD_write_String(i * 6, 2, " ");

			if (i == 0)
			{
				i = 4;
			}
			else
			{
				--i;
			}

			LCD_write_String(i * 6, 2, "^");
		}
		else if (Press(CRCL))
		{
			LCD_write_String(i * 6, 2, " ");
			
			if (i == 4)
			{
				i = 0;
			}
			else
			{
				++i;
			}

			LCD_write_String(i * 6, 2, "^");
		}

		Delay(600);
	}

	LCD_clear();
	
	if (exit == 2)
	{
		LCD_write_String(0, 0, "Canceled.");
		Delay(3000);
	}
	else 
	{
		speed_Y = atoi(buffer);

		LCD_write_String(0, 0, "Saved.");
		Delay(3000);
	}				
}

void Delay_Config(void)
{
	char buffer[6];
	uint8_t i = 4;
	uint8_t exit = 0;

	itoa(buffer, speed_W, 5);

	LCD_clear();

	LCD_write_String(0, 0, "New delay_Time:");
	LCD_write_String(0, 1, buffer);
	LCD_write_String(24, 2, "^");

	Delay(1000);
	while(exit == 0)
	{
		if (Press(R1))
		{
			exit = 1;
		}
		else if (Press(R2))
		{
			exit = 2;
		}
		else if (Press(TRNGL))
		{
			if (buffer[i] == '9')
			{
				buffer[i] = '0';
			}
			else
			{
				++buffer[i];
			}

			LCD_write_String(0, 1, buffer);
		}
		else if (Press(CRSS))
		{
			if (buffer[i] == '0')
			{
				buffer[i] = '9';
			}
			else
			{
				--buffer[i];
			}

			LCD_write_String(0, 1, buffer);
		}
		else if (Press(SQR))
		{
			LCD_write_String(i * 6, 2, " ");

			if (i == 0)
			{
				i = 4;
			}
			else
			{
				--i;
			}

			LCD_write_String(i * 6, 2, "^");
		}
		else if (Press(CRCL))
		{
			LCD_write_String(i * 6, 2, " ");
			
			if (i == 4)
			{
				i = 0;
			}
			else
			{
				++i;
			}

			LCD_write_String(i * 6, 2, "^");
		}

		Delay(600);
	}

	LCD_clear();
	
	if (exit == 2)
	{
		LCD_write_String(0, 0, "Canceled.");
		Delay(3000);
	}
	else 
	{
		speed_W = atoi(buffer);

		LCD_write_String(0, 0, "Saved.");
		Delay(3000);
	}				
}
