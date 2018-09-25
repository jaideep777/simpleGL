// ============================ SOME DEBUG UTILS ==========================//
//
//	Functions to get list of devices on machine and
//	get their properties.
//
//	It also chooses the GPU to use from the argument (argv)
//	
//	Compile with nvcc, no linking necessary
//
// ============================ SOME DEBUG UTILS ==========================//


#include <cuda_runtime.h>
#include <sstream>
using namespace std;

// ============================ SOME DEBUG UTILS ==========================//

// This will output the proper CUDA error strings in the event that a CUDA host call returns an error
#define checkCudaError(ans) { _checkCudaError(__FILE__, __LINE__, ans); }
inline void _checkCudaErrors(const char * file, int line, cudaError err){
	if( err != cudaSuccess) {
		cerr << file << "(" << line << ") : Cuda Runtime API Error " 
			 << int(err) << ": " << cudaGetErrorString(err) << ".\n";
		exit(-1);
	}
}

// This will output the proper error string when calling cudaGetLastError
#define getLastCudaError(ans) { _getLastCudaError(__FILE__, __LINE__, ans); }
inline void _getLastCudaError(const char *file, int line, string s=""){
	string errMessage = s;
	cudaError_t err = cudaGetLastError();
	if( err != cudaSuccess){
		cerr << file << "(" << line << ") : Last Cuda Error - " << errMessage 
			 << " (" << int(err) << "): " << cudaGetErrorString(err) << ".\n";
		exit(-1);
	}
}

inline int initDevice(int argc, char** argv){
	int deviceCount = 0;
	checkCudaErrors(cudaGetDeviceCount(&deviceCount));
	
	if (deviceCount == 0){
		cout << "There is no device supporting CUDA\n";
		exit(-1);
	}

	cout << "Found " << deviceCount << " CUDA Capable devices:\n";

	int dev, driverVersion = 0, runtimeVersion = 0;     
	for (dev = 0; dev < deviceCount; ++dev) {
		cudaDeviceProp deviceProp;
		cudaGetDeviceProperties(&deviceProp, dev);

		cudaDriverGetVersion(&driverVersion);
		cudaRuntimeGetVersion(&runtimeVersion);
  
		cout << dev << ") " << deviceProp.name << '\n';
		cout << "    Driver Version: " << driverVersion/1000 << "." << (driverVersion%100)/10 << '\n';
		cout << "    Runtime Version: " << runtimeVersion/1000 << "." << (runtimeVersion%100)/10 << '\n';
		cout << "    Compute Capability: " << deviceProp.major << "." << deviceProp.minor << '\n';
		cout << "    Multiprocessors: " <<  deviceProp.multiProcessorCount << '\n';
//		cout << "Cores: " <<  deviceProp.multiProcessorCount << " Multiprocessors x "
//			 << ConvertSMVer2Cores(deviceProp.major, deviceProp.minor) << " CUDA Cores/MP = " 
//			 << ConvertSMVer2Cores(deviceProp.major, deviceProp.minor) * deviceProp.multiProcessorCount 
//			 << "\n"; 

		cout << "    Max threads per multiprocessor:  " 
			 << deviceProp.maxThreadsPerMultiProcessor << '\n';
		cout << "    Max threads per block: " 
			 << deviceProp.maxThreadsPerBlock << '\n';
		cout << "    Shared Memory per block: " << deviceProp.sharedMemPerBlock/1024.0f << " kB\n";
		cout << "    Maximum sizes of each dimension of a block: "
			 << deviceProp.maxThreadsDim[0] << " x " 
			 << deviceProp.maxThreadsDim[1] << " x " 
			 << deviceProp.maxThreadsDim[2] << '\n';
//		cout << "Maximum sizes of each dimension of a grid: "
//			 << deviceProp.maxGridSize[0] << " x " 
//			 << deviceProp.maxGridSize[1] << " x " 
//			 << deviceProp.maxGridSize[2] << '\n';
		cout << '\n';
	}

	int cDev = 0;	
	if (argc < 2) cDev=0;
	else {
		stringstream ss; ss.str(argv[1]);
		ss >> cDev;
	}

	if (cDev >=  deviceCount) {
		cout << "Cant find chosen device!\n";
		return -1;
	}

	cudaSetDevice(cDev);
	getLastCudaError("cuda_device_set");
	cout << "\n> Using device " << cDev << "...\n\n";

	return cDev;
}


