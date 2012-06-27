#pragma once
#include"3D_graph_lib.h"

#define B_MSG_CLICK			0x01001
#define B_MSG_BUT_DOWN		0x01002
#define B_MSG_BUT_UP		0x01003
#define B_MSG_MOUSE_MOVE	0x01004
#define B_MSG_ADD			0x01005
#define B_MSG_REM			0x01006

struct B_Object{
	B_Object *parent,*son,*brother;
	//---
	bool visible;
	int x_size,y_size,z_size;
	virtual bool is_inside(int x,int y);//
	virtual void on_click(int x,int y);
	virtual void paint(graph_lib_3D *my_gl,int x,int y);
	B_Object(void);
};

struct B_Box{
	//int x_size,y_size;
};

struct B_App:B_Object{
	int count;
	//void add(B_Object *a);
};

struct B_Scroller:B_Object{
	int percentage;
};

//struct