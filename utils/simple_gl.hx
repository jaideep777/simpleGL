#ifndef SIMPLE_GL
#define SIMPLE_GL

// ----------------------------------------------------------
// this file assumes that glew and glut are already included 
// ----------------------------------------------------------

#include <fstream>

inline void loadShader(string filename, GLuint &shader_id, GLenum shader_type){
	ifstream fin(filename.c_str());
	string s(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
	
	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &vertSrc, NULL);
	glCompileShader(shader_id);

}


#endif

