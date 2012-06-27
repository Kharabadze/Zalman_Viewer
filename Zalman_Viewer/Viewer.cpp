#include"Viewer.h"
#include<math.h>
#include<direct.h>
#include<stdio.h>
#include"exp_pictures.hpp"

Viewer::Viewer(void){
	redraw_me_please=false;
	prokrutka=0;
	return;
}

void Viewer::Invalidate(bool a){
	//SendMessage(hWnd,WM_PAINT,0,0);
	redraw_me_please=true;
	return;
}

void Viewer::OnLButtonDown(UINT nFlags,CPoint pt){
	int xx=pt.x,yy=my_gl.max_y-pt.y/2;
	if((menu_is_visible)&&(pt.y<84)){
		menu_mouse.x=pt.x;
		menu_mouse.y=pt.y;
		menu_is_clicked=true;
	}else if((current_file_num>=0)&&(xx>=this->mi_x)&&(xx<=this->ma_x)&&(yy>=this->mi_y)&&(yy<=this->ma_y)){
		is_move_big=true;
		//place_big_mov_x=place_big_mov_y=0;
		//big_prokr_x=
	}else if(pt.x<my_gl.max_x-16){
		is_movable=true;
		place_prokr=prokrutka*2;
	}else{
		is_scroll_movable=true;
		scroll_prokr=prokrutka*2*my_gl.max_y/total_height;
	}
	first_mouse.x=old_mouse.x=pt.x;
	first_mouse.y=old_mouse.y=pt.y;
	return;
}
void Viewer::OnLButtonUp(UINT nFlags,CPoint pt){
	is_movable=false;
	is_scroll_movable=false;
	is_move_big=false;
	//(old_mouse.x==pt.x)&&(old_mouse.y==pt.y)&&
	if ( (first_mouse.x==pt.x) && (first_mouse.y==pt.y) ){
		if ( (menu_is_clicked) || (menu_is_visible) && (pt.y<84) ){;
		}else{
			click_mouse.x=pt.x;
			click_mouse.y=pt.y;
		}
	}
	Invalidate(false);
	return;
}

void Viewer::OnMouseMove(UINT nFlags,CPoint pt){

	if((nFlags&MK_LBUTTON)==0){
		is_movable=false;
		is_scroll_movable=false;
	}

	CPoint new_mouse;
	new_mouse.x=pt.x;new_mouse.y=pt.y;
	//Menu enable
	if((menu_is_visible==false)&&(new_mouse.y<6)){
		menu_is_visible=true;
		Invalidate(false);
	}else if((menu_is_visible==true)&&(new_mouse.y>84)){
		menu_is_visible=false;
		Invalidate(false);
	}
	//Compare it
	if(((nFlags&MK_LBUTTON)==0)||
		(new_mouse.x==old_mouse.x)&&
		(new_mouse.y==old_mouse.y))
		return;

	//OnTimer(13);//D.K.

	if(is_move_big){
		place_big_mov_y-=new_mouse.y-old_mouse.y;
		place_big_mov_x+=new_mouse.x-old_mouse.x;
		old_mouse.x=new_mouse.x;old_mouse.y=new_mouse.y;
		Invalidate(false);
	}if(is_movable){
		place_prokr-=new_mouse.y-old_mouse.y;
		prokrutka=place_prokr/2;
		//prokrutka-=new_mouse.y-old_mouse.y;
		old_mouse.x=new_mouse.x;old_mouse.y=new_mouse.y;
		Invalidate(false);
	}else if(is_scroll_movable){
		scroll_prokr+=new_mouse.y-old_mouse.y;
		prokrutka=scroll_prokr*total_height/(2*my_gl.max_y);
		old_mouse.x=new_mouse.x;old_mouse.y=new_mouse.y;
		Invalidate(false);
	}
	return;
}

BOOL Viewer::OnMouseWheel(HWND *hWND,UINT nFlags,short zDelta, CPoint pt){
	if(zDelta==0)return(false);
	//--- Correction
	int qq=zDelta/120;
	tagPOINT pt_corr;//CPoint pt_corr;
	int vert_non;
	pt_corr.x=pt.x;pt_corr.y=pt.y;
	//LPPOINT a;a->x;
	ScreenToClient(*hWND,&pt_corr);//ClientToScreen(&main_point);
	vert_non=pt_corr.y;
	pt_corr.y=my_gl.max_y-pt_corr.y/2;

	//if((pt_corr.x>=0)&&(pt_corr.x<my_gl.max_x))
	if(1){
		if((current_file_num>=0)&&
			(pt_corr.x>=this->mi_x)&&(pt_corr.x<=this->ma_x)&&
			(pt_corr.y>=this->mi_y)&&(pt_corr.y<=this->ma_y)){
				//Zoom
				logarithm+=qq;
				float x_vec=pt_corr.x-my_gl.max_x/2-place_big_mov_x;
				float y_vec=-vert_non+my_gl.max_y-place_big_mov_y;
				float new_x_vec=pow(2.0,qq/12.0)*x_vec;
				float new_y_vec=pow(2.0,qq/12.0)*y_vec;
				place_big_mov_x=pt_corr.x-my_gl.max_x/2-new_x_vec;
				place_big_mov_y=-vert_non +my_gl.max_y  -new_y_vec;
		}else if((pt_corr.x>=my_gl.max_x-16)&&(pt_corr.x<my_gl.max_x)){
			prokrutka-=zDelta/2;//Scroll
		}else{
			prokrutka-=zDelta/5;
		}
	}else{
		return(false);
	}
	Invalidate(false);
	//Invalidate(true);
	return(false);
}

void Viewer::OnTimer(UINT_PTR nIDEvent){
	if(nIDEvent==13){

		one_file_info *cur=my_explorer.file_array;
		int i=0;
		for(int i=0;i<my_explorer.File_Number;i++){
			if(cur->empty){
				cur->get_information(true,temp_image,temp_image2);
				Invalidate(false);
				break;
			}
			cur=cur->next;
		}
	}
	return;
}
void Viewer::OnDraw(HWND *hWnd,HDC* pDC){
	//================================ OLD
	//CZalman_ViewerDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: add draw code for native data here

	//=== Взять список файлов
	if(!my_explorer.names_updated){
		my_explorer.get_folder();
	}

	//=== Взятие координат начальной точки
	POINT main_point;
	main_point.x=0;main_point.y=0;
	ClientToScreen(*hWnd,&main_point);
	//=== Взятие размера
	GetClientRect(*hWnd,&myrect);
	//=== Инициализация
	my_gl.init(main_point.x,main_point.y,myrect.right,myrect.bottom);
	int maxx,maxy;
	my_gl.getmax(maxx,maxy);

	//=== Коррекция названия файла
	//if(0)
	if(((current_file_num<0)||(current_file_num>=my_explorer.File_Number))&&(is_first_external_name)){
		//pDoc->DK_File_Name;
		my_explorer.file_cursor=my_explorer.file_array;
		for(int i=0;i<my_explorer.File_Number;i++){

			bool equal=true;
			int cn=0;
			while(1){
				WCHAR a=DK_File_Name[cn];
				WCHAR b=my_explorer.file_cursor->filename[cn];
				a=towlower(a);b=towlower(b);
				if(a!=b){
					equal=false;
					break;
				}else if(DK_File_Name[cn]==0){
					equal=true;
					break;
				}
				cn++;
			}

			if(equal){
				current_file_num=i;
				my_explorer.file_drawer=my_explorer.file_cursor;
				break;
			}
			my_explorer.file_cursor=my_explorer.file_cursor->next;
		}
	}
	is_first_external_name=false;

	//while(1)
	{// :)
		//=== Predate mouse
		click_mouse.y=(myrect.bottom-click_mouse.y)/2;
		//=== Обработка большой картинки
		if((current_file_num>=0)&&(current_file_num<my_explorer.File_Number)){
			if((click_mouse.x>=mi_x)&&(click_mouse.x<=ma_x)&&
				(click_mouse.y>=mi_y)&&(click_mouse.y<=ma_y)){
				current_file_num=-1;
				click_mouse.x=click_mouse.y=-1;//No mouse click
			}
		}
		//=== Коррекция перемотки
		int pict_size=192,prom=10,scroll=16;
		int font_size=34;
		//int hor=100;
		//while(prom+(pict_size+prom)*hor+scroll<maxx)hor--;
		int hor=(maxx-prom-scroll)/(pict_size+prom);
		if(hor<1)hor=1;
		int vn = (my_explorer.File_Number+hor-1)/hor;
		//===
		//total_height=( 10 + prom+(18+pict_size+prom)*vn)/2;
		total_height=( font_size + prom+(font_size+pict_size+prom)*vn)/2;
		if(prokrutka+maxy>total_height)prokrutka=(total_height-maxy);
		if(prokrutka<0)prokrutka=0;
		//===
		{//=== Выдать название каталога
			my_gl.set_color(0,0,255);
			//char bufer[256];//memset(bufer,0,sizeof(bufer));
			//int j=0;
			//while((j<255)&&(my_explorer.cur_fold[j])){
			//	bufer[j]=my_explorer.cur_fold[j];
			//	j++;
			//}bufer[j]=0;
			int iix=10;
			int iiy=-(font_size/2)+maxy+prokrutka;
			my_gl.goto_xyz(iix,iiy,0);
			//my_gl.dos_print(bufer);
			my_gl.win_print(my_explorer.cur_fold);
		}
		//=== Индикация файлов
		my_explorer.file_cursor=my_explorer.file_array;
		for(int i=0;i<my_explorer.File_Number;i++){
			int iy=i/hor;
			int ix=i%hor;
			int iix=prom+(pict_size+prom)*ix;
			int iiy=-font_size/2+maxy+prokrutka-(pict_size+font_size+prom)*(iy+1)/2;
			if((iiy+9+96>0)&&(iiy<maxy)){
				//if(my_explorer.file_cursor->empty){
				//	//--- Make description
				//	my_explorer.file_cursor->get_information();
				//}
				//--- Process Click
				if((click_mouse.x>=iix)&&(click_mouse.x<iix+192)&&
				   (click_mouse.y>=iiy)&&(click_mouse.y<=iiy+font_size/2+96)){
					   current_file_num=i;
					   my_explorer.file_drawer=my_explorer.file_cursor;
				}
				//--- Thumb
				if((iiy+font_size/2+96>ma_y)||(iiy<mi_y)||(iix+192>mi_x)||(iix<ma_x)){
					my_gl.goto_xyz(iix,iiy+font_size/2,0);
					my_gl.thumbnail(my_explorer.file_cursor->thumbnail);
					{//---text
						//if(i==current_file_num)
						//	my_gl.set_color(255,255,0);
						//else
						my_gl.set_color(0,255,255);
						//char bufer[256];//memset(bufer,0,sizeof(bufer));
						//int j=0;
						//while((j<12)&&(my_explorer.file_cursor->filename[j])){
						//	bufer[j]=my_explorer.file_cursor->filename[j];
						//	j++;
						//}bufer[j]=0;
						my_gl.goto_xyz(iix,iiy,0);
						//my_gl.dos_print(bufer);
						my_gl.win_print(my_explorer.file_cursor->filename,192);
					}
				}
			}
			my_explorer.file_cursor=my_explorer.file_cursor->next;
		}

		//=== рисовать ScrollBar
		my_gl.scroll_bar(prokrutka,total_height);
		//=== Update mouse
		click_mouse.x=click_mouse.y=-1;//No mouse click

//goto debug_return1;//DEBUG

//=== PROCESS FOLDER
		if((current_file_num>=0)&&(current_file_num<my_explorer.File_Number)){
			if(my_explorer.file_drawer)
			if(my_explorer.file_drawer->folder){
				//my_explorer.File_Number=-1;
				if(!wcscmp(my_explorer.file_drawer->filename,L"..")){
					int rl=wcslen(my_explorer.cur_fold);
					//WCHAR r=L'\';
					while(rl>=0){
						if(my_explorer.cur_fold[rl]=='\\')break;
						rl--;
					}
					if(rl>=0){
						my_explorer.cur_fold[rl]=0;rl--;
						if(my_explorer.cur_fold[rl]==L':'){
							my_explorer.cur_fold[++rl]=L'\\';
							my_explorer.cur_fold[++rl]=0;
						}
					}
					//_wsystem(L"cd ..");
					_wchdir(my_explorer.cur_fold);
				}else{
					_wchdir(my_explorer.file_drawer->filename);
				}
				my_explorer.clear_folder();
				current_file_num=-1;

				Invalidate(false);
			}
		}
		//break;
	}
//debug_return1://DEBUG


	//=== Get Big Picture
	//bool draw_me=false;
	if(current_file_num!=old_current_file_num){
		if((current_file_num>=0)&&(current_file_num<my_explorer.File_Number)){
			bool mono=my_explorer.file_drawer->mono;// :)

			place_big_mov_y=0;
			place_big_mov_x=0;

			//if(image )FreeImage_Unload(image  );//Later remove
			//if(image2)FreeImage_Unload(image2 );//Later remove
			my_explorer.file_drawer->get_information(false,image,image2);
			if(my_explorer.file_drawer==0){
				current_file_num=-1;//No picture
			}else if((my_explorer.file_drawer->bad_picture)||(image==0)){;
				current_file_num=-1;//No picture
			}else{
				fr_image_bufer = (BYTE*)FreeImage_GetBits(image);
				fr_image_pitch = FreeImage_GetPitch(image);
				if(image2){
					fr_image_bufer2 = (BYTE*)FreeImage_GetBits(image2);
					fr_image_pitch2 = FreeImage_GetPitch(image2);
				}else if(mono){
					fr_image_bufer2 = fr_image_bufer;
					fr_image_pitch2 = fr_image_pitch;
				}else{
					int shift = fr_image_pitch/6*3;
					fr_image_bufer2 = &fr_image_bufer[shift];
					fr_image_pitch2 = fr_image_pitch;
				}

				int x,in_x=my_explorer.file_drawer->im_w;
				int y,in_y=my_explorer.file_drawer->im_h;
				if(my_explorer.file_drawer->mono)in_x*=2;

				logarithm=1;
				x=in_x;y=in_y;
				while((x<my_gl.max_x)&&(y<my_gl.max_y)){
					logarithm++;
					float mnoj=pow(2.0,logarithm/12.0);
					x=in_x*mnoj;
					y=in_y*mnoj;
				}
				while((x>my_gl.max_x)||(y>my_gl.max_y)){
					logarithm--;
					float mnoj=pow(2.0,logarithm/12.0);
					x=in_x*mnoj;
					y=in_y*mnoj;
				}
			}
		}
	}
	old_current_file_num=current_file_num;

//goto debug_return;//DEBUG

	if((my_explorer.file_drawer)&&(current_file_num>=0)&&(current_file_num<my_explorer.File_Number)){//--- Draw picture...
		//int in_x=my_explorer.file_drawer->im_w;
		//int in_y=my_explorer.file_drawer->im_h;
		//int x=in_x<<(logarithm/12);
		//int	y=in_y<<(logarithm/12);
		//--- Center
		int c_x,c_y;
		my_gl.getmax(c_x,c_y);
		c_x/=2;c_y/=2;
		c_x+=place_big_mov_x;
		c_y+=place_big_mov_y/2;

		int s_x=my_explorer.file_drawer->im_w;
		int s_y=my_explorer.file_drawer->im_h;
		//int r_x=0,r_y=0,l_x=s_x/2,l_y=0;
		//x*=oktava_tab[logarithm%12];
		//y*=oktava_tab[logarithm%12];

		bool mono=my_explorer.file_drawer->mono;
		int temp_s_x=(mono)?2*s_x:s_x;
		//int temp_l_x=(mono)?0:l_x;
		my_gl.draw_big_picture(
			fr_image_bufer,fr_image_pitch,
			fr_image_bufer2,fr_image_pitch2,
			temp_s_x,s_y,
			//r_x,r_y,temp_l_x,l_y,
			c_x,c_y,
			logarithm,pow(2.0,logarithm/12.0),
			FI_RGBA_RED,FI_RGBA_GREEN,FI_RGBA_BLUE,
			mi_x,ma_x,mi_y,ma_y);
		//my_gl.draw_big_picture(
		//	0,0,in_x,in_y,(1<<(logarithm/12))*oktava_tab[logarithm%12],
		//	fr_image_bufer,fr_image_pitch,
		//	FI_RGBA_RED,FI_RGBA_GREEN,FI_RGBA_BLUE);
	}
debug_return:

	if(menu_is_visible){
		my_gl.set_color(255,127,255);
		my_gl.goto_xy(0,my_gl.max_y-42);
		//my_gl.rectangle(my_gl.max_x-1,my_gl.max_y-1);
		my_gl.smooth(my_gl.max_x-1,my_gl.max_y-1);

		int x_position=10;
		bool disks=true;
		char disk_num='A';
		while(1){
			if(disks){
				char bufer[4]={disk_num,':','\\',0};
				WCHAR wbufer[4]={disk_num,':','\\',0};
				int tp=GetDriveType(wbufer);
				if(tp!=DRIVE_NO_ROOT_DIR){
					my_gl.goto_xy(x_position,my_gl.max_y-32);
					unsigned char *cp=IM_UNKNOWN;
					if(tp==DRIVE_UNKNOWN) cp=IM_UNKNOWN;
					if(tp==DRIVE_REMOVABLE) cp=IM_REMOVABLE;
					if(tp==DRIVE_FIXED) cp=IM_FIXED;
					if(tp==DRIVE_REMOTE) cp=IM_REMOTE;
					if(tp==DRIVE_CDROM) cp=IM_CDROM;
					if(tp==DRIVE_RAMDISK) cp=IM_RAMDISK;
					my_gl.draw_mono_pic(cp,64,32);
					//my_gl.draw_mono_pic(IM_UNKNOWN,64,32);
					//my_gl.draw_mono_pic(IM_REMOVABLE,64,32);
					//my_gl.draw_mono_pic(IM_FIXED,64,32);
					//my_gl.draw_mono_pic(IM_REMOTE,64,32);
					//my_gl.draw_mono_pic(IM_CDROM,64,32);
					//my_gl.draw_mono_pic(IM_RAMDISK,64,32);
					my_gl.goto_xy(x_position+8,my_gl.max_y-40);
					my_gl.dos_print(bufer);
					//=== Process click
					if(menu_is_clicked){
						if((menu_mouse.x>=x_position)&&(menu_mouse.x<x_position+64)){
							if(_chdrive(disk_num-'A'+1)==0){
								my_explorer.clear_folder();
								current_file_num=-1;
								Invalidate(false);
							}
						}
					}
					//=== Update
					x_position+=64+8;
				}
			}else{
				int sm=0;
				char bufer[40];
				my_gl.goto_xy(x_position,my_gl.max_y-32);
				unsigned char *cp=IM_UNKNOWN;
				if(disk_num==0){
					cp=IM_Prev;
					sprintf(bufer,"Prev");sm=0;
				}else if(disk_num==1){
					cp=IM_Next;
					sprintf(bufer,"Next");sm=0;
				}else if(disk_num==2){
					cp=IM_Help;
					sprintf(bufer,"Help");sm=0;
				}else if(disk_num==3){
					cp=IM_Web;
					sprintf(bufer,"Web");sm=8;
				}
				my_gl.draw_mono_pic(cp,64,32);
				my_gl.goto_xy(x_position+sm,my_gl.max_y-40);
				my_gl.dos_print(bufer);
				//=== Process click
				if(menu_is_clicked){
					if((menu_mouse.x>=x_position)&&(menu_mouse.x<x_position+64)){
						if(disk_num==0){
							menu_is_clicked=false;// !!!
							my_explorer.file_cursor=my_explorer.file_array;
							if((current_file_num<0)||(current_file_num>=my_explorer.File_Number)){;
							}else{
								int last=-1;
								one_file_info *q_tek=0;
								for(int yy=0;yy<my_explorer.File_Number;yy++){
									if(!my_explorer.file_cursor->folder){
										if(yy==current_file_num){
											current_file_num=last;
											my_explorer.file_drawer=q_tek;
											Invalidate(false);
											break;
										}
										last=yy;
										q_tek=my_explorer.file_cursor;
									}
									my_explorer.file_cursor=my_explorer.file_cursor->next;
								}
							}
							Invalidate(false);
						}else if(disk_num==1){
							menu_is_clicked=false;// !!!
							my_explorer.file_cursor=my_explorer.file_array;
							if((current_file_num<0)||(current_file_num>=my_explorer.File_Number)){;
							}else{
								bool good=false,ex_x=false;
								for(int yy=0;yy<my_explorer.File_Number;yy++){
									if(!my_explorer.file_cursor->folder){
										if(good){
											current_file_num=yy;
											my_explorer.file_drawer=my_explorer.file_cursor;
											Invalidate(false);
											ex_x=true;
											break;
										}else{
											if(yy==current_file_num)good=true;
										}
									}
									my_explorer.file_cursor=my_explorer.file_cursor->next;
								}
								if(!ex_x){
									current_file_num=-1;
									my_explorer.file_drawer=0;
								}
							}
							Invalidate(false);
						}else if(disk_num==2){
							menu_is_clicked=false;// !!!
							#define MY_MAX_PATH 4096
							WCHAR help_buffer[MY_MAX_PATH];
							GetModuleFileName(NULL,help_buffer,MAX_PATH);
							int len=0;
							while(help_buffer[len]!=0)len++;
							while(1){
								if(len==0)break;
								if(help_buffer[len-1]=='\\')break;
								if(help_buffer[len-1]==':')break;
								len--;
							}
							WCHAR help[18]=L"help.html";
							for(int i=0;i<10;i++)help_buffer[len++]=help[i];
							HWND Handle=NULL;//Handle=this->hwn
							//ShellExecute(Handle, L"open", L"help\\main.html", 0, 0, SW_SHOWMAXIMIZED);
							//ShellExecute(Handle, L"open", L"help.html", 0, 0, SW_SHOWMAXIMIZED);
							ShellExecute(Handle, L"open", help_buffer, 0, 0, SW_SHOWMAXIMIZED);
							#undef MY_MAX_PATH
						}else if(disk_num==3){
							menu_is_clicked=false;// !!!
							HWND Handle=NULL;//Handle=this->hwn
							ShellExecute(Handle, L"open", L"http://jps.hut2.ru/zalman_viewer_en.html", 0, 0, SW_SHOWMAXIMIZED);
						}
					}
				}
				//=== Update
				x_position+=64+12;
			}

			//=== Update
			disk_num++;
			if(disks){
				if(disk_num>'Z'){disks=false;disk_num=0;}
			}else{
				if(disk_num>3)break;
			}
		}
		my_gl.goto_xy(x_position+20,my_gl.max_y-10);
		my_gl.set_color(127,127,64);
#ifdef _M_X64
			my_gl.dos_print("Zalman_Viewer v.2.5 x64\nby David Kharabadze\nprogram is free!\nwww.jps.hut2.ru");
#else
			my_gl.dos_print("Zalman_Viewer v.2.5 x86\nby David Kharabadze\nprogram is free!\nwww.jps.hut2.ru");
#endif

		//--- Disable click!!!
		menu_is_clicked=false;// !!!
	}

	//{//=== Debug
	//	char bufer[1024];
	//	sprintf(bufer,"x=%i,y=%i,l=%i",place_big_mov_x,place_big_mov_y,logarithm);
	//	my_gl.goto_xy(0,0);
	//	my_gl.set_color(255,0,0);
	//	my_gl.dos_print(bufer);
	//}


	//=== Вывод
	my_gl.output(pDC);
	//=== Стандартная библиотека
	//pDC->TextOutW(0,0,L"My Viewer");
}
