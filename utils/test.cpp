#include <iostream>
//#include <algorithm>
#include <vector>
using namespace std;

#include "simple_io.h"
#include "simple_initializer.h"
//#include "cuda_device.h"


class Particle{
	public:
	float x;
	float v;
	int a;
	char c;
	float b;
	
	Particle(){
		a=1988; c='a'; v=5.56; x=18.10; b=1.66;
	}
};

template <class T>
void printVec(vector <T> &v, string name = "v"){
	cout << name << " = ";
	for (int i=0; i<v.size(); ++i) cout << v[i] << " " ;
	cout << '\n';
}

void printParticles(vector <Particle> &p){
	cout << "x   " << "\t" << "v   " << "\t" << "a   " << "\t" << "c   " << "\t" << "b   " << "\n";
	for (int i=0; i<p.size(); ++i){
		cout << p[i].x << "\t" << p[i].v << "\t" << p[i].a << "\t" << p[i].c << "\t" << p[i].b << "\n";
	}
	cout << "\n";
}

/*
int main(){
	

	// ====== memcpy2D test ====================
	
	const int N = 10;
	vector <float> pos(N, 19.99);
	
	vector <Particle> p_all(N);
	float * pos_dev = new float[N];
	float * vel_dev = new float[N];
	char * c_dev = new char[N];
	
	printParticles(p_all);

	memcpy2D(  (float*)pos_dev, (Particle*)&(p_all[0].x), sizeof(float), N);
	memcpy2D(  (float*)vel_dev, (Particle*)&(p_all[0].v), sizeof(float), N);
	memcpy2D(  (char*)c_dev,   (Particle*)&(p_all[0].c), sizeof(char), N);
	
	printArray(pos_dev, N, "posdev");
	printArray(vel_dev, N, "veldev");
	printArray(c_dev, N, "cdev");

	// operate on device arrays
	for (int i=0; i<N; ++i) pos_dev[i] = 19.99+i;
	for (int i=0; i<N; ++i) vel_dev[i] = 12.1-i;
	for (int i=0; i<N; ++i) c_dev[i] = 'a'+i;
	
	printArray(pos_dev, N, "posdev");
	printArray(vel_dev, N, "veldev");
	printArray(c_dev, N, "cdev");
	
	memcpy2D(  (Particle*)&(p_all[0].x), (float*)pos_dev, sizeof(float), N);
	memcpy2D(  (Particle*)&(p_all[0].v), (float*)vel_dev, sizeof(float), N);
	memcpy2D(  (Particle*)&(p_all[0].c), (char*)c_dev, sizeof(char), N);
	
	printParticles(p_all);
	
	return 0;

}
*/

#include <unistd.h>
int main(int argc, char ** argv){

//	short a[9] = {1,-1,3,4,-4,6,7,8,9};
//	vector <int> va(a,a+9);

//	int cDevice = initDevice(argc, argv);

//	cout << "array sum = " << sizeof(arraySum(a, 9)) <<  " " << arraySum(a, 9)
//		 << " " << arrayMax(a,9) << " " << arrayMin(a,9) << '\n';
//	cout << "vector max = " << *max_element(va.begin(), va.end()) << '\n';
	
//	vector <float> rn(10000);
//	for (int i=0; i<10000; ++i) rn[i] = rnorm();
//	
//	Histogram h(rn,20);
//	h.plot_console();
//	
//	float d = 3.14159265;
//	cout << d << '\n';
//	cout << "d = " << setprecision(8) << fixed << d << '\n';
//	std::cout.copyfmt(std::ios(NULL));
//	cout << as_string(d,4, true) << '\n';

//		int ngen = 2000; int tnow = 0;
//		
//		SimpleProgressBar prog(&ngen, "test");
//		
//		prog.start();
//		for (int i=0; i<ngen; ++i){
//			//tnow = i;
//			
//			usleep(1000);
//			prog.update();//print();
//			
//		}
//		//prog.stop();

//	Initializer I("../exec_config/execution_config.r");
//	I.readFile();
//	I.printVars();
//	
//	int genMax = (int) I.getScalar("genMax");
//	cout << genMax << '\n';
//	I.getArray("c", -1);
//	I.getString("jj");
	
	parse("hi my name is jaideep 123 is 456 haha");

//	
	return 0;
}


