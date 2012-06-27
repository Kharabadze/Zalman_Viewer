#pragma once

//#include<sys/stat.h>
#include"FreeImage.h"

struct one_file_info{
	bool empty;
	bool folder;
	bool mono;
	bool bad_picture;
	//--- Picture
	//FIBITMAP *image;//For future use
	FREE_IMAGE_FORMAT fif;// Type of picture
	int im_w,im_h;

	unsigned char thumbnail[2*96*192*3];
	wchar_t *filename;
	one_file_info *next;
	bool have_description;
	//bool mono;
	int size_x,size_y;
	int left_x,left_y;
	int right_x,right_y;
	int colormask;
	int rotation;

	void get_name(wchar_t *fn,bool is_folder,bool is_mono);
	void get_information(bool thumb,FIBITMAP *&image,FIBITMAP *&image2);
	//=== Constructor
	one_file_info(void);
	~one_file_info(void);
};

struct my_expl{
	wchar_t cur_fold[60000];
	bool names_updated;
	int File_Number;
	one_file_info *file_array;
	one_file_info *file_cursor;
	one_file_info *file_drawer;
	//=============
	void add_filename(wchar_t *fileinfo_name,bool is_folder);
	void get_folder(void);
	void clear_folder(void);
	//=============
	my_expl(void);
	~my_expl(void);
};