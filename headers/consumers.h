#ifndef CONSUMERS_H
#define CONSUMERS_H

#include <string>
#include <curand.h>
#include <curand_kernel.h>

#include "../headers/graphics.h"
#include "../utils/simple_initializer.h"
#include "../utils/simple_histogram.h"

#include "../headers/resource.h"

using namespace std;


class Consumer{
	public:
	float2 pos;		// actual pos (float) 
	int2 pos_i;		// grid cell id of pos

	float h;
	float RT;
	float Kdsd;
	
	float rc;
	
	float ld;
	float nd;
	float vc;
	float vc_x;
	float vc_avg;
	
	float wsum;		// sum of imitation kernel weights (i.e. row sum of pd[i,:])
	int imit_whom;
	
	// for output
	float ld_cumm;
	float nd_cumm;
};

class ConsumerSystem{

	public:
	vector <Consumer> consumers;
	Consumer * consumers_dev, * consumers_child_dev;
	
	bool graphics;
	
	int nx, ny;
	float L, dL;
	int nc;
	float dt;
	
	float ke_lmax;		// bound for exploitation kernel in length units 
	float ke_nmax;		// bound for exploitation kernel in index units (kernel will go from [-ke_n...0...ke_n])
	float ke_sd;
	
	float *ke, *ke_dev;	// exploitation kernels on grid
	float *ke_all, *ke_all_dev;
	
	float ki_sd;		// sd of imitation kernel 
	
	float * pd_dev; 	// pairwise distances (converted to gaussian probabilities)
	float * cumm_prob_dev;	// matrix to store cummulative probabilities in roulette sampling 
//	float * wsum_dev;	// array of sums of imitation kernel weights (i.e. row sums of pd)
	
	float * nc_zeros;	// an array of zeros of length nc! helps to quickly init a device array to zero
	
	int vc_Tw;
	float * vc_window, *vc_window_dev; //, * vc_dev;
	
	int out_Tw;		// output interval
	
	float b, cd, ch;
	
	bool b_imit_h, b_imit_rt, b_imit_kd;
	float rImit;
	float mu_h, mu_RT, mu_kd;	// mutation size in h, rt, kd
	float imresv, dresv;
	
	float tmu; // heterogeneity parameter, included here for ease of access
	
	curandState * cs_dev_XWstates;
	int *cs_seeds_h, *cs_seeds_dev; 

	string output_dir, expt_desc;
	ofstream fout_h[2], fout_rc[2], fout_sd[2], fout_ndc[2], fout_ldc[2], fout_r, fout_wsum;

	vector <double> brks_h, brks_rc, brks_kd, brks_ldc, brks_ndc;
	Histogram hist_h, hist_rc, hist_kd, hist_ldc, hist_ndc;

	PointSet cons_shape;
	
	public:
	void init(Initializer &I);
	void initIO(Initializer &I);
	void closeIO();
	void initRNG();
	
	void updateExploitationKernels();
	void calcResConsumed(float * resource_dev);
	void disperse(ResourceGrid * resourceGrid);
	void calcPayoff(int t);
	void calcAvgPayoff();

	void imitate_by_kernel_sync();
//	void imitate_sync();
	
//	void imitate_global();
//	void imitate_global_sync();

//	void imitate_local_sync();

	void writeState(int istep, ResourceGrid * resgrid);

	void graphics_updateArrays();
	
	void printConsumers();
		
	void freeMemory();
	
};

#endif
