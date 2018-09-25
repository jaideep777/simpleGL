#ifndef IO_UTILS_H__
#define IO_UTILS_H__

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
#include <cstring>
using namespace std;



template <class T>
string as_string(T f, int prec = 5, bool fmt_fixed = false){
	stringstream ss;
	if (fmt_fixed) ss << fixed;
	ss << setprecision(prec) << f;
	return ss.str();
}

inline float as_float(string s){
	float f;
	stringstream ss; ss.str(s);
	ss >> f;
	return f;
}

template <class T, class U>
void printMap(map <int, T> &m, map <int, U> &m2, string name = "map"){
	cout << name << ":\n";
	for (typename map<int, T>::iterator i = m.begin(); i!= m.end(); ++i){
		cout << i->first << "|" << i->second << ", " << m2[i->first] << '\n';
	}
	cout << '\n';
}


template <class T>
void printMap(map <int, T> &m, string name = "map"){
	cout << name << ":\n";
	for (typename map<int, T>::iterator i = m.begin(); i!= m.end(); ++i){
		cout << i->first << "|" << i->second << '\n';
	}
	cout << '\n';
}

//template <class T>
//void printArray(T *v, int n, string name = "v", string nameSep=" ", string endSep = ""){
//	cout << name << ":" << nameSep;
//	for (int i = 0; i< n; ++i){
//		cout << v[i] << " ";
//	}
//	cout << "\n" << endSep;
//}

inline void printTime(float clkt){
	int nhrs = clkt/1000.0f/60.f/60.f;
	int nmins = clkt/1000.0f/60.f - nhrs*60;
	float nsec = clkt/1000.0f - nhrs*60 - nmins*60*60; 
	cout << "| t = " << nhrs << ":" << nmins << ":" << nsec << " hh:mm:ss.s.\n";
}

template <class SRC, class DST>
inline DST* memcpy2D(DST * dst, SRC * src, int bytes_per_elem, int n_elements){
	if (bytes_per_elem > sizeof(DST)) {
		cout << "Error: overlapping copy (bytes per element > destination pitch)\n\n";
		return NULL;
	}
	DST * d = dst; SRC * s = src;
	for (int i=0; i<n_elements; ++i){
		memcpy((void*)d, (void*)s, bytes_per_elem);
		++d; ++s;
	}
	return dst;
}

inline vector <string> parse(string cmd){
	vector <string> tokens;
	stringstream ss; ss.str(cmd);
//	cout << "Tokens: \n";
	while (!ss.eof()){
		string s; ss >> s;
		tokens.push_back(s);
//		cout << s << "\n";
	} 
//	cout << "----\n";
	return tokens;
}


#endif


