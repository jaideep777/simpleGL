#version 400

layout(location=0) in vec2 in_Position;
out vec4 ex_Color;

uniform vec4 bounds;	// .x=xmin, .y=xmax, .z=ymin, .w=ymax

void main(void){
	float x = -1+2/(bounds.y-bounds.x)*(in_Position.x-bounds.x);
	float y = -1+2/(bounds.w-bounds.z)*(in_Position.y-bounds.z);
	gl_Position = vec4(x,y,0,1);
	ex_Color = vec4(1,1,1,0.5);
}


