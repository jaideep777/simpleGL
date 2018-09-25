#ifndef GRAPHER_H__
#define GRAPHER_H__

// ============================================================================
//
//	This is a Utility for drawing graphs in realtime in an active
//	OpenGL window.
//	
//	It assumes that the OpenGL window coordinates span from 
//	0 to 100 in both x & y coordinates.
//	
//	It is upto the user to ensure this by setting the appropriate 
//	projection and modelview matrices before calling functions from this file 
//
// 	to compile, link with glut: -lglut
// ============================================================================


#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
#include <GL/freeglut.h>
using namespace std;

#include "simple_io.h"
#include "simple_math.h"
#include "simple_palettes.h"



// ========= some drawing utils =============


inline void drawString(string s){
	for (int i=0; i<s.size(); ++i){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s.c_str()[i]);
	}
}

inline void drawCircle(float R, int N=32, bool fill=false){
	if (!fill) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
		//glColor3f(0.5,0.5,0.5);
		for (int i=0; i<N; ++i){
			glVertex2f(R*cos(i*2*pi/N), R*sin(i*2*pi/N));
		}
		glVertex2f(R,0.0f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


// ============================================================================

class BarPlotParams{
	public:
	float mar_t;		// margins
	float mar;
	float mar_b;
	float t;			// space between axis and text
	float f;			// space between bars as fraction of bar width
	float s;			// preferred width of labels
	float ch_nb;		// number of bars below which labels scale will not change
	bool isHist;		// is this a histogram?
	int pointSize;

	BarPlotParams(){
		mar_t = 10;
		mar = 5;
		mar_b = 20;
		t = 4;
		f = 0.5;
		s = 10;
		ch_nb = 7;		// number of bars below which labels scale will not change
		isHist = false;
		pointSize =2;
	}
};

class ScatterParams{
	public:
	float mar_l;
	float mar;
	float mar_b;
	float t;
	float ch_nb;		
	int pointSize;

	ScatterParams(){
		mar_l = 20;
		mar = 7;
		mar_b = 20;
		t = 4;
		ch_nb = 7;		
		pointSize =2;
	}
};

// ================ PLOTTING Functions ===============================

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// bar plots
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

inline int bar(vector <float> &x, vector <float> &y, float alpha=0.3, Colour_rgb c = Colour_rgb(),
				BarPlotParams par = BarPlotParams() ){

		// plot area params
	float nx = x.size();
	float ny = y.size();
	if (!par.isHist && nx != ny) cout << "BARPLOT ERROR!!\n\n";
	if ( par.isHist && nx != ny+1) cout << "HIST ERROR!!\n\n";

	float l = 100-par.s-par.t-2*par.mar;
	float d = (100-par.mar_t-par.mar_b)/(ny+(ny-1)*par.f);
	float x0 = par.mar+par.s+par.t;
	float y0 = par.mar_b;

	float sc = l/arrayMax(&y[0], y.size());	// scale

	// draw bars
	glBegin(GL_QUADS);
		glColor4f(c.r,c.g,c.b, alpha);
		for (int i=0; i<ny; ++i){
			float y1 = y0+i*d*(1+par.f);
			glVertex2f(x0, y1); glVertex2f(x0+sc*y[i], y1); 
			glVertex2f(x0+sc*y[i], y1+d); glVertex2f(x0, y1+d);
		}
	glEnd();
	
	// draw bin labels
	glColor4f(1,1,1,0.7);
	for (int i=0; i<nx; ++i){
		glPushMatrix();
			float scale = min(0.25/nx, 0.25/par.ch_nb);
			
			float yoff = (par.isHist)? -par.f*d/2 : d/2;
			glTranslatef(x0-44*par.s*scale, par.mar_b+i*d*(1+par.f)+ yoff -60*scale, 0);
			
			glScalef(scale,scale,scale);
			drawString(as_string(x[i]));
		glPopMatrix();
	}

}

inline int bar2(vector <float> &x, vector <float> &y1, vector <float> &y2, float alpha=0.4, 
				Colour_rgb c1 = Colour_rgb(1,0,0), Colour_rgb c2 = Colour_rgb(0,1,0),
				BarPlotParams par = BarPlotParams() ){

	// plot area params
	float nx = x.size();
	float ny1 = y1.size();
	if (!par.isHist && nx != ny1)  cout << "BARPLOT2 ERROR: nx != ny1\n\n";
	if (ny1 != ny1) 			   cout << "BARPLOT2 ERROR: y1 & y2 not of equal length!!\n\n";
	if (par.isHist && nx != ny1+1) cout << "HIST2 ERROR:    nx != ny+1\n\n";

	float l = 100-par.s-par.t-2*par.mar;
	float d = (100-par.mar_t-par.mar_b)/(ny1+(ny1-1)*par.f);
	float x0 = par.mar+par.s+par.t;
	float y0 = par.mar_b;

	float M1 = arrayMax(&y1[0], y1.size());
	float M2 = arrayMax(&y2[0], y2.size());
	float sc = l/(fmax(M1,M2)+1e-6);	// scale - 1e-6 to avoid division by 0

	// draw bars
	glBegin(GL_QUADS);
		for (int i=0; i<ny1; ++i){
			float yi = y0+i*d*(1+par.f);
			glColor4f(c1.r,c1.g,c1.b, alpha);
			glVertex2f(x0, yi); glVertex2f(x0+sc*y1[i], yi); 
			glVertex2f(x0+sc*y1[i], yi+d/2); glVertex2f(x0, yi+d/2);

			glColor4f(c2.r,c2.g,c2.b, alpha);
			glVertex2f(x0, yi+d/2); glVertex2f(x0+sc*y2[i], yi+d/2); 
			glVertex2f(x0+sc*y2[i], yi+d); glVertex2f(x0, yi+d);
		}
	glEnd();
	
	// draw bin labels
	glColor4f(1,1,1,0.7);
	for (int i=0; i<nx; ++i){
		glPushMatrix();
			float scale = min(0.25/nx, 0.25/par.ch_nb);
			
			float yoff = (par.isHist)? -par.f*d/2 : d/2;
			glTranslatef(x0-44*par.s*scale, par.mar_b+i*d*(1+par.f)+ yoff -60*scale, 0);
			
			glScalef(scale,scale,scale);
			drawString(as_string(x[i]));
		glPopMatrix();
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Histogram plots
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline int hist2(vector <float> &x, vector <float> &y1, vector <float> &y2, float alpha=0.4, 
				  Colour_rgb c1 = Colour_rgb(1,0,0), Colour_rgb c2 = Colour_rgb(0,1,0),
				  BarPlotParams par = BarPlotParams() ){
			  
	par.isHist = true;	
	bar2(x, y1, y2, alpha, c1, c2, par);
}

inline int hist(vector <float> &x, vector <float> &y, float alpha=0.4, Colour_rgb c = Colour_rgb(1,1,1), 
			 BarPlotParams par = BarPlotParams() ){
			  
	par.isHist = true;	
	bar(x, y, alpha, c, par);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PIE chart
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline int pie2(float p, float R, float alpha=0.4, 
		 Colour_rgb c1 = Colour_rgb(0,1,1), Colour_rgb c2 = Colour_rgb(1,0,1)){
		 
	int N = 32;
	
	int n = p*N+1;
	// sector 1
	glBegin(GL_POLYGON);
		glColor4f(c1.r,c1.g,c1.b, alpha);
		glVertex2f(0,0);
		for (int i=0; i<n; ++i){
			glVertex2f(-R*cos(i*2*pi/N+pi/2), R*sin(i*2*pi/N+pi/2));
		}
		glVertex2f(-R*cos(p*2*pi+pi/2), R*sin(p*2*pi+pi/2));
		//glVertex2f(0,0);
	glEnd();
		
	// sector 2
	glBegin(GL_POLYGON);
		glColor4f(c2.r,c2.g,c2.b, alpha);
		glVertex2f(0,0);
		glVertex2f(-R*cos(p*2*pi+pi/2), R*sin(p*2*pi+pi/2));
		for (int i=n; i<=N; ++i){
			glVertex2f(-R*cos(i*2*pi/N+pi/2), R*sin(i*2*pi/N+pi/2));
		}
	glEnd();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// scatter plot
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

inline int scatter(vector <float> &x, vector <float> &y, vector <int> colours = vector <int> (1,0), 
				   vector <Colour_rgb> palette = vector <Colour_rgb> (1),
				   bool logx = false, float alpha=0.8, int nticksx = 11, ScatterParams par = ScatterParams()){

	int nx = x.size();

	float l = 100-par.mar_l-par.mar;
	float h = 100-par.mar_b-par.mar;

	float xmax = arrayMax(&x[0], x.size());
	float xmin = arrayMin(&x[0], x.size());
	if (logx) {
		xmax = log(xmax); xmin = log(xmin);
	}
	float dx = xmax - xmin;

	
	float ymax = arrayMax(&y[0], y.size());
	float ymin = arrayMin(&y[0], y.size());
	float dy = ymax - ymin;

	glPointSize(3);
	glBegin(GL_POINTS);
		for (int i=0; i<nx; ++i){

			Colour_rgb c1 = palette[colours[i%colours.size()]];
			glColor4f(c1.r,c1.g,c1.b, alpha);
			
			float xi;
			if (logx) xi = par.mar_l + (log(x[i])-xmin)/dx*l;
			else 	  xi = par.mar_l + (x[i]-xmin)/dx*l;
			float yi = par.mar_b + (y[i]-ymin)/dy*h;
			glVertex2f(xi, yi);
		}
	glEnd();
	
	glColor4f(1,1,1,0.3);
	glBegin(GL_LINES);
		glVertex2f(par.mar_l, par.mar_b); glVertex2f(100-par.mar, par.mar_b);
		glVertex2f(par.mar_l, par.mar_b); glVertex2f(par.mar_l, 100-par.mar);
	glEnd();

	glColor4f(1,1,1,0.5);
	
	// x axis
	glPointSize(1);
	int Nx = nticksx;
	for (int i=0; i<Nx; ++i){
		glPushMatrix();
			float scale = 0.2/Nx;
			
			float yoff = par.t + 20*scale;
			glTranslatef(par.mar_l + i*l/(Nx-1), par.mar_b - yoff, 0);
			glPushMatrix();	// not necessary?
			glBegin(GL_LINES); glVertex2f(0,1 +yoff); glVertex2f(0,-1 + yoff); glEnd();	// ticks
			glPopMatrix();
			glScalef(scale,scale,scale);
			if (!logx) drawString(as_string(xmin+i*(xmax-xmin)/(Nx-1), 3));
			else 	   drawString(as_string(exp(xmin+i*(xmax-xmin)/(Nx-1)), 3));
		glPopMatrix();
	}
	
	// y axis
	Nx = 11;
	for (int i=0; i<Nx; ++i){
		glPushMatrix();
			float scale = 0.2/Nx;
			
			float xoff = par.t + 400*scale;
			glTranslatef(par.mar_l -xoff, par.mar_b + i*h/(Nx-1), 0);
			glPushMatrix();	// not necessary?
			glBegin(GL_LINES); glVertex2f(xoff-1,0); glVertex2f(xoff+1,0); glEnd();	// ticks
			glPopMatrix();
			glScalef(scale,scale,scale);
			drawString(as_string(ymin+i*(ymax-ymin)/(Nx-1), 3));
		glPopMatrix();
	}

	
}


#endif


