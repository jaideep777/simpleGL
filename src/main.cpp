#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

#include "../headers/graphics.h"
//#include "../utils/simple_initializer.h" 
#include "../utils/simple_math.h"


int main(int argc, char ** argv){
	float pos[] = {   50, 0, 0,//0.5,
				  0, 25, 0,//-0.5,
				  0,  0, -30,//  -1,
				  25,   25, -10 //  1,
				};

	float col[] = {1,0,0, 1,
				   0,1,0, 1,
				   0,0,1, 1,
				   1,1,1, 1
				   };

	init_hyperGL(&argc, argv);
	

	Shape pt(4, 3, "points"); // (no. of points, components per point (2/3), render method);
	pt.pointSize = 8;
	pt.setVertices(pos);	
	pt.setColors(col);
//	pt.autoExtent(pos);	

	
	float pos3[] = {0,0,0, 100,0,0, 0,0,0, 0,100,0, 0,0,0, 0,0,100};
	float col3[] = {1,0,0, 0.5,
				    1,0,0, 0.5,
				    0,1,0, 0.5,
				    0,1,0, 0.5,
				    0.0,0.8,1, 0.5,
				    0.0,0.8,1, 0.5
				   };
	
	Shape axis(6, 3, "lines");
	axis.setVertices(pos3);
	axis.setColors(col3);


	GLfloat pos4[] = {
	   50.f,  50.f, 0.0f,
	  -50.f,  50.f, 0.0f,
	  -50.f, -50.f, 0.0f,
	   50.f, -50.f, 0.0f
	};
	float col4[] = {
	  0, 0,1,1, 	0, 1, 0,1,
	  1,0,0,1,      1,1,1,1
	};
	int tess_ids[] = {0,1,2,2,3,0};

	unsigned char image[] = {
	  0, 0,255,255, 	0, 255, 0,255,    0,255,255,255,
	  255,0,0,255,    255,255,255,255,    255,255,0,255   
	};
	float UVs[] = {
	   1.0f, 1.0f,
	   0.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 0.0f
	};

//	Shape f(4, 3, "triangles", "tex"); // (no. of points, components per point (2/3), render method);
//	f.pointSize = 8;
//	f.setVertices(pos4);	
////	f.setColors(col4);
//	f.setElements(tess_ids, 6);
//	f.applyTexture(UVs, image, 3,2);

	Frame f1(0, 0, 50, 50, image);
//	f1.setExtent(-100,100,-100,100);
	f1.setLayer(100);
	
	while(1){       // infinite loop needed to poll anim_on signal.
		glutMainLoopEvent();
		usleep(20000);
	}
	// launch sim end.
	
	return 0;
}

