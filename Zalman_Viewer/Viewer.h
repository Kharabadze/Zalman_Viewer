#pragma once
#include<Windows.h>
#include<cstring>
#include"3D_graph_lib.h"
#include"my_expl.h"

struct CPoint{
	int x,y;
};
struct Viewer{
	bool redraw_me_please;
	void Invalidate(bool a);
	//--
	void OnLButtonDown(UINT nFlags,CPoint pt);
	void OnLButtonUp(UINT nFlags,CPoint pt);
	void OnMouseMove(UINT nFlags,CPoint pt);
	BOOL OnMouseWheel(HWND *hWND,UINT nFlags,short zDelta, CPoint pt);
	void OnTimer(UINT_PTR nIDEvent);
	void OnDraw(HWND *hWnd,HDC* pDC);
	//--- &
	wchar_t DK_File_Name[1024];//D.K.

	//=========== Start D.K.
	bool is_first_external_name;
	//bool redraw_ASAP;
	RECT myrect;//Size of window
	graph_lib_3D my_gl;
	my_expl my_explorer;
	int total_height;
	int prokrutka;
	int place_prokr,scroll_prokr;
	int current_file_num,old_current_file_num;
	//---
	bool is_movable,is_scroll_movable;
	CPoint old_mouse;
	CPoint first_mouse;
	//--- Click
	CPoint click_mouse;
	FIBITMAP *image,*image2,*temp_image,*temp_image2;//D.K.: *image2
	BYTE *fr_image_bufer,*fr_image_bufer2;
	int fr_image_pitch,fr_image_pitch2;
	int logarithm;
	float oktava_tab[12];
	//--- proc
	//afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta, CPoint pt);
	//afx_msg void OnLButtonDown(UINT nFlags,CPoint pt);
	//afx_msg void OnLButtonUp(UINT nFlags,CPoint pt);
	//afx_msg void OnMouseMove(UINT nFlags,CPoint pt);
	//--- Timer
	UINT_PTR m_nTimer;//D.K.
	int OnCreate(LPCREATESTRUCT lpcs);//D.K.
	//void OnTimer(UINT_PTR nIDEvent);//D.K.
	//--- Big picture
	int mi_x,ma_x,mi_y,ma_y;//D.K.
	bool is_move_big;
	int place_big_mov_x,place_big_mov_y;
	//--- Menu
	bool menu_is_visible;
	CPoint menu_mouse;
	bool menu_is_clicked;
	//=========== Finish D.K.
	Viewer(void);

};