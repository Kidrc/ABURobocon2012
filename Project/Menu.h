#ifndef __MENU_H
#define __MENU_H

#include <stdlib.h>
#include "PSX.h"
#include "LCD_STD.h"
#include "GeneralDef.h"

typedef struct menu_node
{
	char name[5];
	struct menu_node * child;
	int child_Len;
	void (*Proceed)(void);
	void (*Add_Child)(char * name, void (*Function)(void));
	void (*Remove_Child)(int node);

} menu_Node;

typedef struct menu_tree
{
	menu_Node * root;
	menu_Node * current;

	void (*Init)(void);

} menu_Tree;

extern menu_Tree menu;


extern void Para_Config(void);

extern void Display_Child(void);

extern void Initialize(void);

extern void Add_Child(char * name, void (*Function)(void));
extern void Remove_Child(int node);

/*
*	在这里添加新的菜单函数原型(返回值和参数必须为void)和变量，在Menu.c里定义
*/
extern uint16_t speed_Y;
extern uint16_t speed_W;
extern uint16_t delay_Time;

extern void Speed_Config(void);
extern void Delay_Config(void);

#endif
