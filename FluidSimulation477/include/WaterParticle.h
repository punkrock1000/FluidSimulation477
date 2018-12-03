#pragma once
#include <Eigen/Dense>
#include <GL/glut.h>
using namespace Eigen;

class WaterParticle
{
public:
	float j;//particle mass
	Vector3f r; //particle position
	float p;//particle density
	//WaterParticle* a ;//neighbours 0-7

	int global_Index;
	
	//particle dimension
	float rad = 1;
	float nSlices = 10;
	float nStacks = 10;

	void setThisParticlePosition(Vector3f v);

	void drawThisParticle();


	//face 1: 0,1,2,3
	//face 2: 0,3,4,5
	//face 3: 4,5,6,7
	//face 4: 1,2,7,6
	//face 5: 2,3,4,7
	//face 6: 0,1,6,5


	WaterParticle();
	

	~WaterParticle();
};

