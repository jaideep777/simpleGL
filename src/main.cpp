#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

#include "../headers/graphics.h"
//#include "../utils/simple_initializer.h" 
#include "../utils/simple_math.h"


int main(int argc, char **argv){

	float pos[] = { 50, 50, //0.5,
				  25, 50, //-0.5,
				  50,  25, //  -1,
				  25,   25 //  1,
				};

	float4 col[] = {make_float4(1,1,1, 1),
				    make_float4(1,0,0, 1),
				    make_float4(0,1,0, 1),
				    make_float4(0,0,1, 1)
				   };

	init_hyperGL(&argc, argv);
	
	int nx = 100, ny = 100;  
	float L = 2;
	ColorMap res_shape("res", false, 100, nx, 0, L);
	float2 cmap_pos[res_shape.nVertices];
	res_shape.createGridCentres(cmap_pos); 
	res_shape.createShaders();
	res_shape.createVBO(cmap_pos, res_shape.nVertices*sizeof(float2));	
	res_shape.createColorBuffer();
	vector <float> res(nx*ny); for (int i=0; i<nx*ny; ++i) res[i]=i; 
	res_shape.updateColors(&res[0], nx*ny);


	Shape pt("3dpt", false, 4); //, 4, -1, 1);
	pt.createShaders();
	pt.createVBO(pos, pt.nVertices*sizeof(float2));	
	pt.createColorBuffer();
	pt.setColors(col, pt.nVertices);
//	pt.setDefaultColor();
	
	
//	Renderer R;
//	R.init();
//	glRenderer = &R;
	
	glRenderer->addShape(&pt);
	
	while(1){       // infinite loop needed to poll anim_on signal.
		glutMainLoopEvent();
		usleep(20000);
	}
	// launch sim end.
	
	return 0;
}



