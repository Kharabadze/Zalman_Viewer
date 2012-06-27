#include"B_Object.h"

//===================================================================== Media

//===================================================================== Object
B_Object::B_Object(void){
	x_size=0;y_size=0;
}
bool B_Object::is_inside(int x,int y){
	return (visible&&(x>=0)&&(x<x_size)&&(y>=0)&&(y<y_size));
}
void B_Object::paint(graph_lib_3D *my_gl,int x,int y){
	my_gl->set_color(128,32,64);
	my_gl->goto_xy(x,y);
	my_gl->rectangle(x+x_size-1,y+y_size-1);
	return;
}
void B_Object::on_click(int x,int y){

	return;
}
