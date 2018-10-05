# MAKEFILE FOR CUDA

#-------------------------------------------------------------------------------
# executable name
TARGET := 1

# files
CCFILES  :=  $(wildcard *.cpp) 
CUFILES  :=  $(wildcard *.cu) 

# ------------------------------------------------------------------------------

# paths
CUDA_INSTALL_PATH ?= /usr/local/cuda#-5.0

# compilers
NVCC := nvcc -O3 #--ptxas-options=-v #,-abi=no
CXX  := g++ -O3
LINK := g++ 

# include and lib dirs (esp for cuda)
INC_PATH := -I$(CUDA_INSTALL_PATH)/include
LIB_PATH := -L$(CUDA_INSTALL_PATH)/lib64 -L/usr/local/lib 
GLLIB_PATH := -L/usr/lib/nvidia-331

# flags
ARCH := sm_20
COMMONFLAGS = -m64 
NVCCFLAGS += $(COMMONFLAGS) -arch=$(ARCH) 
CXXFLAGS += $(COMMONFLAGS) $(CXXWARN_FLAGS) 
LINKFLAGS += $(COMMONFLAGS) 

# libs
#LIBS = -lcudart 					# cuda libs 		-lcutil_x86_64 -lshrutil_x86_64
GLLIBS = -lGL -lglut -lGLU -lGLEW 				# openGL libs       -lGL -lGLEW  #-lX11 -lXi -lXmu 		
LIBS = 	 -lcudart -lcurand -lcufft -lgsl -lgslcblas -ljpeg	# additional libs

# files
CC_OBJS  := $(CCFILES:.cpp=.o) # $(patsubst %.cpp, %.o, $(CPP_SOURCES))
CU_OBJS  := $(CUFILES:.cu=.cu_o) # $(patsubst %.cu, %.cu_o, $(CU_SOURCES))

# common dependencies	
COM_DEP = 

all: $(TARGET)

$(TARGET): $(CC_OBJS) $(CU_OBJS)
	$(LINK) -o $(TARGET) $(LIB_PATH) $(GLLIB_PATH) $(CU_OBJS) $(CC_OBJS) $(LIBS) $(GLLIBS)

%.cu_o : %.cu $(COM_DEP)
	$(NVCC) $(NVCCFLAGS) $(INC_PATH) -o $@ -c $<

%.o: %.cpp $(COM_DEP)
	$(CXX) $(CXXFLAGS) $(INC_PATH) -o $@ -c $< 

clean:
	rm -f nohup.out *.o *.cu_o
	
re: clean all

# ------------------------------------------------------------------------------





#-gencode=arch=compute_10,code=\"sm_10,compute_10\"  -gencode=arch=compute_20,code=\"sm_20,compute_20\"  -gencode=arch=compute_30,code=\"sm_30,compute_30\" 

#-W -Wall -Wimplicit -Wswitch -Wformat -Wchar-subscripts -Wparentheses -Wmultichar -Wtrigraphs -Wpointer-arith -Wcast-align -Wreturn-type -Wno-unused-function 
#-m64 -fno-strict-aliasing 
#-I. -I/usr/local/cuda/include -I../../common/inc -I../../../shared//inc 
#-DUNIX -O2


#g++ -fPIC   -m64 -o ../../bin/linux/release/swarming_chasing_predator obj/x86_64/release/genmtrand.cpp.o  obj/x86_64/release/simpleGL.cu.o  -L/usr/local/cuda/lib64 -L../../lib -L../../common/lib/linux -L../../../shared//lib -lcudart   
#-lGL -lGLU -lX11 -lXi -lXmu -lGLEW_x86_64 -L/usr/X11R6/lib64 -lGLEW_x86_64 -L/usr/X11R6/lib64 -lglut 
#-L/usr/local/cuda/lib64 -L../../lib -L../../common/lib/linux -L../../../shared//lib -lcudart 
#-L/usr/lib -lgsl -lgslcblas 
#-lcutil_x86_64  -lshrutil_x86_64 




#CXXWARN_FLAGS := \
#	-W -Wall \
#	-Wimplicit \
#	-Wswitch \
#	-Wformat \
#	-Wchar-subscripts \
#	-Wparentheses \
#	-Wmultichar \
#	-Wtrigraphs \
#	-Wpointer-arith \
#	-Wcast-align \
#	-Wreturn-type \
#	-Wno-unused-function \
#	$(SPACE)

#CWARN_FLAGS := $(CXXWARN_FLAGS) \
#	-Wstrict-prototypes \
#	-Wmissing-prototypes \
#	-Wmissing-declarations \
#	-Wnested-externs \
#	-Wmain \
#	
	
#HEADERS  := $(wildcard *.h)
	
