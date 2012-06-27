//#include"stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<io.h>
#include<memory.h>

#include"my_expl.h"

#include"exp_pictures.hpp"
//#include<sys/types.h>
//#include<sys/stat.h>
//#include<sys/stat.h>
//#include<sys/types.h>
//#include<stdio.h>
//#include<errno.h>


#ifdef _M_X64
#pragma comment(lib,"FreeImage_x64.lib")
#else
#pragma comment(lib,"FreeImage_x86.lib")
#endif


void one_file_info::get_name(wchar_t *fn,bool is_folder,bool is_mono){
	int len=wcslen(fn);
	filename=new(wchar_t[len+1]);
	wcscpy(filename,fn);
	folder=is_folder;// :)
	mono=is_mono;// :) :)
	this->empty=(is_folder)?false:true;
	if(is_folder){
		int pp=96*192*3;
		if(!wcscmp(fn,L"..")){
			for(int y=0;y<96;y++){
				for(int x=0;x<192;x++){
					int ox=(y*192+x)*3;
					thumbnail[pp+ox+0]=thumbnail[ox+0]=up_folder[ox+0];
					thumbnail[pp+ox+1]=thumbnail[ox+1]=up_folder[ox+1];
					thumbnail[pp+ox+2]=thumbnail[ox+2]=up_folder[ox+2];
				}
			}
		}else{
			for(int y=0;y<96;y++){
				for(int x=0;x<192;x++){
					int ox=(y*192+x)*3;
					thumbnail[pp+ox+0]=thumbnail[ox+0]=down_folder[ox+0];
					thumbnail[pp+ox+1]=thumbnail[ox+1]=down_folder[ox+1];
					thumbnail[pp+ox+2]=thumbnail[ox+2]=down_folder[ox+2];
				}
			}
		}
	}
	return;
}
//image1,image2 = empty
void one_file_info::get_information(bool thumb,FIBITMAP *&image,FIBITMAP *&image2){
	bad_picture=false;// :)
	if(this->folder){
		empty=false;
	}else{
		if(!thumb){
			if(image!=0)FreeImage_Unload(image );//Later remove
			if(image2!=0)FreeImage_Unload(image2 );//Later remove
			image=image2=0;
		}

		//FILE *f;_wfopen_s(&f,this->filename,L"RB");
		//FREE_IMAGE_FORMAT fif=FreeImage_GetFileTypeU(filename);
		// === Start Reading ===
		//image=0;
		//image=FreeImage_LoadU(fif,filename);
		{
			FILE *ff;
			_wfopen_s(&ff,filename,L"rb");
			fseek(ff,0,SEEK_END);
			int len = ftell(ff);
			fseek(ff,0,SEEK_SET);
			unsigned char *bufer=new(unsigned char[len]);
			fread(bufer,1,len,ff);
			fclose(ff);
			//--- Test for JPEG - MPO
			//bool MPO_is = false;
			int MPO_start = 0;
			if(fif == FIF_JPEG){
				int st =0;
				int sost=0;
				while(1){
					if(st>=len)break;
					unsigned char a=bufer[st++];
					if(a==0xff){//marker
						if(st>=len)break;
						unsigned char b=bufer[st++];
						if(b==0xD8){
							if(sost==0)sost++;
							else if(sost==2){//process table
								if(st<4)break;
								MPO_start=st-2;
								break;
							}else break;
						}else if(b==0xD9){
							if(sost==1)sost=2;
							else break;
						}/*else
						if(b==0xE2){
							int size = bufer[st++]<<8;
							size|=bufer[st++];
							if(size<2)break;
							int newst=st+size;
							//=== Title
							if(bufer[st++]!=0x4D){st=newst;break;}//'M'
							if(bufer[st++]!=0x50){st=newst;break;}//'P'
							if(bufer[st++]!=0x46){st=newst;break;}//'F'
							if(bufer[st++]!=0x00){st=newst;break;}//'\0'

							//=== Shift zero
							int shift_zero = st;

							//=== Endian
							bool big_endian=false;
							if(bufer[st]==0x4D){
								st++;
								if(bufer[st++]!=0x4D){st=newst;break;}//'M'
								if(bufer[st++]!=0x00){st=newst;break;}//'M'
								if(bufer[st++]!=0x2A){st=newst;break;}//'M'
								big_endian=true;
							}else if(bufer[st]==0x49){
								st++;
								if(bufer[st++]!=0x49){st=newst;break;}//'M'
								if(bufer[st++]!=0x2A){st=newst;break;}//'M'
								if(bufer[st++]!=0x00){st=newst;break;}//'M'
								big_endian=false;
							}else {st=newst;break;}

							//=== Shift
							int sh_st=0;
							if(big_endian){
								(sh_st<<=8)|=bufer[st++];
								(sh_st<<=8)|=bufer[st++];
								(sh_st<<=8)|=bufer[st++];
								(sh_st<<=8)|=bufer[st++];
							}else{
								sh_st|=bufer[st++];
								sh_st|=bufer[st++]<<8;
								sh_st|=bufer[st++]<<16;
								sh_st|=bufer[st++]<<24;
							}
							st = shift_zero+sh_st;

							//=== Process count
							int count=0;
							if(big_endian){
								(count<<=8)|=bufer[st++];
								(count<<=8)|=bufer[st++];
							}else{
								count|=bufer[st++];
								count|=bufer[st++]<<8;
							}

							//=== CYCLE
							for(int i=0;i<count;i++){
								st+=2;//skip header
								int type = 0;
								if(big_endian){
									(type<<=8)|=bufer[st++];
									(type<<=8)|=bufer[st++];
									(type<<=8)|=bufer[st++];
									(type<<=8)|=bufer[st++];
								}else{
									type|=bufer[st++];
									type|=bufer[st++]<<8;
									type|=bufer[st++]<<16;
									type|=bufer[st++]<<24;
								}
								int ts=0;
								if(type == 0x00070000)ts=1;
								if(type == 0x00040000)ts=4;
								if(type == 0x000A0000)ts=8;
								//=== count2
								int count2 = 0;
								if(big_endian){
									(count2<<=8)|=bufer[st++];
									(count2<<=8)|=bufer[st++];
								}else{
									count2|=bufer[st++];
									count2|=bufer[st++]<<8;
								}
								st+=count2*ts;
							}

							//=== GOOD!
							//MPO_is = true;
							MPO_start = 0;
							if(big_endian){
								(MPO_start<<=8)|=bufer[st++];
								(MPO_start<<=8)|=bufer[st++];
								(MPO_start<<=8)|=bufer[st++];
								(MPO_start<<=8)|=bufer[st++];
							}else{
								MPO_start|=bufer[st++];
								MPO_start|=bufer[st++]<<8;
								MPO_start|=bufer[st++]<<16;
								MPO_start|=bufer[st++]<<24;
							}
							MPO_start+=shift_zero;
							break;/// MPO !!!
						}*/
						else if(
							(b==0x01)||
							(b>=0xD0)&&(b<=0xD7)||
							(b==0xD8)||
							(b==0xD9))st++;//SOI
						else if(
							(b>=0x02)&&(b<=0xcf)||
							(b>=0xda)&&(b<=0xfd)){
							int size = bufer[st++]<<8;
							size|=bufer[st++];
							if(size<2)break;
							st+=size-2;
						}
					}
				}
			}
			//--- Convert to image
			FIMEMORY *hmem = FreeImage_OpenMemory(&bufer[MPO_start], len-MPO_start);
			FIMEMORY *hmem2 = (MPO_start)?FreeImage_OpenMemory(&bufer[0], MPO_start):0;
			image = 0;
			image = FreeImage_LoadFromMemory(fif,hmem,0);
			image2 = 0;
			image2 = (MPO_start)?FreeImage_LoadFromMemory(fif,hmem2,0):0;
			//---
			delete[](bufer);
		}
		// === Finish Reading ===
		if(image==0){
			empty=false;
			bad_picture=true;
			for(int i=0;i<2*192*96;i++){
				this->thumbnail[i*3+0]=0x16;
				this->thumbnail[i*3+1]=0x16;
				this->thumbnail[i*3+2]=0xff;
			}
			return;
		}
		im_w=FreeImage_GetWidth(image);
		im_h=FreeImage_GetHeight(image);
		if(image2){
			int im_w_dop=FreeImage_GetWidth(image2);
			int im_h_dop=FreeImage_GetHeight(image2);
			if(im_w_dop<im_w)im_w=im_w_dop;
			if(im_h_dop<im_h)im_h=im_h_dop;
		}

		int im_w2=(mono)?im_w:im_w/2;
		int lo=__min(im_w2,im_h);
		int sx=(im_w2-lo)/2;
		int sy=(im_h-lo);// /2;

		int bpp=FreeImage_GetBPP(image);
		if(bpp!=24){//Conversion
			FIBITMAP *temp;
			temp=FreeImage_ConvertTo24Bits(image);
			FreeImage_Unload(image);
			image=temp;
		}
		if(image2){
			int bpp=FreeImage_GetBPP(image2);
			if(bpp!=24){//Conversion
				FIBITMAP *temp;
				temp=FreeImage_ConvertTo24Bits(image2);
				FreeImage_Unload(image2);
				image2=temp;
			}
		}
		//if(bbp!=24)


		BYTE *g_image = (BYTE*)FreeImage_GetBits(image);
		BYTE *g_image2 = (image2)?(BYTE*)FreeImage_GetBits(image2):0;
		int pitch = FreeImage_GetPitch(image);
		int pitch2 = (image2)?FreeImage_GetPitch(image2):0;
		if(empty){
			for(int p=0;p<2;p++){
				for(int y=0;y<96;y++){
					int oy=(y*(lo-1)+47)/95 +sy;
					BYTE *g_line=&g_image[oy*pitch+p*im_w2*3];
					if(mono) g_line=&g_image[oy*pitch];// :)
					if(image2) g_line = (p)?&g_image2[oy*pitch2]:&g_image[oy*pitch];

					for(int x=0;x<192;x++){
						int ox=(x*(lo-1)+95)/191+sx;// :)
						//if(mono)ox*=2;
						BYTE *g_pix=&g_line[ox*3];
						thumbnail[((p*96+y)*192+x)*3+2]=g_pix[FI_RGBA_RED];
						thumbnail[((p*96+y)*192+x)*3+1]=g_pix[FI_RGBA_GREEN];
						thumbnail[((p*96+y)*192+x)*3+0]=g_pix[FI_RGBA_BLUE];
					}
				}
			}
			empty=false;
		}
		//Free only if empty
		if(thumb){
			if(image)FreeImage_Unload(image );//Later remove
			if(image2)FreeImage_Unload(image2);//Later remove
			image=image2=0;
		}
	}
	return;
}
one_file_info::one_file_info(void){
	empty=true;
	filename=0;
	mono=false;
	folder=false;
	bad_picture=false;
	next=0;
	for(int i=0;i<2*192*96;i++){
		this->thumbnail[i*3+0]=0x7f;
		this->thumbnail[i*3+1]=0x3f;
		this->thumbnail[i*3+2]=0x7f;
	}
	return;
}
one_file_info::~one_file_info(void){
	if(filename)delete[](filename);
	if(next)delete(next);
	empty=true;
	return;
}
bool is_equal(wchar_t *a,wchar_t *b){
	for(int i=0;i<5;i++){
		if((towlower(a[i])==0)&&(b[i]==0))return true;
		if(towlower(a[i])!=b[i])return false;
	}
	return true;
}

bool is_picture(wchar_t *filename,bool &mono){
	int len=wcslen(filename);
	int dot=-1;
	for(int i=0;i<len;i++)
		if(filename[i]=='.')dot=i;
	if(dot==-1)return false;//No extension
	/*if(!wcscmp(&filename[dot],L".jpg"))return true;
	if(!wcscmp(&filename[dot],L".Jpg"))return true;
	if(!wcscmp(&filename[dot],L".JPG"))return true;
	if(!wcscmp(&filename[dot],L".jpeg"))return true;
	if(!wcscmp(&filename[dot],L".Jpeg"))return true;
	if(!wcscmp(&filename[dot],L".JPEG"))return true;*/
	bool res=false;
	if(is_equal(&filename[dot+1],L"jps")){
		mono=false;res=true;
	}else if(
		is_equal(&filename[dot+1],L"jpg")||//Jpeg JFIF
		is_equal(&filename[dot+1],L"jpeg")||//Jpeg JFIF
		is_equal(&filename[dot+1],L"jp2")||//Jpeg 2000
		is_equal(&filename[dot+1],L"j2k")||//Jpeg 2000
		is_equal(&filename[dot+1],L"gif")||//GIF
		is_equal(&filename[dot+1],L"png")||//Portable Network Graphics
		is_equal(&filename[dot+1],L"bmp")||//Bitmap
		is_equal(&filename[dot+1],L"pcx")||//PCX
		is_equal(&filename[dot+1],L"ico")//Icon
		){
		mono=true;res=true;
	}else{
		mono=true;res=false;
	}

	return(res);
	//if(!wcscmp(&filename[dot],L".jps"))return true;
	//if(!wcscmp(&filename[dot],L".Jps"))return true;
	//if(!wcscmp(&filename[dot],L".JpS"))return true;
	//if(!wcscmp(&filename[dot],L".JPS"))return true;
	//return false;
}
//===============================================
//===============================================
//===============================================
//===============================================
void my_expl::add_filename(wchar_t *fileinfo_name,bool is_folder){
	if(!wcscmp(fileinfo_name,L"."))return;
	//if(!wcscmp(fileinfo_name,L".."))return;
	if(is_folder){
		if(file_array==0){
			file_array=new(one_file_info);
			file_cursor=file_array;
		}else{
			file_cursor->next=new(one_file_info);
			file_cursor=file_cursor->next;
		}
		file_cursor->get_name(fileinfo_name,is_folder,true);
		File_Number++;
	}else{
		bool is_mono=false;
		bool isp=is_picture(fileinfo_name,is_mono);//stereo = JPS only
		FREE_IMAGE_FORMAT loc_fif=FreeImage_GetFileTypeU(fileinfo_name);
		//if(isp)
		isp=(loc_fif!=FIF_UNKNOWN);//is picture only if known format...
		if(isp){
			if(file_array==0){
				file_array=new(one_file_info);
				file_cursor=file_array;
			}else{
				file_cursor->next=new(one_file_info);
				file_cursor=file_cursor->next;
			}
			file_cursor->get_name(fileinfo_name,is_folder,is_mono);//!!!
			file_cursor->fif=loc_fif;//Remember format !!!
			File_Number++;
		}
	}
	return;
}
void my_expl::get_folder(void){
	//----- Get folder
	//if(cur_fold)delete[](cur_fold);
	_wgetcwd(cur_fold,65000);
	//-----
	_wfinddata64_t FD;
	intptr_t xx=_wfindfirst64(L"*.*",&FD);
	if(xx==-1)return;//Error
	//intptr_t
	//CFileFind my_cat;
	//bool cont = my_cat.FindFile(L"*.*");  
   	while(xx){ 
		//cont=my_cat.FindNextFile();//if(!cont)break;
		//CString FileName = my_cat.GetFileName(); 
		//add_filename(FileName.GetBuffer(),my_cat.IsDirectory());
		//_A_ARCH
		//_A_HIDDEN
		//_A_NORMAL
		//_A_RDONLY
		//_A_SUBDIR
		//_A_SYSTEM
		bool subdir = (FD.attrib & _A_SUBDIR);
		add_filename(FD.name,subdir);
		if(_wfindnext64(xx,&FD)!=0)break;
	}
	_findclose(xx);
	
	names_updated=true;
	return;
}
void my_expl::clear_folder(void){
	delete(file_array);
	file_array=file_cursor=0;
	memset(this,0,sizeof(*this));
	names_updated=false;
	return;
}

//================================================
my_expl::my_expl(void){
	memset(this,0,sizeof(*this));
	names_updated=false;
	file_array=file_cursor=file_drawer=0;
	//=== Get picture
	/*if(1){
		FILE *f,*o;
		fopen_s(&f,"Next.bmp","rb");
		fopen_s(&o,"f_out.txt","wt");
		fseek(f,54,SEEK_SET);
		for(int y=0;y<64;y++){
			for(int x=0;x<64;x++)
				for(int c=0;c<3;c++){
					unsigned char b=0;
					fread(&b,1,1,f);
					if(y%2==0){
						fprintf(o,"%3i,",b);
						if((x%8==7)&&(c==2))
							fprintf(o,"\n");
					}
				}
		}
		fclose(f);
		fclose(o);
	}*/
	return;
}

my_expl::~my_expl(void){
	if(file_array)delete(file_array);
	//--- Reset
	memset(this,0,sizeof(*this));
	return;
}