#ifndef JAI_HISTOGRAM_H
#define JAI_HISTOGRAM_H

// =============================================================================
// 		The Histogram Class
// 		ADDED by : JAIDEEP
// 		21 Dec 2013
//
//		to compile, link with gsl: -lgsl, -lgslcblas
//
// =============================================================================

#include <gsl/gsl_histogram.h>
#include <iomanip>
#include <vector>

#include "simple_math.h"

class Histogram{
	public:
	gsl_histogram * h;
	
	inline Histogram();
	inline Histogram(vector <float> &data, int nbins, float range_min = 1e20, float range_max = 1e20);
	inline Histogram(vector <float> &data, vector <double> &breaks);

	inline Histogram(vector <float> &data, vector <float> &w, int nbins, float range_min = 1e20, float range_max = 1e20);
	inline Histogram(vector <float> &data, vector <float> &w, vector <double> &breaks);
	
	inline ~Histogram();
	
	inline int plot_console();
	inline vector <float> getCounts();
	inline vector <float> getMids();
	inline vector <float> getMids_log();
	inline vector <float> getBreaks();
	inline int convertToPdf();
};

inline Histogram::Histogram(){};

inline Histogram::Histogram(vector <float> &data, int nbins, float range_min, float range_max){

	h = gsl_histogram_alloc (nbins);

	if (range_min > 9e19) range_min = arrayMin(&data[0], data.size()); //cout << "min = " << data_min << '\n';
	if (range_max > 9e19) range_max = arrayMax(&data[0], data.size()); //cout << "min = " << data_min << '\n';

	if (range_min == range_max) {
		range_min -= 1e-3;
		range_max += 1e-3;
	}

	gsl_histogram_set_ranges_uniform (h, range_min, range_max);

	// create histogram
	gsl_histogram_reset(h);
	for (int i=0; i<data.size(); ++i) gsl_histogram_increment(h, data[i]);
}


inline Histogram::Histogram(vector <float> &data, vector <double> &breaks){
	h = gsl_histogram_alloc (breaks.size()-1);

	gsl_histogram_set_ranges (h, &breaks[0], breaks.size());

	// create histogram
	gsl_histogram_reset(h);
	for (int i=0; i<data.size(); ++i) gsl_histogram_increment(h, data[i]);
}


inline Histogram::Histogram(vector <float> &data, vector <float> &w, int nbins, float range_min, float range_max){

	h = gsl_histogram_alloc (nbins);

	if (range_min > 9e19) range_min = arrayMin(&data[0], data.size()); //cout << "min = " << data_min << '\n';
	if (range_max > 9e19) range_max = arrayMax(&data[0], data.size()); //cout << "min = " << data_min << '\n';

	gsl_histogram_set_ranges_uniform (h, range_min, range_max);

	// create histogram
	gsl_histogram_reset(h);
	for (int i=0; i<data.size(); ++i) gsl_histogram_accumulate(h, data[i], w[i]);
}


inline Histogram::Histogram(vector <float> &data, vector <float> &w, vector <double> &breaks){
	h = gsl_histogram_alloc (breaks.size()-1);

	gsl_histogram_set_ranges (h, &breaks[0], breaks.size());

	// create histogram
	gsl_histogram_reset(h);
	for (int i=0; i<data.size(); ++i) gsl_histogram_accumulate(h, data[i], w[i]);
}


inline Histogram::~Histogram(){
	gsl_histogram_free(h);
}

inline int Histogram::plot_console(){
	vector <float> b = getMids();
	cout << setprecision(2) << fixed;
	for (int i=0; i<h->n; ++i){
		cout << setw(7) << b[i] << " <- " << setw(7) << h->bin[i] << '\t';
		int max_count = gsl_histogram_max_val(h);
		int step = max_count/40 +1;
		for (int k=0; k<h->bin[i]; k+=step) cout << ".";
		cout << '\n';
	}
	cout.copyfmt(ios(NULL));	// reset cout state
	
	return 0;
}

inline vector <float> Histogram::getCounts(){
	vector <float> freq(h->n, 0);
	for (int i=0; i<h->n; ++i){
		freq[i] = h->bin[i];
	}
	return freq;
}

inline vector <float> Histogram::getMids(){
	vector <float> freq(h->n, 0);
	for (int i=0; i<h->n; ++i){
		freq[i] = (h->range[i] + h->range[i+1])/2;
	}
	return freq;
}

inline vector <float> Histogram::getMids_log(){
	vector <float> freq(h->n, 0);
	for (int i=0; i<h->n; ++i){
		freq[i] = sqrt(h->range[i] * h->range[i+1]);
	}
	return freq;
}

inline vector <float> Histogram::getBreaks(){
	vector <float> freq(h->n+1, 0);
	for (int i=0; i<h->n+1; ++i){
		freq[i] = h->range[i];
	}
	return freq;
}

inline int Histogram::convertToPdf(){
	float hsum = gsl_histogram_sum(h);
	for (int i=0; i<h->n; ++i){
		h->bin[i] /= (hsum+1e-6);
	}
	return 0;
	
}


// =============================================================================
// 		Data summaries
// 		ADDED by : JAIDEEP
// 		11 May 2015
// =============================================================================

template <class T> 
void printSummary(T * data, int n, string s = ""){
	T dat_sum = data[0], dat_max = data[0], dat_min = data[0];
	for (int i=1; i<n; ++i){
		dat_sum += data[i];
		dat_min = fmin(dat_min, data[i]);
		dat_max = fmax(dat_max, data[i]);
	}
	float dat_mean = float(dat_sum)/n;

	cout << s << " summary: " << dat_min << " - " << dat_mean << " - " << dat_max << endl;
	vector <float> v(n);
	for (int i=0; i<n; ++i) v[i] = data[i];
	Histogram h(v, 20);

	h.plot_console();
}


// test program

//int main(){
//	int N = 10000;
//	vector <float> data(N), w(N,1);
//	for (int i=0; i<N; ++i) {
//		data[i] = norm_randf()*10;
//		if (data[i]<0) w[i] = 0;
//	}
//	double breaks_array[] = {-20,-15,-10,-5,0,5,10,15,20};
//	vector <double> breaks(breaks_array, breaks_array+9);
//	
//	Histogram h(data,w,32,-40,40);
//	
//	h.plot_console();

//	vector <float> counts = h.getMids();
//	for (int i=0; i<counts.size(); ++i) cout << counts[i] << " ";
//	cout << '\n';
//	
//	return 0;
//}


#endif
