#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cmath>

#include"Viewer.h"

Viewer mv;
LRESULT CALLBACK WindowProc (HWND, UINT, WPARAM, LPARAM);
HDC dc;


//char szProgName[]="Zalman_Viewer";
//char szIconName[]="IDI_ICON1";


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	// Create window
	WNDCLASS wc = {0};
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	//wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hCursor = LoadCursor (NULL, IDC_HAND);
	wc.lpszClassName= L"CMyWnd";
	//wc.hIcon=LoadIcon(hInstance,L"IDI_ICON1");//LOAD NEW ICON
	//wc.hIcon=LoadIcon(NULL,IDI_HAND);//LOAD NEW ICON
	//wc.hIcon=LoadIcon(NULL,L"IDI_ICON1");//LOAD NEW ICON
	wc.hIcon=LoadIcon(NULL,L"IDI_ICON1");//LOAD NEW ICON
	//wc.hIcon=LoadIconA(0,"IDI_ICON1");

	RegisterClass (&wc);
	
	HWND hWnd = CreateWindow(L"CMyWnd", L"Zalman_Viewer",
		WS_OVERLAPPEDWINDOW ,
		CW_USEDEFAULT , CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	
	

	dc = GetDC (hWnd);
	//ShowWindow (hWnd, nCmdShow);
	ShowWindow(hWnd, nCmdShow|SW_MAXIMIZE);
	
	//--- Get pan
	//mp.load(L"DAV_PICTURES\\ROOM");
	// Message loop (timer, etc)
	//SetTimer (hWnd, 1, USER_TIMER_MINIMUM, NULL);
	//SetTimer (hWnd, 1, 50, NULL);
	//SetTimer (hWnd, 13, 50, NULL);
	SetTimer (hWnd, 13, 10, NULL);
	MSG msg;
	while (GetMessage(&msg,NULL,0,0) > 0)// while not WM_QUIT (0) nor some error (-1)
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return 0;//msg.wParam;
}
// Message processing function
LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	static bool Move = true;
	static int Phase=0, Width, Height;
	
	//--- Move
	CPoint pt;
	pt.x = lParam&0xffff;
	pt.y = (lParam>>16)&0xffff;
	UINT nFlags=wParam & 0xffff;
	short zDelta =(wParam>>16) & 0xffff;
	//--- Timer
	UINT_PTR nIDEvent=wParam;

	switch (message){
		case WM_MOUSEMOVE:
			mv.OnMouseMove(nFlags,pt);
			break;

		case WM_LBUTTONDOWN:
			mv.OnLButtonDown(nFlags,pt);
			//Move = !Move;
			break;

		case WM_LBUTTONUP:
			mv.OnLButtonUp(nFlags,pt);
			break;

		case WM_MOUSEWHEEL:
			//nFlags&=0xffff;
			mv.OnMouseWheel(&hWnd,nFlags,zDelta,pt);
			break;

		case WM_MOVE:
			mv.Invalidate(false);
			break;

		case WM_CREATE:
			break;

		case WM_TIMER:
			mv.OnTimer(nIDEvent);
			break;

		case WM_PAINT:
			mv.Invalidate(false);
			//mv.OnDraw(&hWnd,&dc);
			//mv.redraw_me_please=false;
			//mv.OnDraw(&hWnd,&dc);
			break;

		case WM_SIZE:
			Width = LOWORD(lParam),
			Height = HIWORD(lParam);
			mv.Invalidate(false);
			break;

		case WM_KEYDOWN:
			if (wParam != VK_ESCAPE)
				break;
			// else no break
		case WM_DESTROY:
			PostQuitMessage (0);
	}
	//--- Add redraw
	if(mv.redraw_me_please){
		mv.redraw_me_please=false;
		mv.OnDraw(&hWnd,&dc);
		//SendMessage(hWnd,WM_PAINT,0,0);
	}

	return DefWindowProc (hWnd, message, wParam, lParam);
}