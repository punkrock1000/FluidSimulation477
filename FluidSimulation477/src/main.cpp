#include "OctTree.h"
#include <iostream>
#define NUM_OF_PARTICLES 10000
#define NUM_NEIGHBORS 16
#define RADIUS 8
//Low corner of the initial tree
#define LC -100
//High corner of the tree
#define HC 100

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void main()
{
	//The tree to hold our particles
	OctTree* myTree = new OctTree(Vec3(LC, LC, LC), Vec3(HC, HC, HC), NULL);

	//The array which will also hold the particles for displaying
	Particle* partArray = new Particle[NUM_OF_PARTICLES];

	//The arrays of the neighbors for each particle
	Particle* neighborArrays[NUM_OF_PARTICLES];

	//Instantiating particles with random values
	for (int i = 0; i < NUM_OF_PARTICLES; i++)
	{
		partArray[i].position.x = RandomFloat(LC/4, HC/4);
		partArray[i].position.y = RandomFloat(LC / 4, HC / 4);
		partArray[i].position.z = RandomFloat(LC / 4, HC / 4);
		partArray[i].mass = 3.0f;

		myTree->insert(&partArray[i]);
	}
	int neighbCount = 0;

	///Main loop

	//Find the neighbors for all particles
	for (int i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		neighborArrays[i] = new Particle[NUM_NEIGHBORS];
		myTree->getNeighbors(&partArray[i], neighborArrays[i], NUM_NEIGHBORS, RADIUS, neighbCount);
		neighbCount = 0;
	}
	///TODO:
	// Get forces from neighbors

	// Update positions from forces

	// Display

	// Delete any objects instantiated with new

	///End of main loop

	myTree->reset();
}
