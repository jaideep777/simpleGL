#version 400
 
in vec4 ex_Color;
out vec4 out_Color;
 
void main(void){
	out_Color = ex_Color;
//	vec2 circCoord = 2.0*gl_PointCoord - 1.0;
//	if (dot(circCoord, circCoord) > 1.0) discard;
}


