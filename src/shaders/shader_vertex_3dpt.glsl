#version 400
 
layout(location=0) in vec2 in_Position;
layout(location=1) in vec4 in_Color;
out vec4 ex_Color;

uniform vec2 xlim;	// .x=xmin, .y=xmax, .z=ymin, .w=ymax
uniform vec2 ylim;	// .x=xmin, .y=xmax, .z=ymin, .w=ymax
uniform vec2 zlim;	// .x=xmin, .y=xmax, .z=ymin, .w=ymax
uniform float psize;

void main(void){
	gl_PointSize = psize;
	float x = -1+2/(xlim.y-xlim.x)*(in_Position.x-xlim.x);
	float y = -1+2/(ylim.y-ylim.x)*(in_Position.y-ylim.x);
	float z = 0; //-1+2/(zlim.y-zlim.x)*(in_Position.z-zlim.x);
	gl_Position = vec4(x,y,z,1);
	ex_Color = in_Color;
}


