#version 400
 
layout(location=0) in vec2 in_Position;
layout(location=1) in vec4 in_Color;
out vec4 ex_Color;

uniform float psize;

uniform mat4 model;

void main(void){
	gl_PointSize = psize;
	float x = in_Position.x;
	float y = in_Position.y;
	float z = 0; 
	gl_Position = model*vec4(x,y,z,1);
	ex_Color = in_Color;
}


