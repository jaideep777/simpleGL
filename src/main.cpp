#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <liblas/liblas.hpp>
using namespace std;

#include "../headers/graphics.h"
//#include "../utils/simple_initializer.h" 
#include "../utils/simple_math.h"

namespace fl{

struct vec3{
    float x;
    float y;
    float z;
};

}

bool compare_z(const fl::vec3& p1, const fl::vec3& p2){
    return (p1.z < p2.z);
}

bool compare_y(const fl::vec3& p1, const fl::vec3& p2){
    return (p1.y < p2.y);
}

bool compare_x(const fl::vec3& p1, const fl::vec3& p2){
    return (p1.x < p2.x);
}

void sort_by_y(float* begin, float* end){
    sort((fl::vec3*)begin, (fl::vec3*)end, compare_y); 
}
    
void sort_by_z(float* begin, float* end){
    sort((fl::vec3*)begin, (fl::vec3*)end, compare_z); 
}

void sort_by_x(float* begin, float* end){
    sort((fl::vec3*)begin, (fl::vec3*)end, compare_x); 
}


vector <int> y_slices(float * pos, int n, float res){
    vector <int> slice_indices(1,0);
    float min = int(pos[3*0+1]/res)*res;
    for (int i=0; i< n; ++i){
        if (pos[3*i+1] > (min + res*slice_indices.size()) ){
            slice_indices.push_back(i);
            cout << "slice: " << i << ", value = " << pos[3*i+1] <<  endl;
        }
    }
//    if (*slice_indices.end() != n-1) slice_indices.push_back(n-1);
    return slice_indices;
}

vector <int> z_slices(float * pos, int n, float res){
    vector <int> slice_indices(1,0);
    float min = int(pos[3*0+2]/res)*res;
    for (int i=0; i< n; ++i){
        if (pos[3*i+2] > (min + res*slice_indices.size()) ){
            slice_indices.push_back(i);
            cout << "slice: " << i << ", value = " << pos[3*i+2] <<  endl;
        }
    }
//    if (*slice_indices.end() != n-1) slice_indices.push_back(n-1);
    return slice_indices;
}


class DigitalElevModel{
    public:
	int nx,ny;
	float dx,dz,x0,z0,xmin,xmax,zmin,zmax;
	vector<float>elev;
//	DigitalElevModel(int _nx, int _ny, float _dx, float _dz, float _x0, float _z0){
//		nx   = _nx;			
//		ny   = _ny;			
//		dx   = _dx;			
//		dz   = _dz;			
//		x0   = _x0;			
//		z0   = _z0;	
//			
//	}
	DigitalElevModel(){}
	void init(int _nx, int _ny, float _dx, float _dz, float _x0, float _z0){
		nx   = _nx;			
		ny   = _ny;			
		dx   = _dx;			
		dz   = _dz;			
		x0   = _x0;			
		z0   = _z0;
		
		xmin = x0 - dx/2;
		zmin = x0 - dx/2;
		
		xmax = (dx*nx) + xmin; 
		zmax = (dz*ny) + zmin;
		
		elev.resize(nx*ny);
		
	}
	
	void initFromBounds(int _xmin, int _xmax, float _zmin, float _zmax, float _dx, float _dz){
		xmin = _xmin;			
		xmax = _xmax;			
		zmin = _zmin;			
		zmax = _zmax;			
		dx   = _dx;			
		dz   = _dz;
		nx   =  (xmax- xmin)/ dx ; 
		ny   =  (zmax- zmin)/ dz ; 
		x0   =  xmin + dx/2; 
		z0   =  zmin + dz/2;
		
		
		 			
	}
	
	vector<int> get_index(float x, float z){
		float ix = floor((x- xmin)/ dx);
		float iz = floor((z- zmin)/ dz);
		vector<int>index(2);
		index[0] = ix;
		index[1] = iz;
		return index;
	}
	
};


class PointCloud{
    public:
    int nverts;
    float dx,dz;
    int nx,ny;
    vector <float> points;
    DigitalElevModel dem;
    void read_las(string file){
        ifstream ifs;
        ifs.open(file.c_str(),ios::in | ios::binary);
        if (!ifs) cout << "Error Opening file: " << file << endl;

        liblas::ReaderFactory f;
        liblas::Reader reader = f.CreateWithStream(ifs);
        liblas::Header const& header = reader.GetHeader();
        cout << "Compressed: " << ((header.Compressed() == true) ? "true\n":"false\n");
        cout << "Signature: " << header.GetFileSignature() << '\n';
        cout << "Points count: " << header.GetPointRecordsCount() << '\n';
        nverts = header.GetPointRecordsCount();

        points.reserve(3*nverts);
        
        while (reader.ReadNextPoint()){ //count<10
            liblas::Point const& p = reader.GetPoint();
            points.push_back(p.GetX());
            points.push_back(p.GetY());
            points.push_back(p.GetZ());

            //cout << p.GetX() << ", " << p.GetY() << ", " << p.GetZ() << "\n";
        }

    }
    
    void createDEM(){
		float xmin = min_element((fl::vec3*)points.data(), (fl::vec3*)(points.data()+3*nverts), compare_x)->x;
		float xmax = max_element((fl::vec3*)points.data(), (fl::vec3*)(points.data()+3*nverts), compare_x)->x;
		float zmin = min_element((fl::vec3*)points.data(), (fl::vec3*)(points.data()+3*nverts), compare_z)->z;
		float zmax = max_element((fl::vec3*)points.data(), (fl::vec3*)(points.data()+3*nverts), compare_z)->z;
		cout << "x range: " << xmin << " " << xmax << endl;      
		cout << "z range: " << zmin << " " << zmax << endl;        
		
		xmin = floor(xmin/dx)*dx;
		xmax = ceil(xmax/dx)*dx;
		zmin = floor(zmin/dz)*dz;
		zmax = ceil(zmax/dz)*dz;
		dem.initFromBounds(xmin,xmax,zmin,zmax,0.5,0.5);
		//dem.init(nx,ny,dx,dz,xmin,zmin);
	}
	
	
    
};


int main(int argc, char **argv){

    PointCloud cr;
    cr.read_las("/home/jaideep/Data/LIDAR/2017-02-13_20-51-49.las");
	//cr.createDEM(0.5,0.5);
	

    init_hyperGL(&argc, argv);

    Palette p(100);
    p.create_rainbow();
//    p.print();
    
//    float pos11[] = {-1.1, 1, 1.1, -2.2, 2, 2.2, -3.3, 3, 3.3};
//    vector <float> cols11z = p.map_values(&pos11[1], 3, 3); 

    cout << "sort...\n";
//    sort_by_z(cr.points.data(), cr.points.data()+3*cr.nverts); 
    sort_by_y(cr.points.data(), cr.points.data()+3*cr.nverts); 
    for (int i=0; i<10; ++i) cout << cr.points[3*i] << " " << cr.points[3*i+1] << " " << cr.points[3*i+2] << endl;

    vector <float> cols9z = p.map_values(&cr.points[2], cr.nverts, 3);

    Shape pt(cr.nverts, 3, "points", false); //, 4, -1, 1);
//    pt.createShaders();
//    pt.pointSize = 1;
    pt.setVertices(cr.points.data());    
//    pt.createColorBuffer();
//    pt.setColors(col9);
    pt.setColors(&cols9z[0]);
    vector <float> ex = calcExtent(cr.points.data(), cr.nverts, 3);
    pt.setExtent(ex);
 
 
//    vector <int> slices = z_slices(cr.points.data(), cr.nverts, 0.1);
//    vector <int> slices = z_slices(cr.points.data(), cr.nverts, 1);
    vector <int> slices = y_slices(cr.points.data(), cr.nverts, 10);
    for (int i=0; i<slices.size(); ++i){
        cout << "slices: " << slices[i] << ": " << cr.points[3*slices[i]+2] << "\n";
    }
    cout << endl;

    vector <Shape*> slices_shapes(slices.size());
    for (int i=1; i<slices.size(); ++i){
        int nv = slices[i]-slices[i-1];
        slices_shapes[i-1] = new Shape(nv, 3, "points", false);
        slices_shapes[i-1]->pointSize = 1;
        slices_shapes[i-1]->setVertices(&cr.points[3*slices[i-1]]);
        slices_shapes[i-1]->setColors(&cols9z[4*slices[i-1]]);
        slices_shapes[i-1]->setExtent(ex);
    }
    slices_shapes[0]->b_render = true;

    int current_render = 0;

	float pos3[] = {-20,0,0, 100,0,0, 0,-20,0, 0,100,0, 0,0,-20, 0,0,100};
    float col3[] = {1,0,0, 0.5    ,
                     1,0,0, 0.5,
                     0,1,0, 0.5,
                     0,1,0, 0.5,
                     0.0,0.8,1, 0.5,
                     0.0,0.8,1, 0.5
                   };
    
    Shape axis(6, 3, "lines");
    axis.setVertices(pos3);
    axis.setColors((float*)col3);
    
    
//    axis.autoExtent(pos3);

//    Palette p(20);
//    p.create_grayscale();
//    p.print();
//    
//    float v[] = {0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4};
//    vector <glm::vec4> c = p.map_values(v, 8);
//    for (int i=0; i<8; ++i) cout << c[i].r << " " << c[i].g << " " << c[i].b << " " << c[i].a << "\n";
//    cout << endl; 
    
    
    while(1){       // infinite loop needed to poll anim_on signal.
        slices_shapes[current_render]->b_render = false;
        current_render = generic_count % (slices_shapes.size()-1);
//        cout << "Currently rendering: " << current_render << "\n";
        slices_shapes[current_render]->b_render = true;
        
        glutMainLoopEvent();
        usleep(20000);
    }
    // launch sim end.
    
    return 0;
}



