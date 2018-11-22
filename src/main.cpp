#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

#include "../headers/graphics.h"
//#include "../utils/simple_initializer.h" 
#include "../utils/simple_math.h"

namespace fl{

struct vec3{
	float x;
	float y;
	float z;
};

}

bool compare_z(const fl::vec3& p1, const fl::vec3& p2){
	return (p1.z < p2.z);
}

bool compare_y(const fl::vec3& p1, const fl::vec3& p2){
	return (p1.y < p2.y);
}

bool compare_x(const fl::vec3& p1, const fl::vec3& p2){
	return (p1.x < p2.x);
}

void sort_by_y(float* begin, float* end){
	sort((fl::vec3*)begin, (fl::vec3*)end, compare_y); 
}
	
void sort_by_z(float* begin, float* end){
	sort((fl::vec3*)begin, (fl::vec3*)end, compare_z); 
}

void sort_by_x(float* begin, float* end){
	sort((fl::vec3*)begin, (fl::vec3*)end, compare_x); 
}


vector <int> y_slices(float * pos, int n, float res){
	vector <int> slice_indices(1,0);
	float min = int(pos[3*0+1]/res)*res;
	for (int i=0; i< n; ++i){
		if (pos[3*i+1] > (min + res*slice_indices.size()) ){
			slice_indices.push_back(i);
			cout << "slice: " << i << ", value = " << pos[3*i+1] <<  endl;
		}
	}
//	if (*slice_indices.end() != n-1) slice_indices.push_back(n-1);
	return slice_indices;
}

vector <int> z_slices(float * pos, int n, float res){
    vector <int> slice_indices(1,0);
    float min = int(pos[3*0+2]/res)*res;
    for (int i=0; i< n; ++i){
        if (pos[3*i+2] > (min + res*slice_indices.size()) ){
            slice_indices.push_back(i);
            cout << "slice: " << i << ", value = " << pos[3*i+2] <<  endl;
        }
    }
//    if (*slice_indices.end() != n-1) slice_indices.push_back(n-1);
    return slice_indices;
}


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


	Shape pt(4, 3, "points"); //, 4, -1, 1);
//	pt.createShaders();
	pt.pointSize = 4;
	pt.setVertices(pos);	
//	pt.createColorBuffer();
	pt.setColors(col);
//	pt.autoExtent(pos);
//	pt.setDefaultColor();
	

	
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

