#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

#include "../headers/graphics.h"
//#include "../utils/simple_initializer.h" 
#include "../utils/simple_math.h"


int main(int argc, char **argv){

	float pos[] = { 50, 50, -10,//0.5,
				  25, 50, -20,//-0.5,
				  50,  25, -30,//  -1,
				  25,   25, 0 //  1,
				};

	float4 col[] = {make_float4(1,1,1, 1),
				    make_float4(1,0,0, 1),
				    make_float4(0,1,0, 1),
				    make_float4(0,0,1, 1)
				   };

	init_hyperGL(&argc, argv);
	
//	int nx = 100, ny = 100;  
//	float L = 2;
//	ColorMap res_shape("res", 100, nx, 0, L);
//	float2 cmap_pos[res_shape.nVertices];
//	res_shape.createGridCentres(cmap_pos); 
//	res_shape.createShaders();
//	res_shape.createVBO(cmap_pos, res_shape.nVertices*sizeof(float2));	
//	res_shape.createColorBuffer();
//	vector <float> res(nx*ny); for (int i=0; i<nx*ny; ++i) res[i]=i; 
//	res_shape.updateColors(&res[0], nx*ny);


	Shape pt("3dpt", 4, "triangles"); //, 4, -1, 1);
//	pt.createShaders();
	pt.setVertices(pos, 3*sizeof(float));	
//	pt.createColorBuffer();
	pt.setColors(col);
//	pt.setDefaultColor();
	

	float pos2[] = { -25, -25, 0,//0.5,
				  -25, 25, 0,//-0.5,
				  25,  -25, 0,//  -1,
				  25,   25, 0 //  1,
					};

	float4 col2[] = {make_float4(1,1,0, 0.5),
				    make_float4(1,0,1, 0.5),
				    make_float4(0,1,0, 0.5),
				    make_float4(1,1,1, 0.5)
				   };
					
	Shape pt2("3dpt", 4, "triangles");
	pt2.setVertices(pos2, 3*sizeof(float));
	pt2.setColors(col2);
	
	float pos3[] = {-100,0,0, 100,0,0, 0,-100,0, 0,100,0, 0,0,-100, 0,0,100};
	float4 col3[] = {make_float4(1,0,0, 0.5),
				    make_float4(1,0,0, 0.5),
				    make_float4(0,1,0, 0.5),
				    make_float4(0,1,0, 0.5),
				    make_float4(0.0,0.8,1, 0.5),
				    make_float4(0.0,0.8,1, 0.5)
				   };
	
	Shape axis("3dpt", 6, "lines");
	axis.setVertices(pos3, 3*sizeof(float));
	axis.setColors(col3);

	Palette p(20);
	p.create_grayscale();
	p.print();
	
	float v[] = {0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4};
	vector <glm::vec4> c = p.map_values(v, 8);
	for (int i=0; i<8; ++i) cout << c[i].r << " " << c[i].g << " " << c[i].b << " " << c[i].a << "\n";
	cout << endl; 
	
	while(1){       // infinite loop needed to poll anim_on signal.
		glutMainLoopEvent();
		usleep(20000);
	}
	// launch sim end.
	
	return 0;
}



