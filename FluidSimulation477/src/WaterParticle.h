#pragma once
#include "Eigen/Dense"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include "GL/glut.h"
#else
#include <GL/freeglut.h>
#endif
#endif
using namespace Eigen;

class WaterParticle
{
public:
	float m;//particle mass
	Vector3f r, v, f; //particle position, velocity, force
	float rho, p;//particle density, pressure
	int neighbors, arrayIndex;
	int neighborIndexes[16];

	int global_Index;
	
	//particle dimension
	float rad = 1;
	float nSlices = 10;
	float nStacks = 10;

	void setPosition(Vector3f v);

	void draw();

	//face 1: 0,1,2,3
	//face 2: 0,3,4,5
	//face 3: 4,5,6,7
	//face 4: 1,2,7,6
	//face 5: 2,3,4,7
	//face 6: 0,1,6,5

	WaterParticle();
};

