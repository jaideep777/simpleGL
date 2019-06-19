#version 330
 
layout(location=0) in vec3 in_pos;
layout(location=1) in vec4 in_col;
out vec4 ex_col;

uniform float psize;

uniform mat4 model;

void main(void){
	gl_PointSize = psize;
	gl_Position = model*vec4(in_pos,1);
	ex_col = in_col;
}


