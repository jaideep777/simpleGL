#version 330
 
in vec3 in_pos;
in vec4 in_col;
in vec2 in_UV;

out vec4 ex_col;
out vec2 ex_UV;

uniform mat4 model;

void main(void){
	gl_Position = model*vec4(in_pos,1);
	ex_col = in_col;
	ex_UV = in_UV;
}


