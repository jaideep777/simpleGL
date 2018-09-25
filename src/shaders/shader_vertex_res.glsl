#version 400
 
layout(location=0) in vec2 in_Position;
layout(location=1) in vec4 in_Color;
out vec4 ex_Color;

uniform vec4 bounds;	// .x=xmin, .y=xmax, .z=ymin, .w=ymax
uniform float psize;

void main(void){
	gl_PointSize = psize;
	float x = -1+2/(bounds.y-bounds.x)*(in_Position.x-bounds.x);
	float y = -1+2/(bounds.w-bounds.z)*(in_Position.y-bounds.z);
	gl_Position = vec4(x,y,0,1);
	ex_Color = in_Color;
}


