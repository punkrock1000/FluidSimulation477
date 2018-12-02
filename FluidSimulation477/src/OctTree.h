#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "simpleMath.h"
#include "WaterParticle.h"
#include "Eigen/Dense"

class OctTree
{
	Vector3f center, bottomLeftBack, topRightFront;

	OctTree* parent;
	WaterParticle* particle;

	/*Eight nodes are as follows
		0: Bottom left back  1: Bottom left front
		2: Top left back  3: Top left front
		4: Bottom right back  5: Bottom right front
		6: Top right back  7: Top right front
	
	*/
	OctTree* subnodes[8];

	bool isSubdivided = false;

	void initializeSubnodes();
private:
	void checkOctant(WaterParticle*);

public:
	OctTree();
	OctTree(Vector3f, Vector3f, OctTree* parent = nullptr);
	~OctTree();

	void reset();
	//TODO: Add WaterParticle class
	void insert(WaterParticle*);
	bool inBoundary(Vector3f);
	bool isRoot();
	bool withinSphere(Vector3f center, float radius);
	//bool withinSphere(Vector3f, float);
	static bool cubeIntersectsSphere(Vector3f, Vector3f, Vector3f, float);
	void getNeighbors(WaterParticle* particle, WaterParticle* partArray, int, float, int&);
	void countParticles(int&);
};
