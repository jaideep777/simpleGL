#ifndef SIMPLE_INITIALIZER
#define SIMPLE_INITIALIZER

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

class Initializer{
	private:
	string init_fname;
	ifstream fin;
	
	map <string, string> strings;
	map <string, float>  scalars;
	map <string, vector <float> > arrays;
	
	public:
	inline Initializer(){};
	inline Initializer(string fname){
		init_fname = fname;
	}

	inline void setInitFile(string fname){
		init_fname = fname;
	}
	inline void readFile(){
		fin.open(init_fname.c_str());
		if (!fin) {
			cout << "FATAL ERROR: Cannot open initializer file " << init_fname << '\n';
			exit(1);
		}
		
		string attr_begin = ">";
		string init_format = "    > STRINGS\n    ... \n\n    > SCALARS\n    ...\n\n    > ARRAYS \n    ...\n";
	
		string s, v;
		float f;
		vector <float> vf;
		
		while (fin >> s && s != attr_begin);	// read until 1st > is reached

		fin >> s; 
		if (s != "STRINGS") {
			cout << "FATAL ERROR: STRINGS section missing in Initializer file. Format must be:\n" << init_format << '\n'; 
			exit(1);
		}
		while (fin >> s && s != attr_begin){
			if (s == "") continue;	// skip empty lines
			if (s == "#") {getline(fin,s,'\n'); continue;}	// skip #-followed lines (comments)
			fin >> v;
			strings[s] = v;
		}

		fin >> s;
		if (s != "SCALARS") {
			cout << "FATAL ERROR: SCALARS section missing in Initializer file. Format must be:\n" << init_format << '\n'; 
			exit(1);
		}
		while (fin >> s && s != attr_begin){
			if (s == "") continue;	// skip empty lines
			if (s == "#") {getline(fin,s,'\n'); continue;}	// skip #-followed lines (comments)
			fin >> f;
			scalars[s] = f;
		}

		fin >> s;
		if (s != "ARRAYS") {
			cout << "FATAL ERROR: ARRAYS section missing in Initializer file. Format must be:\n" << init_format << '\n'; 
			exit(1);
		}
		while (fin >> s && s != attr_begin){
			if (s == "") continue;	// skip empty lines
			if (s == "#") {getline(fin,s,'\n'); continue;}	// skip #-followed lines (comments)
			while (fin >> f && f != -1) vf.push_back(f);
			arrays[s] = vf;
			vf.resize(0);
		}

		fin.close();
	}
	
	inline string getString(string s){
		map <string, string>::iterator it = strings.find(s);
		if (it != strings.end()) return it->second;
		else {
			cout << "FATAL ERROR: Could not find required variable " << s << " in initializer file.\n";
			exit(1);
		}
	}
	
	inline float getScalar(string s){
		map <string, float>::iterator it = scalars.find(s);
		if (it != scalars.end()) return it->second;
		else {
			cout << "FATAL ERROR: Could not find required variable " << s << " in initializer file.\n";
			exit(1);
		}
	}
	
	inline vector <float> getArray(string s, int size = -1){
		map <string, vector<float> >::iterator it = arrays.find(s);
		if (it == arrays.end()) {	// array not found
			cout << "FATAL ERROR: Could not find required array " << s << " in initializer file.\n";
			exit(1);
		}
		if (it->second.size() == 0){
			cout << "FATAL ERROR: Required array " << s << " is empty!\n"; 
			exit(1);
		}
		if (size == -1) return it->second;
		else if (size != it->second.size()) {
			cout << "FATAL ERROR: Incorrect size of array " << s << ". Required " << size << ", found " << it->second.size() << '\n'; 
			exit(1);
		}
		else return it->second;
	}
	
	inline void printVars(){
		cout << "-------:\n";
		cout << "STRINGS:\n";
		cout << "-------:\n";
		for (map<string, string>::iterator it = strings.begin(); it != strings.end(); ++it){
			cout << it->first << ": " << it->second << '\n';
		}
		cout << "-------:\n";
		cout << "SCALARS:\n";
		cout << "-------:\n";
		for (map<string, float>::iterator it = scalars.begin(); it != scalars.end(); ++it){
			cout << it->first << ": " << it->second << '\n';
		}
		cout << "-------:\n";
		cout << "ARRAYS:\n";
		cout << "------:\n";
		for (map<string, vector<float> >::iterator it = arrays.begin(); it != arrays.end(); ++it){
			cout << it->first << ": ";
			for (int i=0; i< it->second.size(); ++i) cout << it->second[i] << " ";
			cout << "\n";
		}
		cout << "\n";
	}
	
};



#endif

