#include "OctTree.h"

OctTree::OctTree()
{
	bottomLeftBack = Vec3(0, 0, 0);
	topRightFront = Vec3(0, 0, 0);
	particle = NULL;
	initializeSubnodes();
}

OctTree::OctTree(Vec3 _bottomLeftBack, Vec3 _topRightFront, OctTree* parent)
	:particle(), bottomLeftBack(_bottomLeftBack), topRightFront(_topRightFront)
{
	center.x = (_topRightFront.x + _bottomLeftBack.x) / 2; 
	center.y = (_topRightFront.y + _bottomLeftBack.y) / 2;
	center.z = (_topRightFront.z + _bottomLeftBack.z) / 2;
	initializeSubnodes();
}

void OctTree::initializeSubnodes()
{
	for (int i = 0; i < 8; ++i)
	{
		subnodes[i] = NULL;
	}
}

void OctTree::checkOctant(Particle* particle)
{
	int index = 0;
	Vec3 newBLB, newTRF;
	//Check if in left half
	if (center.x >= particle->position.x)
	{
		//Check if in bottom half
		if (center.y >= particle->position.y)
		{
			//Check if in back half
			if (center.z >= particle->position.z)
			{
				index = 0;
				newBLB = bottomLeftBack;
				newTRF = center;
			}
			else
			{
				index = 1;
				newBLB = Vec3(bottomLeftBack.x, bottomLeftBack.y, center.z);
				newTRF = Vec3(center.x, center.y, topRightFront.z);
			}
		}
		else
		{
			if (center.z >= particle->position.z)
			{
				index = 2;
				newBLB = Vec3(bottomLeftBack.x, center.y, bottomLeftBack.z);
				newTRF = Vec3(center.x, topRightFront.y, center.z);
			}
			else
			{
				index = 3;
				newBLB = Vec3(bottomLeftBack.x, center.y, center.z);
				newTRF = Vec3(center.x, topRightFront.y, topRightFront.z);
			}
		}
	}
	//In right half
	else
	{
		//Check if in bottom half
		if (center.y >= particle->position.z)
		{
			//Check if in back half
			if (center.z >= particle->position.z)
			{
				index = 4;
				newBLB = Vec3(center.x, bottomLeftBack.y, bottomLeftBack.z);
				newTRF = Vec3(topRightFront.x, center.y, center.z);
			}
			else
			{
				index = 5;
				newBLB = Vec3(center.x, bottomLeftBack.y, center.z);
				newTRF = Vec3(topRightFront.x, center.y, topRightFront.z);
			}
		}
		else
		{
			if (center.z >= particle->position.z)
			{
				index = 6;
				newBLB = Vec3(center.x, center.y, bottomLeftBack.z);
				newTRF = Vec3(topRightFront.x, topRightFront.y, center.z);
			}
			else
			{
				index = 7;
				newBLB = center;
				newTRF = topRightFront;
			}
		}
	}

	if (subnodes[index] == NULL)
	{
		subnodes[index] = new OctTree(newBLB, newTRF, this);
	}
	subnodes[index]->insert(particle);
}

void OctTree::insert(Particle* particle)
{
	if (particle == NULL)
		return;

	if(!inBoundary(particle->position))
		return;

	if (this->particle == NULL && !isSubdivided)
	{
		this->particle = particle;
		this->center = Vec3((topRightFront.x + bottomLeftBack.x) / 2, 
			(topRightFront.y + bottomLeftBack.y) / 2, 
			(topRightFront.z + bottomLeftBack.z) / 2);
		return;
	}

	if (this->particle != NULL)
	{
		if (particle->position.x == this->particle->position.x &&
			particle->position.y == this->particle->position.y &&
			particle->position.z == this->particle->position.z)
			return;
		checkOctant(this->particle);
		isSubdivided = true;
		this->particle = NULL;
	}
	checkOctant(particle);
	return;
}

void OctTree::reset()
{
	if (!isRoot())
		throw std::runtime_error("Only the root node may reset the tree.");

	for (int i = 0; i < 8; ++i)
	{
		if (subnodes[i] != NULL)
		{
			delete subnodes[i];
			subnodes[i] = NULL;
		}
	}

	if (this->particle != NULL)
		this->particle = NULL;

	isSubdivided = false;
}

bool OctTree::withinSphere(Vec3 center, float radius)
{
	float dist = this->particle->position.distance(center);
	if (dist <= radius * radius)
		return true;
	return false;
}

void OctTree::getNeighbors(Particle* particle, Particle* partArray, int numNeighbors, float radius, int &curNeighborIdx)
{
	if (curNeighborIdx < numNeighbors)
	{
		//Check if at a leaf node
		if (this->particle != NULL)
		{
			if (this->particle->position.distance(particle->position) <= radius)
			{
				if (this->particle != particle)
				{
					partArray[curNeighborIdx] = *this->particle;
					curNeighborIdx++;
				}
			}
			return;
		}
		//Check if sphere of the radius intersects with a subnode
		for (int i = 0; i < 8; ++i)
		{
			if (subnodes[i] != NULL)
			{
				if (this->cubeIntersectsSphere(subnodes[i]->bottomLeftBack, subnodes[i]->topRightFront,
					particle->position, radius))
				{
					subnodes[i]->getNeighbors(particle, partArray, numNeighbors, radius, curNeighborIdx);
				}
			}
		}
	}
}

/*
Taken from:: https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test
*/
bool OctTree::cubeIntersectsSphere(Vec3 C1, Vec3 C2, Vec3 S, float R)
{
	float dist_squared = R * R;
	/* assume C1 and C2 are element-wise sorted, if not, do that now */
	if (S.x < C1.x) dist_squared -= (S.x - C1.x) * (S.x - C1.x);
	else if (S.x > C2.x) dist_squared -= (S.x - C2.y) * (S.x - C2.y);
	if (S.y < C1.y) dist_squared -= (S.y - C1.y) * (S.y - C1.y);
	else if (S.y > C2.y) dist_squared -= (S.y - C2.y) * (S.y - C2.y);
	if (S.z < C1.z) dist_squared -= (S.z - C1.z) * (S.z - C1.z);
	else if (S.z > C2.z) dist_squared -= (S.z - C2.z) * (S.z - C2.z);
	return dist_squared > 0;
}

bool OctTree::inBoundary(Vec3 _pos)
{
	return(_pos.x >= bottomLeftBack.x && _pos.x <= topRightFront.x &&
		_pos.y >= bottomLeftBack.y && _pos.y <= topRightFront.y &&
		_pos.z >= bottomLeftBack.z && _pos.z <= topRightFront.z);
}

bool OctTree::isRoot()
{
	return parent == NULL;
}

OctTree::~OctTree()
{
	for (int i = 0; i < 8; ++i)
	{
		delete subnodes[i];
	}
}