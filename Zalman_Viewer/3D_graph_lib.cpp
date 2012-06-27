//#include"stdafx.h"

#include"3D_graph_lib.h"
#include<memory.h>

#include"font_dos.hpp"//Font
#include"font_win.h"//Unicode font

#define LIMIT(arg1,arg_min,arg_max) (__max(arg_min,__min(arg_max,arg1)))

void graph_lib_3D::print(int x,int y,WCHAR *string,int parameters){
	//{
    //BITMAPINFO bm = { sizeof(BITMAPINFOHEADER), PX, PY, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
    //HBITMAP pic = CreateDIBSection( GetDC(win), &bm, DIB_RGB_COLORS, (void**)&picture, 0,0 );
    //picDC = CreateCompatibleDC( GetDC(win) ); 
    //SelectObject( picDC, pic );
  //}
	//fread( picture, 4,PX*PY, f );
	return;
}
void graph_lib_3D::dos_char(char c){
	unsigned char tem=c;
	for(int yy=0;yy<8;yy++){
		unsigned __int16 temp=fontik[tem][yy];
		int ny=cur_y+yy;
		if((ny>=0)&&(ny<max_y))
			for(int xx=0;xx<16;xx++){
				for(int p=0;p<2;p++){
					int nx=cur_x+xx+p*cur_s;
					if((temp&1)&&(nx>=0)&&(nx<max_x))
					image_bufer[(1+p+ny*2)*max_x+nx]=cur_color;
					//image_bufer[(2+ny*2)*max_x+cur_x+xx+cur_s]=cur_color;
				}
				temp>>=1;
			}
	}
}
bool graph_lib_3D::win_char (WCHAR c){
	//if(c<128)dos_char(c);
	//else dos_char('?');
	int w=my_font_width[c];
	int ppo=my_font_pointer[c];
	for(int p=0;p<2;p++){
		for(int i=0;i<w;i++){
			unsigned __int16 b=my_font_ligatura[ppo+i];
			int nx=cur_x+i+p*cur_s;
			if((nx>=0)&&(nx<max_x))
			for(int j=0;j<16;j++){
				if(((b>>j)&1)!=0){
						int ny=cur_y+j;
						if((ny>=0)&&(ny<max_y))
						image_bufer[(1+p+ny*2)*max_x+nx]=cur_color;
				}
			}
		}
	}
	return true;
}
void graph_lib_3D::dos_print(char *string){
	int len=strlen(string);
	int old_c=0;
	int main_x=cur_x;//,main_y=cur_y;
	for(int i=0;i<len;i++){
		char c=string[i];
		if((c==13)||(c==10)){
			if((c==old_c)||(old_c==0)){
				//Enter
				cur_y-=9;
				cur_x=main_x;
				old_c=c;
			}else{
				old_c=0;//Forget
			}
		}else{
			dos_char(string[i]);
			cur_x+=16;
			old_c=0;
		}
	}
}
void graph_lib_3D::win_print(WCHAR *string,int maxlen){
	int len=wcslen(string);
	int old_c=0;
	int main_x=cur_x;//,main_y=cur_y;
	int counter=0;
	for(int i=0;i<len;i++){
		wchar_t c=string[i];
		if((c==13)||(c==10)){
			if((c==old_c)||(old_c==0)){
				//Enter
				cur_y-=18;//9;
				cur_x=main_x;
				old_c=c;
			}else{
				old_c=0;//Forget
			}
		}else{
			int aw=my_font_width[c]+1;
			if((maxlen>0)&&(counter+aw>maxlen))break;
			win_char(c);
			counter+=aw;
			cur_x+=aw;
		}
	}
}
//====================================================
void graph_lib_3D::goto_xy(int x,int y){
	cur_x=x;cur_y=y;cur_s=0;
	return;
}
void graph_lib_3D::goto_xyz(int x,int y,int smesh){
	cur_x=x;cur_y=y;cur_s=smesh;
	return;
}
void graph_lib_3D::set_color(int red,int green,int blue){
	cur_color=((red&0xff)<<16)|((green&0xff)<<8)|((blue&0xff));
	return;
}
//====================================================
void graph_lib_3D::putpixel(void){
	int gx,gy;
	gx = LIMIT(cur_x,0,max_x-1);
	gy = LIMIT(cur_y,0,max_y-1);
	image_bufer[(1+gy*2)*max_x+gx]=cur_color;

	gx = LIMIT(cur_x+cur_s,0,max_x-1);
	//gy = LIMIT(cur_y,0,max_y-1);
	image_bufer[(2+gy*2)*max_x+gx]=cur_color;
}
void graph_lib_3D::rectangle(int x,int y){
	int gx,gy;
	for(int i=cur_x;i<=x;i++){
		image_bufer[(1+cur_y*2)*max_x+i]=cur_color;
		image_bufer[(1+    y*2)*max_x+i]=cur_color;
		image_bufer[(2+cur_y*2)*max_x+i+cur_s]=cur_color;
		image_bufer[(2+    y*2)*max_x+i+cur_s]=cur_color;
	}
	for(int i=cur_y;i<=y;i++){
		image_bufer[(1+i*2)*max_x+cur_x]=cur_color;
		image_bufer[(1+i*2)*max_x+    x]=cur_color;
		image_bufer[(2+i*2)*max_x+cur_x+cur_s]=cur_color;
		image_bufer[(2+i*2)*max_x+    x+cur_s]=cur_color;
	}
	cur_x=x;cur_y=y;
}
void graph_lib_3D::thumbnail(unsigned char *thumb){// 2 x 96 x 192
	for(int p=0;p<2;p++)
		for(int y=0;y<96;y++){
			int ny=y+cur_y;
			if((ny>=0)&&(ny<max_y))
				for(int x=0;x<192;x++){
					int nx=x+cur_x+((p==1)?cur_s:0);
					if((nx>=0)&&(nx<max_x))
						image_bufer[(1+p+ny*2)*max_x+nx]=
						(thumb[((p*96+y)*192+x)*3+2]<<16)|
						(thumb[((p*96+y)*192+x)*3+1]<<8)|
						(thumb[((p*96+y)*192+x)*3+0]);
				}
		}
	return;
}

void graph_lib_3D::smooth(int x,int y){
	int gx,gy;
	for(int p=0;p<2;p++){
		for(int j=cur_y;j<=y;j++){
			int jj=(j<max_y-2)?j+2:j;
			for(int i=cur_x;i<=x;i++){
				int ii=(i<max_x-1)?i+1:i;
				int cc2=0;
				for(int c=0;c<3;c++){
					int cc1=0;
					cc1 =(image_bufer[(1+p+ j*2)*max_x+ i]>>(8*c))&0xff;
					cc1+=(image_bufer[(1+p+ j*2)*max_x+ii]>>(8*c))&0xff;
					cc1+=(image_bufer[(1+p+jj*2)*max_x+ i]>>(8*c))&0xff;
					cc1+=(image_bufer[(1+p+jj*2)*max_x+ii]>>(8*c))&0xff;
					cc1>>=3;
					cc2|=(cc1<<(8*c));
				}
				image_bufer[(1+p+ j*2)*max_x+ i]=cc2;
			}
		}
	}
	cur_x=x;cur_y=y;
}
void graph_lib_3D::draw_mono_pic(unsigned char *picture,int x_size,int y_size){
	for(int y=0;y<y_size;y++){
		int ny=y+cur_y;
		if((ny>=0)&&(ny<max_y))
			for(int x=0;x<x_size;x++){
				int nx=x+cur_x;
				if((nx>=0)&&(nx<max_x)){
					int c=
						(picture[(y*64+x)*3+2]<<16)|
						(picture[(y*64+x)*3+1]<<8)|
						(picture[(y*64+x)*3+0]);
					if(c!=0)
					image_bufer[(1+ny*2)*max_x+nx]=
					image_bufer[(2+ny*2)*max_x+nx]=c;
				}
			}
	}
	return;
}

void graph_lib_3D::draw_big_picture(
			BYTE *inp_picture,int inp_pitch,
			BYTE *inp_picture2,int inp_pitch2,
			int s_x,int s_y,
			//int r_x,int r_y,int l_x,int l_y,
			int c_x,int c_y,
			int lgg,float mnoj,
			int r,int g,int b,
			int &mi_x,int &ma_x,int &mi_y,int &ma_y){
	//--- Start drawing
	//int mi_x=int(c_x-s_x*mnoj/2.0+1.0),ma_x=int(c_x+s_x*mnoj/2.0);
	//int mi_y=int(c_y-s_y*mnoj/2.0+1.0),ma_y=int(c_y+s_y*mnoj/2.0);
	mi_x=int(c_x-s_x*mnoj/2.0+1.0);ma_x=int(c_x+s_x*mnoj/2.0);
	mi_y=int(c_y-s_y*mnoj/2.0+1.0);ma_y=int(c_y+s_y*mnoj/2.0);
	mi_x=LIMIT(mi_x,0,max_x-1);
	mi_y=LIMIT(mi_y,0,max_y-1);
	ma_x=LIMIT(ma_x,0,max_x-1);
	ma_y=LIMIT(ma_y,0,max_y-1);
	int color_mas[3]={b,g,r};
	//image_bufer[(1+c_y*2)*max_x+c_x]=0xffffff;//Right
	float rel_x,rel_y;
	//--- Reserv memory
	//int *hor=new(int [(ma_x-mi_x+1)*8]);
	int *hor=new(int [(ma_x-mi_x+1)*4]);
	int *ver=new(int [(ma_y-mi_y+1)*4]);
	//--- Fill vert array
	for(int iy=mi_y;iy<=ma_y;iy++){
		rel_y=(iy-c_y)/mnoj+s_y/2.0;
		int lo_y=int(rel_y),hi_y=int(rel_y+1);
		float ves_hi_y=rel_y-lo_y,ves_lo_y=hi_y-rel_y;
		lo_y=LIMIT(lo_y,0,s_y-1);
		hi_y=LIMIT(hi_y,0,s_y-1);

		int jy=iy-mi_y;
		ver[jy*4+0]=lo_y;
		ver[jy*4+1]=hi_y;
		ver[jy*4+2]=int(ves_lo_y*0x100+0.5f);
		ver[jy*4+3]=int(ves_hi_y*0x100+0.5f);
	}
	//--- Fill hor array
	//for(int p=0;p<2;p++)
	for(int ix=mi_x;ix<=ma_x;ix++){
		rel_x=(ix-c_x)/(2.0*mnoj)+s_x/4.0;
		//rel_x+=(p)?l_x:r_x;//Correct left/right
		int lo_x=int(rel_x),hi_x=int(rel_x+1);
		float ves_hi_x=rel_x-lo_x,ves_lo_x=hi_x-rel_x;
		lo_x=LIMIT(lo_x,0,s_x-1);
		hi_x=LIMIT(hi_x,0,s_x-1);

		int jx=ix-mi_x;
		hor[jx*4+0]=lo_x;
		hor[jx*4+1]=hi_x;
		hor[jx*4+2]=int(ves_lo_x*0x100+0.5f);
		hor[jx*4+3]=int(ves_hi_x*0x100+0.5f);
	}
	//--- Draw picture
	for(int jy=0,iy=mi_y;iy<=ma_y;iy++,jy++){
		int lo_y=ver[jy*4+0];
		int hi_y=ver[jy*4+1];
		int ves_lo_y=ver[jy*4+2];
		int ves_hi_y=ver[jy*4+3];

		for(int p=0;p<2;p++){
			BYTE *ip=(p)?inp_picture2:inp_picture;
			for(int ix=mi_x,jx=0;ix<=ma_x;ix++,jx++){
				int lo_x=hor[jx*4+0];
				int hi_x=hor[jx*4+1];
				int ves_lo_x=hor[jx*4+2];
				int ves_hi_x=hor[jx*4+3];
				//---
				int _a_=lo_y*inp_pitch+(lo_x)*3;
				int _b_=hi_y*inp_pitch+(lo_x)*3;
				int _c_=lo_y*inp_pitch+(hi_x)*3;
				int _d_=hi_y*inp_pitch+(hi_x)*3;
				//---
				{
					int mean_1,mean_2,mean_3=0;
					int col=0;
					for(int c=0;c<3;c++){
						mean_1 =ip[_a_+color_mas[c]]*ves_lo_y+
								ip[_b_+color_mas[c]]*ves_hi_y;
						mean_2 =ip[_c_+color_mas[c]]*ves_lo_y+
								ip[_d_+color_mas[c]]*ves_hi_y;
						mean_3 =mean_1*ves_lo_x+mean_2*ves_hi_x;
						//int smc=int(mean_3+0.5);
						int smc=(mean_3>>16);
						smc=LIMIT(smc,0,255);
						col|=smc<<(c*8);
					}
					image_bufer[(1+p+iy*2)*max_x+ix]=col;//Right
					//image_bufer[(2+iy*2)*max_x+ix]=col;//Left
				}
				//image_bufer[(1+iy*2)*max_x+ix]=0x0000ff;//Right
				//image_bufer[(2+iy*2)*max_x+ix]=0xffff00;//Left
			}
		}
	}
	delete[](hor);
	delete[](ver);
	return;
}

void graph_lib_3D::scroll_bar(int current,int maximum){
	for(int y=0;y<max_y;y++){
		int sootv=((max_y-1-y)*(maximum-1)+(max_y-1)/2)/(max_y-1);
		bool is_vis=((sootv>=current)&&(sootv<current+max_y));
		for(int x=-16;x<0;x++){
			image_bufer[(1+y*2)*max_x+max_x+x]=(is_vis)?0xff0000:0x00ff00;
			image_bufer[(2+y*2)*max_x+max_x+x]=(is_vis)?0xff0000:0x00ff00;
		}

	}
}
//====================================================
void graph_lib_3D::output(HDC* pDC){
	{
		//--- Main output
			//=== Output
			BITMAPINFO lpbmi;
			//lpbmi.bmiColors[0].rgbBlue=0;
			//lpbmi.bmiColors[0].rgbGreen=0;
			//lpbmi.bmiColors[0].rgbRed=0;
			//lpbmi.bmiColors=NULL;
			lpbmi.bmiHeader.biBitCount=32;//24;
			lpbmi.bmiHeader.biClrImportant=1;
			lpbmi.bmiHeader.biClrUsed=1;
			lpbmi.bmiHeader.biCompression=BI_RGB;//0;
			lpbmi.bmiHeader.biHeight=ry;
			lpbmi.bmiHeader.biPlanes=1;
			lpbmi.bmiHeader.biSize=sizeof(lpbmi);
			lpbmi.bmiHeader.biSizeImage=rx*ry*4;
			lpbmi.bmiHeader.biWidth=rx;//half_width;
			lpbmi.bmiHeader.biXPelsPerMeter=1;
			lpbmi.bmiHeader.biYPelsPerMeter=1;


			//HDC dc = GetDC(0);//Thanx to my friends :)
			for(int i=0;i<2;i++){
				//int x_dot=Params.x[i];
				//int y_dot=Params.y[i];
				SetDIBitsToDevice(
					*pDC,//dc,//Device Context (Full screen)
					0,//x_dot,//X dest
					0,//y_dot,//Y dest
					rx,//Params.w,//XWidth
					ry,//Params.h,//YWidth
					0,//??? Xsrc
					0,//??? Ysrc
					0,//??? uStartScan
					ry,//Params.h,//ScanLines
					//pDoc->image_bufer[main_point.y%2],//picture_bufer[i],
					&image_bufer[(skip_line)?0:rx],//picture_bufer[i],
					&lpbmi,
					DIB_RGB_COLORS//No Palette
				);
			}
			//ReleaseDC(0, dc);
	}
	if(skip_line)
	return;
}

void graph_lib_3D::init(int x0,int y0,int x1,int y1){
	//--- Remember real size
	rx=x1,ry=y1;
	//FileSystemView
	skip_line=false;
	max_x=x1;max_y=y1;
	skip_line=(y0%2==y1%2);
	if(skip_line){max_y-=1;}
	if(max_y%2)max_y-=1;//correct last line
	max_y/=2;
	//== memory
	int new_max_size = rx*(ry+1);
	if(new_max_size>max_size){
		if((max_size!=0)&&(image_bufer!=0))delete[]image_bufer;
		max_size=new_max_size;
		image_bufer=new(unsigned int[max_size]);
	}
	//=== Clear
	//memset(image_bufer,0x3f,max_size*sizeof(unsigned int));
	memset(image_bufer,0x00,max_size*sizeof(unsigned int));
}
void graph_lib_3D::getmax(int&x,int&y){
	x=max_x;
	y=max_y;
}
//====================================================
graph_lib_3D::graph_lib_3D(void){
	memset(this,0,sizeof(*this));
	//--- Make font
	/*if(1){
		unsigned __int16 bufer[256][8];
		FILE *f;fopen_s(&f,"font_09_bw.bmp","rb");
		unsigned __int16 bc;
		for(int ii=0;ii<31;ii++)fread(&bc,1,2,f);//Skip header
		for(int ii=0;ii<256*8;ii++){//  16*8 x 16
			fread(&bc,1,2,f);
			{//--- Reverse
				unsigned __int16 nbc=0,bit=0;
				for(int i=0;i<8;i++){
					bit=(bc>>i)&1;
					nbc|=bit<<(7-i);
					bit=(bc>>(i+8))&1;
					nbc|=bit<<(15-i);
				}
				bc=nbc;
			}
			int ch_x=ii%16;
			int ch_y=15-(ii/(16*8));
			int ln_y=(ii/16)%8;
			bufer[ch_y*16+ch_x][ln_y]=bc;
		}
		fclose(f);fopen_s(&f,"fnt.dat","wt");
		for(int i=0;i<256;i++){
			fprintf(f,"{");
			for(int jj=0;jj<8;jj++){
				fprintf(f,"0x%04x",bufer[i][jj]);
				if(jj<7)fprintf(f,",");
			}
			fprintf(f,"},\n");
		}
		fclose(f);
	}*/
	return;
}
graph_lib_3D::~graph_lib_3D(void){
	if((max_size!=0)&&(image_bufer!=0))delete[]image_bufer;
	memset(this,0,sizeof(*this));
	return;
}
