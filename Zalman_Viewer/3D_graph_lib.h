#pragma once
#include<Windows.h>
struct graph_lib_3D{
	bool skip_line;
	int rx,ry;//Real x,y
	int max_x,max_y;//Maximum x,y
	int max_size;
	unsigned int *image_bufer;
	//-----------------------------------
	int cur_x,cur_y,cur_s;
	int cur_color;
	//-----------------------------------
	void goto_xy(int x,int y);
	void goto_xyz(int x,int y,int smesh);
	void set_color(int red,int green,int blue);
	void putpixel();
	void rectangle(int x,int y);
	void thumbnail(unsigned char *thumb);
	void scroll_bar(int current,int maximum);
	//-----------------------------------
	void print(int x,int y,wchar_t *string,int parameters);
	void dos_char(char c);
	void dos_print(char *string);
	bool win_char (wchar_t c);
	void win_print(wchar_t *string,int maxlen=0);
	//-----------------------------------
	void smooth(int x,int y);
	void draw_mono_pic(unsigned char *picture,int x_size,int y_size);
	//void draw_small_pic(
	//		BYTE *inp_picture,int inp_pitch,
	//		BYTE *inp_picture2,int inp_pitch2,
	//		int x_size,int y_size);
	void draw_big_picture(
			BYTE *inp_picture,int inp_pitch,
			BYTE *inp_picture2,int inp_pitch2,
			int s_x,int s_y,
			//int r_x,int r_y,int l_x,int l_y,
			int c_x,int c_y,
			int lgg,float mnoj,
			int r,int g,int b,
			int &mi_x,int &ma_x,int &mi_y,int &ma_y);

	//-----------------------------------
	void init(int x0,int y0,int x1,int y1);
	void getmax(int&x,int&y);
	void output(HDC* pDC);
	graph_lib_3D(void);
	~graph_lib_3D(void);
};