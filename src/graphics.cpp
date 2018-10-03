#include "../headers/graphics.h"

#include "../utils/simple_io.h"
#include "../utils/simple_math.h"
#include "../utils/simple_histogram.h"


Renderer * glRenderer = NULL;


void loadShader(string filename, GLuint &shader_id, GLenum shader_type){

	ifstream fin(filename.c_str());
	string c((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	const char * glsl_src = c.c_str();

	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &glsl_src, NULL);
	glCompileShader(shader_id);
	if (GL_NO_ERROR != glGetError()) cout << "Error compiling shader!\n";
}


void Shape::createShaders(){
	GLenum ErrorCheckValue = glGetError();

	cout << "creating shaders for " << objName << "... "; 

	loadShader(vertexShaderFile, vertexShader_id, GL_VERTEX_SHADER);	
	loadShader(fragmentShaderFile, fragmentShader_id, GL_FRAGMENT_SHADER);

	program_id = glCreateProgram();
	glAttachShader(program_id, vertexShader_id);
	glAttachShader(program_id, fragmentShader_id);
	glLinkProgram(program_id);
	glDeleteShader(fragmentShader_id);
	glDeleteShader(vertexShader_id);
	
	
	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR){
		cout << "ERROR: Could not create the shaders: " << gluErrorString(ErrorCheckValue) << endl;
	}

	cout << "DONE." << endl;
}


void Shape::deleteShaders(){
	glUseProgram(0);

	glDetachShader(program_id, vertexShader_id);
	glDetachShader(program_id, fragmentShader_id);

//	glDeleteShader(fragmentShader_id);
//	glDeleteShader(vertexShader_id);

	glDeleteProgram(program_id);
}

void Shape::useProgram(){
	glUseProgram(program_id);
}

void Shape::setRenderVariable(string s, float  f){
	GLuint loc = glGetUniformLocation(program_id, s.c_str());
	glUniform1f(loc, f);
}

void Shape::setRenderVariable(string s, float2 f){
	GLuint loc = glGetUniformLocation(program_id, s.c_str());
	glUniform2f(loc, f.x, f.y);
}

void Shape::setRenderVariable(string s, float3 f){
	GLuint loc = glGetUniformLocation(program_id, s.c_str());
	glUniform3f(loc, f.x, f.y, f.z);
}

void Shape::setRenderVariable(string s, float4 f){
	GLuint loc = glGetUniformLocation(program_id, s.c_str());
	glUniform4f(loc, f.x, f.y, f.z, f.w);
}

void Shape::setShaderVariable(string s, glm::mat4 f){
	GLuint loc = glGetUniformLocation(program_id, s.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(f));
}

// ===========================================================
// class Palette 
// ===========================================================

glm::vec4 HSVtoRGB(float h, float s, float v ){
	glm::vec4 C;
	
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

Palette::Palette(int _n){
	n = _n;
	colors.resize(n);
}

void Palette::create_rainbow(float start, float end){
	for (int i=0; i<n; ++i){
		float h = start + i*(end-start)/n; float s=1, v=1;
		colors[i] = HSVtoRGB(h,s,v);
	}
}

void Palette::create_random(float start, float end){
	for (int i=0; i<n; ++i){
		colors[i].r = start + (end-start)*rand()/RAND_MAX;
		colors[i].g = start + (end-start)*rand()/RAND_MAX;
		colors[i].b = start + (end-start)*rand()/RAND_MAX;
	}
}


void Palette::create_grayscale(float start, float end){
	for (int i=0; i<n; ++i){
		colors[i].r = start + (end-start)*float(i)/(n-1);
		colors[i].g = start + (end-start)*float(i)/(n-1);
		colors[i].b = start + (end-start)*float(i)/(n-1);
	}
}


void Palette::create_ramp(glm::vec4 start, glm::vec4 end){
	for (int i=0; i<n; ++i){
		colors[i].r = start.r + (end.r-start.r)*float(i)/(n-1);
		colors[i].g = start.g + (end.g-start.g)*float(i)/(n-1);
		colors[i].b = start.b + (end.b-start.b)*float(i)/(n-1);
	}
}


void Palette::print(){
	cout << "palette:\n";
	for (int i=0; i<n; ++i){
		glm::vec4 c = colors[i];
		cout << int(c.r*255) << "\t" << int(c.g*255) << "\t" << int(c.b*255) << "\n";
	}
	cout << '\n';
}

vector <glm::vec4> Palette::map_values(float* v, int nval, float vmin, float vmax){
	vector <glm::vec4> cols(nval);
	
	float min_val = (vmin == 1e20f)? *min_element(v,v+nval):vmin;
	float max_val = (vmax == 1e20f)? *max_element(v,v+nval):vmax;

//	cout << "minmax:" <<  min_val << " " << max_val << endl;
//	colMax = max(fabs(colMax), fabs(colMin));
//	cout << "nCol = " << nCol << ", colMax = " << colMax << ", colMin = " << colMin << '\n';
	for (int i=0; i < nval; ++i) {
		glm::vec4 c;
		int colID = (v[i] - min_val)/(max_val-min_val)*(n-1);
//		cout << "colid = " << colID << endl;
		if (colID < 0 || colID > n-1){
			cols[i] = glm::vec4(0.f,0.f,0.f,1.f);
		}
		else{
			cols[i] = colors[colID];
		}
	}
	return cols;
}


// ===========================================================
// class Shape
// ===========================================================

Shape::Shape(string obj_name, int nVert, string _type){
	objName = obj_name;
	vertexShaderFile = "shaders/shader_vertex_" + obj_name + ".glsl";
	fragmentShaderFile = "shaders/shader_fragment_" + obj_name + ".glsl";
	//doubleBuffered = dbuff;
	//swap = 0;
	type = _type;
	nVertices = nVert;
	model = glm::mat4(1.0f);

	cout << "create " << objName << endl;
	// create vertex buffer
	glGenBuffers(1, &vbo_id);					// create buffer ids and store in array

	// create color buffer
	glGenBuffers(1, &colorBuffer_id);

	createShaders();
}

Shape::~Shape(){
	
	deleteShaders();
	
	cout << "destroy " << objName << endl;
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &colorBuffer_id);
}

void Shape::setVertices(void* data, int nbytesPerVertex){

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id); 	// Bring 1st buffer into current openGL context
	glBufferData(GL_ARRAY_BUFFER, nbytesPerVertex*nVertices, data, GL_DYNAMIC_DRAW); 
	// remove buffers from curent context. (appropriate buffers will be set bu CUDA resources)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}


void Shape::setColors(float4 *colData){
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_id);
	glBufferData(GL_ARRAY_BUFFER, nVertices*4*sizeof(float), colData, GL_DYNAMIC_DRAW); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Shape::render(){

	useProgram();
	
	// set the point size to match physical scale
	setRenderVariable("psize", 4);
	setShaderVariable("model", glRenderer->projection*glRenderer->view*model);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexAttribPointer(0, 			// index (as specified in vertex shader layout (location = index))
						  3, 			// size (1D/2D/3D/4D) (1-4)
						  GL_FLOAT, 	// data type of each component
						  GL_FALSE, 	// normalize?
						  0, 			// stride (space between consecutive values) 0 = tightly packed
						  0);			// 
	glEnableVertexAttribArray(0);	

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_id);
	glVertexAttribPointer(1, 
						  4, 
						  GL_FLOAT, 
						  GL_FALSE, 
						  0, 
						  0);
	glEnableVertexAttribArray(1);	

	if (type == "triangles") 	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	else if (type == "lines")  	glDrawArrays(GL_LINES, 0, nVertices);
	else if (type == "points") 	glDrawArrays(GL_POINTS, 0, nVertices);
//	else if (type == "quads") 	glDrawArrays(GL_QUADS, 0, nVertices);
	else 	  					glDrawArrays(GL_POINTS, 0, nVertices);
	
}



// ===========================================================
// class Colormap
// ===========================================================

//ColorMap::ColorMap(){
////	objName = "cmap";
////	doubleBuffered = false;
////	swap = 0;
////	nlevs = 100;
////	nx = 10;
////	nVertices = 100;
////	xmin = 0;
////	xmax = 100;
////	vertexShaderFile = "shaders/shader_vertex_" + objName + ".glsl";
////	fragmentShaderFile = "shaders/shader_fragment_" + objName + ".glsl";
////	type = "colormap";

////	palette = createPalette_grayscale(nlevs, 0.1, 0.9);
//}

//ColorMap::ColorMap(string obj_name, int _nlevs, int _nx, float _xmin, float _xmax){
//	objName = obj_name;
////	doubleBuffered = dbuff;
////	swap = 0;
//	nlevs = _nlevs;
//	nx = _nx;
//	nVertices = nx*nx;
//	xmin = _xmin;
//	xmax = _xmax;
//	vertexShaderFile = "shaders/shader_vertex_" + obj_name + ".glsl";
//	fragmentShaderFile = "shaders/shader_fragment_" + obj_name + ".glsl";
//	type = "colormap";

//	palette = createPalette_grayscale(nlevs, 0.1, 0.9);
//};


//void ColorMap::createGridCentres(float2 *cmap_tmp){
//	float dxC = (xmax-xmin)/nx;
//	for (int iy=0; iy<nx; ++iy){
//		for (int ix=0; ix<nx; ++ix){
//			cmap_tmp[iy*nx+ix].x = xmin+dxC*(2*ix+1)/2.0;
//			cmap_tmp[iy*nx+ix].y = xmin+dxC*(2*iy+1)/2.0;
//		}
//	} 

//}




//void ColorMap::render(){

//	useProgram();
//	
//	// set the coord system bounds for getting orthograhic projection in vertex-shader
//	setRenderVariable("bounds", make_float4(xmin, xmax, xmin, xmax));
//	
//	// set the point size to match physical scale
//	float window_size_x = min(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
//	float psize = window_size_x/nx;
//	//if (psize < 1) psize = 1;
//	setRenderVariable("psize", psize);

//	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);	

//	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_id);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(1);	

//	glDrawArrays(GL_POINTS, 0, nVertices);

////	swap = 1-swap;
//}




// ===========================================================
// class Renderer
// ===========================================================

void Renderer::init(){

	b_renderText = true;
	b_renderLabels = true;
	b_renderRs = true;
	b_renderConsole = false;
	b_renderGrid = false;
	b_renderAxes = false;
	b_renderColorMap = true;

	window_width = 512;
	window_height = 512;
	
//	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -100.0f) );
	camera_tx = camera_ty = camera_rx = camera_ry = 0;
	camera_s = 1;
	
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 100.0f), 
  		   			   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   			   glm::vec3(0.0f, 1.0f, 0.0f));
  	view = glm::translate(view, glm::vec3(camera_tx, camera_ty, 0));
  		   			   
	projection = glm::perspective(glm::radians(90.0f), float(window_width) / window_height, 0.1f, 1000.0f);
	//projection = glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.f, 100.0f);

	glm::vec4 a = projection*view*glm::vec4(1,0,0,1);
	cout << "Vec:" << a.x << " " << a.y << " " << a.z << " " << a.w << endl;

	int t = 50; //I.getScalar("dispInterval");
	if (t < 0) {
		nSkip = -t;
		updateMode = Step;
		displayInterval = 50;	// in this case, only title will be updated in timer function (at 20 fps)
	}
	else {
		displayInterval = t;
		nSkip = -1;
		updateMode = Time;
	}

	quality = 3; //I.getScalar("graphicsQual");

	// create colour palettes
	int n = 100;
	//palette = createPalette_rainbow(n, 0, 0.75);
	//palette_rand = createPalette_random(n);

}


int Renderer::getDisplayInterval(){
	return displayInterval;
}

int Renderer::addShape(Shape* shp){
	shapes_vec.push_back(shp);
}

void Renderer::togglePause(){
	b_paused = !b_paused;
}

void Renderer::toggleConsole(){
	command = "";
	b_renderConsole = !b_renderConsole;
}

void Renderer::toggleText(){
	b_renderText = !b_renderText;
}

void Renderer::toggleGrid(){
	b_renderGrid = !b_renderGrid;
}

void Renderer::toggleAxes(){
	b_renderAxes = !b_renderAxes;
}

void Renderer::receiveConsoleChar(char key){
	switch (key){
		case 27:	// esc
			b_renderConsole = false;
			cout << "Command-line turned off.\n";
		break;
				
		case 13:	// enter
			executeCommand();
		break;
		
		case 8:		// backspace
			if (command.size() != 0) command = command.substr(0, command.size()-1);
		break;
		
		default:
			command += key;
			//cout << "command = " << command << '\n';
		break;
	}
}


int Renderer::executeCommand(){
	vector <string> args = parse(command);
	
	if (args[0] == "exit"){
		b_renderConsole = false;
		cout << "Command-line turned off.\n";
	}

	if (args[0] == "set"){
		if (args[1] == "background" || args[1] == "bg" ){
			if (args.size() >= 5){
				float r = as_float(args[2]), g = as_float(args[3]), b = as_float(args[4]); //, a = as_float(args[5]);
				glClearColor(r,g,b,1);
			}
		}
	}

	else{}
	
	command = "";

}


string Renderer::makeTitle(){
    const unsigned char * ver = glGetString(GL_VERSION);
    stringstream sout; 
    sout << fixed << setprecision(1) //<< psys-> N << " Particles" //<< "GL" << ver 
    								 << ", kcps = " << 100 //psys->kernelCounter.fps
    								 << ", dcps = " << frameCounter.fps;
    								// << ", s = " << psys->igen << "." << psys->istep;
	return sout.str();
}


// =================================================================================
//
//			OpenGL functions and callbacks
//				Init				
//				Display
//				Timer
//				reshape
//				keypress
//				
//			These functions assume that
//				glRenderer points to a valid renderer R
//				psys points to a valid Particle System P
//				P is connect()-ed to R
//
// =================================================================================


// function to initialize opengl, as well as the default renderer
bool init_hyperGL(int *argc, char **argv){
	cout << "init GL" << endl;
	
	glRenderer = new Renderer;
	glRenderer->init();

	// init
	glutInit(argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitWindowSize(glRenderer->window_width, glRenderer->window_height);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello World!");

	glewExperimental = GL_TRUE;
	glewInit();

	// Callbacks
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPress);
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseMove);
//	glutIdleFunc(NULL);	// start animation immediately. Otherwise init with NULL	
	glutTimerFunc(glRenderer->getDisplayInterval(), timerEvent, 0);
//	glutCloseFunc(cleanup);
	
    // default initialization
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glGenVertexArrays(1, &glRenderer->vao_id);
	glBindVertexArray(glRenderer->vao_id);

    return true;
}

void cleanup(){
}



// ===================== DISPLAY FUNCTION ====================================//

void display(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRenderer->view = glm::lookAt(glm::vec3(0.0f, 0.0f, 100.0f), 
  		   			   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   			   glm::vec3(0.0f, 1.0f, 0.0f));
  	glRenderer->view = glm::translate(glRenderer->view, glm::vec3(glRenderer->camera_tx, glRenderer->camera_ty, 0));
  	glRenderer->view = glm::scale(glRenderer->view, glm::vec3(glRenderer->camera_s, glRenderer->camera_s, glRenderer->camera_s));
  	glRenderer->view = glm::rotate(glRenderer->view, glRenderer->camera_rx*0.1f, glm::vec3(1.f, 0.f, 0.f));
  	glRenderer->view = glm::rotate(glRenderer->view, glRenderer->camera_ry*0.1f, glm::vec3(0.f, 1.f, 0.f));

//	render all shapes in list
	for (int i=0; i<glRenderer->shapes_vec.size(); ++i){
		Shape * s = glRenderer->shapes_vec[i];
		s->render();
	}

	glRenderer->frameCounter.increment();	// calculate display rate

//	glutPostRedisplay();
	glutSwapBuffers();


}

// ============================ CALLBACKS ====================================//

void timerEvent(int value){
	
//	glRenderer->psys->animate();

    glutSetWindowTitle(glRenderer->makeTitle().c_str());

	if (glRenderer->updateMode == Time) glutPostRedisplay();
	glutTimerFunc(glRenderer->getDisplayInterval(), timerEvent, 0);
}


void reshape(int w, int h){
	int x = min(w,h); 	// keep window square
    // viewport
    glViewport(0, 0, x, x);
//	glRenderer->tailLen = glRenderer->tailLen_def * glRenderer->xmax*float(glRenderer->window_height)/float(x);	
}


void keyPress(unsigned char key, int x, int y){
	if (!glRenderer->b_renderConsole){		
		
		if (key == 32){
			glRenderer->togglePause();
		}
		
		else if (key == 27){
			cout << "\n\n~~~ Simulation ABORTED! ~~~\n\n";
			exit(0);
		}	

		else if (key == 'x'){
			glRenderer->toggleConsole();
			cout << "Command-line turned on.\n";
		}
		
		else{
		}

	}
	else{	// console is on. keys will be sent to command buffer.
		glRenderer->receiveConsoleChar(key);
	}
		
	glutPostRedisplay();

}

bool lMousePressed, rMousePressed, mMousePressed;
float mouse_x0=0, mouse_y0=0;

void mousePress(int button, int state, int x, int y){
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN){
				lMousePressed = 1;
				mouse_x0 = x;
				mouse_y0 = y;
			}
			else{
				lMousePressed = 0;
			} 
		break;

		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN){
				mMousePressed = 1;
				mouse_x0 = x;
				mouse_y0 = y;
			}
			else{
				mMousePressed = 0;
			} 
		break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN){
				rMousePressed = 1;
				mouse_x0 = x;
				mouse_y0 = y;
			}
			else{
				rMousePressed = 0;
			} 
			break;

		default:
		break;
	}
}

void mouseMove(int x, int y){
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	float w = glutGet(GLUT_WINDOW_WIDTH);
	if (lMousePressed == 1){
		glRenderer->camera_rx += 0.2*(y - mouse_y0);
		glRenderer->camera_ry += 0.2*(x - mouse_x0);
	}
	if (rMousePressed == 1){
		float r = (y - mouse_y0)/h;
		glRenderer->camera_s *= 1+r;
	}
	if (mMousePressed == 1){
		glRenderer->camera_ty -= 200*(y - mouse_y0)/h;	// -= because y is measured from top
		glRenderer->camera_tx += 200*(x - mouse_x0)/w;
	}
	mouse_y0 = y;
	mouse_x0 = x;
	glutPostRedisplay();

}



