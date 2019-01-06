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
	

	Shape pt(4, 3, "points"); //, 4, -1, 1);
	pt.pointSize = 4;
	pt.setVertices(pos);	
	pt.setColors(col);
	pt.autoExtent(pos);	

	
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


	while(1){       // infinite loop needed to poll anim_on signal.
		glutMainLoopEvent();
		usleep(20000);
	}
	// launch sim end.
	
	return 0;
}

