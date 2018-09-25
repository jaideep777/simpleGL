#ifndef TURBULENCE1_H
#define TURBULENCE1_H

#include <cufft.h>
#include <string>
#include <curand.h>
#include <curand_kernel.h>
#include "../utils/simple_initializer.h"
using namespace std;

class TurbulenceEngine{
	
	public:
	int nx;	// grid size for turbulence
	int ny;  

	float L;
	float xmin, xmax, ymin, ymax;

	float mu, xi, nu, lambda0;
	float dt;
	
	cufftHandle plan;	// fft config
	int nlevCol;

	float *lambda, *lambda_dev;		// energy spectrum of noise
	cufftComplex * Psi, * Psi_dev;	// FFT of potential fn
	cufftComplex * psi, * psi_dev;	// potential fn
	cufftComplex * Z, * Z_dev;		// noise matrix
	
	float2 * vel_field, * vel_field_dev;		// velocity field
	
	curandState * te_dev_XWstates;
	int *te_seeds_h, *te_seeds_dev; 
	
	void initRNG();
	void init(Initializer &I);
	
	void generateSpectrum();
	void calcEquilPsi();
	
//	void generateNoise();
//	void modifyPsi();
	void generateNoise_gpu();
	void modifyPsi_gpu();
	void transformPsi();
	void calcVelocityField();

	void normalize_psi();

	void update();

	void updateColorMap();
	void printMap(string mapname, ofstream &fout);

	void freeMemory();

};



#endif
