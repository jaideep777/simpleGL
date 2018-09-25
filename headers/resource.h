#ifndef TURBULENCE_H
#define TURBULENCE_H

#include <cufft.h>
#include <string>
#include <curand.h>
#include <curand_kernel.h>

#include "../headers/graphics.h"
#include "../utils/simple_initializer.h"
using namespace std;


class ResourceGrid{
	
	public:
	// resource 
	int nx, ny;	 // grid resolution
	float L, dL; // grid size 

	float dt; 	 // time step 

	bool graphics;
	
	float D; 	 // diffusion parameter for resource
	float * r, *r_dev;   // growth rate
	float * K, *K_dev;   // carrying capacity
	float *res, * res_dev, *res_new_dev;  // resource
	
	float totalRes;
	
	void init(Initializer &I);
	
	void update();
//	void updateColorMap();
//	void printMap(string mapname);
	void diffuse();
	void grow(float * ke_all_dev);
	
	float sumResource();
	
	void freeMemory();
	
	// graphics
	ColorMap res_shape;
	void graphics_updateArrays();

};



#endif
