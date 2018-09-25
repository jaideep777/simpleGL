#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

#include <fstream>
#include <iostream>
#include <map>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

const float pi = 3.14159265358;

#ifdef __CUDACC__
#define DEVICE_NAME __device__ __host__
#else
#define DEVICE_NAME 
#endif

// =============================================================================
// 		Simple indexing utils
// 		ADDED by : JAIDEEP
// 		6 May 2013
// =============================================================================

inline DEVICE_NAME int ix2(int ix, int iy, int nx){
	return iy*nx + ix;
}

inline DEVICE_NAME int ix3(int ix, int iy, int iz, int nx, int ny){
	return iz*nx*ny + iy*nx + ix;
}

// =============================================================================
// 		1D Grid functions
// 		ADDED by : JAIDEEP
// 		18 Nov 2014
// =============================================================================
inline int discretize_index(float f, int n, float minf, float maxf){
	if (fabs(minf - maxf) <1e-6) return 0;
	else return int((f-minf)/(maxf-minf)*(n-1));
}


inline DEVICE_NAME int pos2cell(float x, float dL){
	return int((x-dL/2)/dL+0.5); //+1
}

inline DEVICE_NAME float cell2pos(int i, float dL){
	return i*dL + dL/2;
}


// =============================================================================
// 		1D Periodic Distance functions
// 		ADDED by : JAIDEEP
// 		19 Feb 2013
// =============================================================================


// indicator variable I{x<a} = {1, x < a; 0 otherwise}
inline DEVICE_NAME float indicator(float x, float a){
	return float(x < a);
}

// brings x to principal range defined by (xmin, xmax)
inline DEVICE_NAME void makePeriodic(float &x, float xmin, float xmax){
	x = xmin + fmodf(x-xmin, xmax-xmin);
	x += xmax-xmin;							// this second fmodf is required because some implementations..
	x = xmin + fmodf(x-xmin, xmax-xmin);	// ..of fmodf use negative remainders in principle range 
//	x = x + (float(x < xmin) - float(x > xmax))*(xmax-xmin);
}

// brings x to principal range defined by (xmin, xmax)
inline DEVICE_NAME int makePeriodicID(int ix, int n){
	ix = ix % n;
	ix += n;		// this second fmodf is required because some implementations..
	ix = ix % n;	// ..of fmodf use negative remainders in principle range 
	return ix;
}

// calculate displacement x1 ---> x2 on a cylinder with circumference X
// limits (-X/2 , X/2) are inspired from principle range of theta
inline DEVICE_NAME float periodicDisplacement(float x1, float x2, float X){
	float dx = (x2-x1);
	makePeriodic(dx, -X/2, X/2);
	return dx;
}

// periodic distance is minimum distance on a cylinder
inline DEVICE_NAME float periodicDistance(float x1, float x2, float X){
	return fabs(periodicDisplacement(x1,x2,X));
}



// =============================================================================
// 		Simple array operations
// 		ADDED by : JAIDEEP
// 		4 Mar 2013
// =============================================================================

template <class T>
T arraySum(T *v, int n){
	T sum=0;
	for (int i=0; i<n; ++i) sum += v[i];
	return sum;
}

template <class T>
T arrayMin(T *v, int n){
	T amin=v[0];
	for (int i=1; i<n; ++i) amin = min(amin, v[i]);
	return amin;
}

template <class T>
T arrayMax(T *v, int n){
	T amax=v[0];
	for (int i=1; i<n; ++i) amax = max(amax, v[i]);
	return amax;
}

// =============================================================================
// 		Random numbers from the default C++ generator
// 		ADDED by : JAIDEEP
// 		9 Nov 2014
// =============================================================================

inline float runif(float rmin=0, float rmax=1){
	float r = float(rand())/RAND_MAX; 
	return rmin + (rmax-rmin)*r;
}

inline float rnorm(float mu=0, float sd=1){
	float u = runif(), v = runif();		// uniform rn's [0,1] for box-muller
	float x = sqrt(-2.0*log(u)) * cos(2*pi*v);
	return mu + sd*x;
}

// =============================================================================
// 		Map operations
// 		ADDED by : JAIDEEP
// 		21 Dec 2013
// =============================================================================

template <class T>
T mapSum(map <int, T> &m){
	T sum = 0;
	for (typename map<int,T>::iterator it= m.begin(); it != m.end(); ++it){
		sum += it->second;
	}
	return sum;
}

template <class T>
float mapAvg(map <int, T> &m){
	return mapSum(m)/float(m.size());
}


// =============================================================================
// 		Sampling Algos
// 		ADDED by : JAIDEEP
// 		15 Apr 2016
// =============================================================================

/*---------------------------------------------------------------------------------------------
	Binary search in array arr to find first element greater than val
---------------------------------------------------------------------------------------------*/
inline int DEVICE_NAME bin_search_lub(float val, float * arr, int arrlen){

 	// selected element
	int r = 0;

	// binary search init
	int lo = 0;
	int hi = arrlen-1;
	int mid = (hi+lo)/2;

	// search for lower bound, then increment it till we go just beyond a
	while(hi != mid && lo != mid){
		if (arr[mid] > val){
			hi = mid;
			mid = (hi+lo)/2;
		}
		else{
			lo = mid;
			mid = (hi+lo)/2;
		}
	}
	r = lo;

	// increment r until lowest number > a is reached (or r is arrlen-1)
	while(arr[r] <= val && r < arrlen-1){
		++r;
	}
	if (r < 1) r=1;
	
	return r;
} 


/*---------------------------------------------------------------------------------------------

	Sampling using roulette wheel algorithm

       0   1      2      3 4      5               6            <-- element of the range (1:n)
	|----|---|---------|--|-|------------|-----------------|
	0    1   2    ^    3  4 5            6                 7   <-- range edges (cumm_prob)
	              |    ^
	              |    |__ upper bound = 1st element > range selector
	              |
                  |__ range selector (selected element = 2 = upper_bound - 1)

---------------------------------------------------------------------------------------------*/
inline int sample_roulette(float * weights, int n){

	// create ranges array
	double cumm_prob[n+1]; 
	cumm_prob[0] = 0;
	for (int i=0; i<n; ++i) cumm_prob[i+1] = cumm_prob[i] + weights[i];

	double a = double(rand())/RAND_MAX*(1-1e-12);  // a is range selector. Must be in [0,1)
	a *= cumm_prob[n];				// transform a into [0, sum(weights) )

	int r = bin_search_lub(a, weights, n+1);

	// we want r-1 because upper bound is the right edge of the range for the desired element
	return r-1;
	
}  


/*---------------------------------------------------------------------------------------------

 Sampling using rejection algorithm

---------------------------------------------------------------------------------------------*/
inline int sample_reject(float * weights, int n){
	bool accept = false;
	int r = -1;
	while(!accept){
		int chosen_one = rand() % n;
		if (double(rand())/RAND_MAX*(1-1e-12) < weights[chosen_one]){
			r = chosen_one;
			accept=true;
		}
	}
	return r;
}




#endif

