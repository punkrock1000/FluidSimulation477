#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include "GL/glut.h"
#else
#include <GL/freeglut.h>
#endif
#endif

#include <iostream>
#include <cmath>
#include <cstring>
#include <chrono>
#include <algorithm>
#include "utils.h"
#include "OctTree.h"
#include "tbb/tbb.h"

using namespace tbb;
using namespace std;

#define NUM_OF_PARTICLES 3000
#define NUM_NEIGHBORS 16
#define RADIUS 16
//Low corner of the initial tree
#define LC -64
//High corner of the tree
#define HC 64

/*CONSTANTS*/
//Mass of water particles
const static float PARTICLE_MASS = 65.0f;
const static float M_PI = atan(1) * 4;
//Variables required for the simulation
const static Vector3f G(0.0f, -9.81f, 0.0f);
//const static float M_PI = atan(1) * 4;
// Resting density
const static float REST_DENS = 1000.0f;
const static float GAS_CONST = 2000.0f;
const static float H = 16.0f;
const static float HSQ = H * H;
// Viscosity
const static float VISC = 20.0f;

const static float POLY6 = 315.0f / (64.0f * M_PI * pow(H, 9.0f));
const static float SPIKY_GRAD = -15.0f / (M_PI*pow(H, 6.0f));
const static float VISC_LAP = 15.0f / (2 * M_PI*pow(H, 3.0f));

const static float TIMESTEP = 0.033f;
const static float BOUND_DAMPING = -0.4f;

OctTree* myTree;
WaterParticle* partArray;

//Timer
using fast_clock = std::chrono::high_resolution_clock;
using time_unit = std::chrono::milliseconds;

auto t1 = fast_clock::now();
auto t2 = t1;

//Window parameters
int width = 1024;
int height = 768;
///* Ortho (if used) */
double _left = 0.0;		/* ortho view volume params */
double _right = 0.0;
double _bottom = 0.0;
double _top = 0.0;
double _zNear = 0.1;
double _zFar = 2000.0;
double fovy = 45.0;
double prev_z = 0;

//Model matrices
double _matrix[16];
double _matrixI[16];

/* Mouse Interface  */
int _mouseX = 0;		/* mouse control variables */
int _mouseY = 0;
bool _mouseLeft = false;
bool _mouseMiddle = false;
bool _mouseRight = false;

double _dragPosX = 0.0;
double _dragPosY = 0.0;
double _dragPosZ = 0.0;


void getNeighbors(WaterParticle* particles, int index)
{
	int neighbCount = 0;
	myTree->getNeighbors(&partArray[index], NUM_NEIGHBORS, RADIUS, neighbCount);
	partArray[index].neighbors = neighbCount;
}

void computeDensity(WaterParticle* particles, int index)
{
	particles[index].rho = REST_DENS;
	int neighborIdx = 0;
	for (int j = 0; j < particles[index].neighbors; ++j)
	{
		neighborIdx = particles[index].neighborIndexes[j];
		Vector3f distVec = particles[neighborIdx].r - particles[index].r;
		float dist = distVec.norm();
		dist *= dist;
		particles[index].rho += POLY6 * pow(HSQ - dist, 3.0f);
	}

	particles[index].p = GAS_CONST * (particles[index].rho - REST_DENS);
}

void computeForce(WaterParticle* particles, int index)
{
	Vector3f fPress(0.0f, 0.0f, 0.0f);
	Vector3f fVisc(0.0f, 0.0f, 0.0f);
	Vector3f fGrav(0.0f, 0.0f, 0.0f);
	int neighborIdx = 0;
	for (int j = 0; j < particles[index].neighbors; ++j)
	{
		neighborIdx = particles[index].neighborIndexes[j];
		Vector3f distVec = particles[neighborIdx].r - particles[index].r;
		float dist = distVec.norm();
		fPress += distVec.normalized() *
			(particles[index].p + particles[neighborIdx].p) /
			(2.0f * particles[neighborIdx].rho) * SPIKY_GRAD * pow(H - dist, 6.0f);

		//Taken from the slides
		fVisc += VISC * (particles[neighborIdx].v - particles[index].v) /
			particles[neighborIdx].rho * VISC_LAP * (pow(dist, 3.0f) / 2 * pow(H, 3.0f) +
				pow(dist, 2.0f) / pow(H, 2.0f) + H / (2 * dist) - 1);
	}
	fGrav = G;
	fGrav = fGrav * particles[index].rho;
	//Hardcoded to get better results
	fPress *= 800000;
	//Navier stokes equation
	particles[index].f = fPress + fVisc + fGrav;
}

class GetNeighbors {
	WaterParticle *const my_a;

public:

	void operator() (const blocked_range<size_t>& r) const
	{
		WaterParticle *a = my_a;
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			getNeighbors(a, i);
		}
	}

	GetNeighbors(WaterParticle a[]) :
		my_a(a)
	{}
};

class ComputeDensities {
	WaterParticle *const my_a;

public:

	void operator() (const blocked_range<size_t>& r) const
	{
		WaterParticle *a = my_a;
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			computeDensity(a, i);
		}
	}

	ComputeDensities(WaterParticle a[]) :
		my_a(a)
	{}
};

class ComputeForces {
	WaterParticle *const my_a;

public:

	void operator() (const blocked_range<size_t>& r) const
	{
		WaterParticle *a = my_a;
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			computeForce(a, i);
		}
	}

	ComputeForces(WaterParticle a[]) :
		my_a(a)
	{}
};

void getNeighborsTbb(WaterParticle particles[], size_t n)
{
	parallel_for(blocked_range <size_t>(0, n), GetNeighbors(particles));
}

void computeDensitiesTbb(WaterParticle particles[], size_t n)
{
	parallel_for(blocked_range <size_t>(0, n), ComputeDensities(particles));
}

void computeForcesTbb(WaterParticle particles[], size_t n)
{
	parallel_for(blocked_range <size_t>(0, n), ComputeForces(particles));
}

//Updates the positions of the particles using the new forces
void integrate(WaterParticle* particles)
{
	for (int i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		particles[i].v += particles[i].f * TIMESTEP / particles[i].rho;
		particles[i].r += particles[i].v * TIMESTEP;

		if ((particles[i].r.x()) < LC)
		{
			particles[i].v.x() *= BOUND_DAMPING;
			particles[i].r.x() = LC + H / 50;
		}
		if ((particles[i].r.y()) < LC)
		{
			particles[i].v.y() *= BOUND_DAMPING;
			particles[i].r.y() = LC + H / 200;
		}
		if ((particles[i].r.z()) < LC)
		{
			particles[i].v.z() *= BOUND_DAMPING;
			particles[i].r.z() = LC + H / 50;
		}
		if ((particles[i].r.x()) > HC)
		{
			particles[i].v.x() *= BOUND_DAMPING;
			particles[i].r.x() = HC - H / 50;
		}
		if ((particles[i].r.y()) > HC)
		{
			particles[i].v.y() *= BOUND_DAMPING;
			particles[i].r.y() = HC - H / 50;
		}
		if ((particles[i].r.z()) > HC)
		{
			particles[i].v.z() *= BOUND_DAMPING;
			particles[i].r.z() = HC - H / 50;
		}
	}
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void pos(double *px, double *py, double *pz, const int x, const int y,
	const int *viewport)
{
	/*
	Use the ortho projection and viewport information
	to map from mouse co-ordinates back into world
	co-ordinates
	*/

	*px = (double)(x - viewport[0]) / (double)(viewport[2]);
	*py = (double)(y - viewport[1]) / (double)(viewport[3]);

	*px = _left + (*px) * (_right - _left);
	*py = _top + (*py) * (_bottom - _top);
	*pz = _zNear;
}

void getMatrix()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, _matrix);
	invertMatrix(_matrix, _matrixI);
}

double vlen(double x, double y, double z)
{
	return sqrt(x * x + y * y + z * z);
}

void init()
{

    glMatrixMode(GL_MODELVIEW_MATRIX);

      //Light values and coordinates
     GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
     GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
     GLfloat lightPos[] = {20.0f, 20.0f, 50.0f, 0.0f};
     glEnable(GL_DEPTH_TEST);
     glFrontFace(GL_CCW);
     //glEnable(GL_CULL_FACE);
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     // Hidden surface removal // Counterclockwise polygons face out // Do not calculate inside of jet // Enable lighting
     glEnable(GL_LIGHTING);
     // Set up and enable light 0
     glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
     glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
     glEnable(GL_LIGHT0);
     // Enable color tracking
     glEnable(GL_COLOR_MATERIAL);
     // Set material properties to follow glColor values
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

     glClearColor(0.2f, 0.2f, 0.2f, 3.0f );
    
     //Rescale normals to unit length
     glEnable(GL_NORMALIZE);
     glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

     glShadeModel(GL_FLAT);
     getMatrix(); //Init matrix

     //Translate camera
     glPushMatrix();
     glLoadIdentity();
     glTranslatef(0,0,-300.0);
	 glRotatef(40, 0, 1, 0);
     glMultMatrixd(_matrix);
     getMatrix();
     glPopMatrix();

}

void changeSize(int w, int h)
{
    glViewport(0, 0, w, h);

    _top = 1.0;
    _bottom = -1.0;
    _left = -(double) w / (double) h;
    _right = -_left;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* glOrtho(_left,_right,_bottom,_top,_zNear,_zFar);  Ortho */
    gluPerspective(fovy, (double) w / (double) h, _zNear, _zFar);	/* Perspective for stereo */

    glMatrixMode(GL_MODELVIEW);
}

void timerFunction(int value)       
{
    glutTimerFunc(10,timerFunction,1);
    glutPostRedisplay();
}

void resetSimulationVariables()
{
	myTree->reset();
}

void initializeSimVariables()
{
	//The tree to hold our particles
	myTree = new OctTree(Vector3f(LC, LC, LC), Vector3f(HC, HC, HC), NULL);

	//The array which will also hold the particles for displaying
	partArray = new WaterParticle[NUM_OF_PARTICLES];

	//Instantiating particles with random values
	//instantiateOctTree(myTree, partArray);

	int neighbCount = 0;
}

void handleKeyPress(unsigned char key, int x, int y)
{ 
	string fn;
    switch(key)
    {
	case 'r':
		resetSimulationVariables();
		initializeSimVariables();
		break;
	case 'q': case 27: // Pressing enter or 'q' exits the program
		exit(0);
		break;
	default: break;
    }
}

void mouseEvent(int button, int state, int x, int y)
{
    int viewport[4];

    _mouseX = x;
    _mouseY = y;

    if (state == GLUT_UP)
	switch (button) {
    case GLUT_LEFT_BUTTON:
            _mouseLeft =false;
            break;
	case GLUT_MIDDLE_BUTTON:
	    _mouseMiddle = false;
	    break;
	case GLUT_RIGHT_BUTTON:
	    _mouseRight = false;
	    break;
    } else
	switch (button) {
	case GLUT_LEFT_BUTTON:
        _mouseLeft = true;
        break;
	case GLUT_MIDDLE_BUTTON:
	    _mouseMiddle = true;
	    break;
	case GLUT_RIGHT_BUTTON:
	    _mouseRight = true;
	    break;
    case 4:         //Zoomout
        glLoadIdentity();
        glTranslatef(0,0,-2.0);
        glMultMatrixd(_matrix);
        getMatrix();
        glutPostRedisplay();
        break;
    case 3:         //Zoomin
        glLoadIdentity();
        glTranslatef(0,0,2.0);
        glMultMatrixd(_matrix);
        getMatrix();
        glutPostRedisplay();
        break;
    default:
        break;
	}

    glGetIntegerv(GL_VIEWPORT, viewport);
    pos(&_dragPosX, &_dragPosY, &_dragPosZ, x, y, viewport);
}

void mouseMoveEvent(int x, int y)
{
    bool changed = false;

    const int dx = x - _mouseX;
    const int dy = y - _mouseY;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (dx == 0 && dy == 0)
        return;

    if (_mouseMiddle || (_mouseLeft && _mouseRight)) 
	{
		/* double s = exp((double)dy*0.01); */
		/* glScalef(s,s,s); */
		/* if(abs(prev_z) <= 1.0) */

		glLoadIdentity();
		glTranslatef(0, 0, dy * 0.01);
		glMultMatrixd(_matrix);

		changed = true;

    } 
	else if (_mouseLeft) 
	{
		double ax, ay, az;
		double bx, by, bz;
		double angle;

		ax = dy;
		ay = dx;
		az = 0.0;
		angle = vlen(ax, ay, az) / (double) (viewport[2] + 1) * 180.0;

		/* Use inverse matrix to determine local axis of rotation */

		bx = _matrixI[0] * ax + _matrixI[4] * ay + _matrixI[8] * az;
		by = _matrixI[1] * ax + _matrixI[5] * ay + _matrixI[9] * az;
		bz = _matrixI[2] * ax + _matrixI[6] * ay + _matrixI[10] * az;

		glRotatef(angle, bx, by, bz);

		changed = true;

    } else if (_mouseRight) 
	{
		double px, py, pz;

		pos(&px, &py, &pz, x, y, viewport);

		glLoadIdentity();
		glTranslatef(px - _dragPosX, py - _dragPosY, pz - _dragPosZ);
		glMultMatrixd(_matrix);

		_dragPosX = px;
		_dragPosY = py;
		_dragPosZ = pz;

		changed = true;
    }

    _mouseX = x;
    _mouseY = y;

    if (changed) {
        getMatrix();
        glutPostRedisplay();
    }
}

void instantiateOctTree(OctTree* simTree, WaterParticle* particles)
{
	//Check if array has been instantiated
	if (particles[0].m != PARTICLE_MASS)
	{
		srand(time(NULL));
		for (int i = 0; i < NUM_OF_PARTICLES; i++)
		{
			particles[i].setPosition(Vector3f(RandomFloat(6 * LC / 8, 6 * HC / 8),
				RandomFloat(6 * LC / 8, 6 * HC / 8),
				RandomFloat(6 * LC / 8, 6 * HC / 8)));
			particles[i].m = PARTICLE_MASS;
			particles[i].v = Vector3f(0.0f, 0.0f, 0.0f);
			particles[i].p = 1000.0f;
			particles[i].neighbors = 0;
			particles[i].arrayIndex = i;

			simTree->insert(&particles[i]);
		}
	}
	else
	{
		for (int i = 0; i < NUM_OF_PARTICLES; i++)
		{
			simTree->insert(&particles[i]);
		}
	}
}

void calculateSimulationVariables()
{
	instantiateOctTree(myTree, partArray);

	getNeighborsTbb(partArray, NUM_OF_PARTICLES);

	// Get forces from neighbors
	computeDensitiesTbb(partArray, NUM_OF_PARTICLES);

	computeForcesTbb(partArray, NUM_OF_PARTICLES);

	// Update positions from forces
	integrate(partArray);
}

void display()
{
	calculateSimulationVariables();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glMultMatrixd(_matrix);
	
	for (int i = 0; i < NUM_OF_PARTICLES; i++)
	{
		partArray[i].draw();
	}

    glutSwapBuffers();

	resetSimulationVariables();
}

int main(int argc, char **argv)
{
	//The tree to hold our particles
	initializeSimVariables();

    glutInit(&argc, argv);
    //Print contex info
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	//double buffer
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("COMP477 - Fluid Simulation Project");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutTimerFunc(10, timerFunction, 1);

    glutMouseFunc(mouseEvent);
    glutMotionFunc(mouseMoveEvent);
    glutKeyboardFunc(handleKeyPress);
    
    init();
    glutMainLoop();
    // Delete the octTree
	myTree->reset();
    return 0;
}

