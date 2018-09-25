/*
 * A very simple library to create colours and palettes 
 *
 * Jaideep Joshi (24 Dec 2013)
 *
 */


#ifndef _PALETTES_H_
#define _PALETTES_H_


#include <iostream>
#include <vector>
#include <cstdlib>		// for random numbers
using namespace std;

class Colour_rgb{
	public:
	float r;
	float g;
	float b;
	
	Colour_rgb(){r=1; b=1; g=1;}
	Colour_rgb(float rr, float gg, float bb){r=rr; b=bb; g=gg;}
};



inline Colour_rgb HSVtoRGB(float h, float s, float v ){
	Colour_rgb C;
	
	if( s == 0 ) {
		// achromatic (grey)
		C.r = C.g = C.b = v;
		return C;
	}

	h = h*360.f/60.f;			// sector 0 to 5
	int i = int(h);
	float f = h - i;			// factorial part of h
	float p = v * ( 1 - s );
	float q = v * ( 1 - s * f );
	float t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			C.r = v;
			C.g = t;
			C.b = p;
			break;
		case 1:
			C.r = q;
			C.g = v;
			C.b = p;
			break;
		case 2:
			C.r = p;
			C.g = v;
			C.b = t;
			break;
		case 3:
			C.r = p;
			C.g = q;
			C.b = v;
			break;
		case 4:
			C.r = t;
			C.g = p;
			C.b = v;
			break;
		default:		// case 5:
			C.r = v;
			C.g = p;
			C.b = q;
			break;
	}

	return C;
}

//inline vector <Colour_rgb> createPalette_rainbow(int N){
//	vector <Colour_rgb> p(N);
//	for (int i=0; i<N; ++i){
//		float h = float(i)/N; float s=1, v=1;
//		p[i] = HSVtoRGB(h,s,v);
//	}
//	return p;	
//}

inline vector <Colour_rgb> createPalette_rainbow(int N, float start=0, float end=1){
	vector <Colour_rgb> p(N);
	for (int i=0; i<N; ++i){
		float h = start + i*(end-start)/N; float s=1, v=1;
		p[i] = HSVtoRGB(h,s,v);
	}
	return p;	
}

inline vector <Colour_rgb> createPalette_random(int N, float start=0, float end=1){
	vector <Colour_rgb> p(N);
	for (int i=0; i<N; ++i){
		p[i].r = start + (end-start)*rand()/RAND_MAX;
		p[i].g = start + (end-start)*rand()/RAND_MAX;
		p[i].b = start + (end-start)*rand()/RAND_MAX;
	}
	return p;	
}


inline vector <Colour_rgb> createPalette_grayscale(int N, float start=0, float end=1){
	vector <Colour_rgb> p(N);
	for (int i=0; i<N; ++i){
		p[i].r = start + (end-start)*float(i)/(N-1);
		p[i].g = start + (end-start)*float(i)/(N-1);
		p[i].b = start + (end-start)*float(i)/(N-1);
	}
	return p;	
}


inline vector <Colour_rgb> createPalette_ramp(int N, Colour_rgb start, Colour_rgb end){
	vector <Colour_rgb> p(N);
	for (int i=0; i<N; ++i){
		p[i].r = start.r + (end.r-start.r)*float(i)/(N-1);
		p[i].g = start.g + (end.g-start.g)*float(i)/(N-1);
		p[i].b = start.b + (end.b-start.b)*float(i)/(N-1);
	}
	return p;	
}


inline void printPalette(vector <Colour_rgb> &p){
	cout << "palette:\n";
	for (int i=0; i<p.size(); ++i){
		Colour_rgb c = p[i];
		cout << int(c.r*255) << "\t" << int(c.g*255) << "\t" << int(c.b*255) << "\n";
	}
	cout << '\n';
}

#endif

// sample program
// ------------------

//int main(){
//	int N = 30;
//	
//	vector <Colour_rgb> p = createPalette_rainbow(30);

//	for (int i=0; i<N; ++i){
//		Colour_rgb c = p[i];
//		cout << int(c.r*255) << "\t" << int(c.g*255) << "\t" << int(c.b*255) << "\n";
//	}
//	return 0;	
//}


