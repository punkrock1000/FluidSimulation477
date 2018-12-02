#include "OctTree.h"

OctTree::OctTree()
{
	bottomLeftBack = Vector3f(0, 0, 0);
	topRightFront = Vector3f(0, 0, 0);
	particle = NULL;
	initializeSubnodes();
}

OctTree::OctTree(Vector3f _bottomLeftBack, Vector3f _topRightFront, OctTree* parent)
	:particle(), bottomLeftBack(_bottomLeftBack), topRightFront(_topRightFront)
{
	center.x() = (_topRightFront.x() + _bottomLeftBack.x()) / 2; 
	center.y() = (_topRightFront.y() + _bottomLeftBack.y()) / 2;
	center.z() = (_topRightFront.z() + _bottomLeftBack.z()) / 2;
	this->parent = parent;
	initializeSubnodes();
}

void OctTree::initializeSubnodes()
{
	for (int i = 0; i < 8; ++i)
	{
		subnodes[i] = NULL;
	}
}

void OctTree::checkOctant(WaterParticle* particle)
{
	int index = 0;
	Vector3f newBLB, newTRF;
	//Check if in left half
	if (center.x() >= particle->r.x())
	{
		//Check if in bottom half
		if (center.y() >= particle->r.y())
		{
			//Check if in back half
			if (center.z() >= particle->r.z())
			{
				index = 0;
				newBLB = bottomLeftBack;
				newTRF = center;
			}
			else
			{
				index = 1;
				newBLB = Vector3f(bottomLeftBack.x(), bottomLeftBack.y(), center.z());
				newTRF = Vector3f(center.x(), center.y(), topRightFront.z());
			}
		}
		else
		{
			if (center.z() >= particle->r.z())
			{
				index = 2;
				newBLB = Vector3f(bottomLeftBack.x(), center.y(), bottomLeftBack.z());
				newTRF = Vector3f(center.x(), topRightFront.y(), center.z());
			}
			else
			{
				index = 3;
				newBLB = Vector3f(bottomLeftBack.x(), center.y(), center.z());
				newTRF = Vector3f(center.x(), topRightFront.y(), topRightFront.z());
			}
		}
	}
	//In right half
	else
	{
		//Check if in bottom half
		if (center.y() >= particle->r.y())
		{
			//Check if in back half
			if (center.z() >= particle->r.z())
			{
				index = 4;
				newBLB = Vector3f(center.x(), bottomLeftBack.y(), bottomLeftBack.z());
				newTRF = Vector3f(topRightFront.x(), center.y(), center.z());
			}
			else
			{
				index = 5;
				newBLB = Vector3f(center.x(), bottomLeftBack.y(), center.z());
				newTRF = Vector3f(topRightFront.x(), center.y(), topRightFront.z());
			}
		}
		else
		{
			if (center.z() >= particle->r.z())
			{
				index = 6;
				newBLB = Vector3f(center.x(), center.y(), bottomLeftBack.z());
				newTRF = Vector3f(topRightFront.x(), topRightFront.y(), center.z());
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

void OctTree::insert(WaterParticle* particle)
{
	if (particle == NULL)
		return;

	if(!inBoundary(particle->r))
		return;

	if (this->particle == NULL && !isSubdivided)
	{
		this->particle = particle;
		this->center = Vector3f((topRightFront.x() + bottomLeftBack.x()) / 2, 
			(topRightFront.y() + bottomLeftBack.y()) / 2,
			(topRightFront.z() + bottomLeftBack.z()) / 2);
		return;
	}

	if (this->particle != NULL)
	{
		if (particle->r.x() == this->particle->r.x() &&
			particle->r.y() == this->particle->r.y() &&
			particle->r.z() == this->particle->r.z())
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

bool OctTree::withinSphere(Vector3f center, float radius)
{
	float dist = distance(this->particle->r, center);
	if (dist <= radius * radius)
		return true;
	return false;
}

void OctTree::getNeighbors(WaterParticle* particle, WaterParticle* partArray, int numNeighbors, float radius, int &curNeighborIdx)
{
	if (curNeighborIdx < numNeighbors)
	{
		//Check if at a leaf node
		if (this->particle != NULL)
		{
			if (distance(this->particle->r, particle->r) <= radius)
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
					particle->r, radius))
					subnodes[i]->getNeighbors(particle, partArray, numNeighbors, radius, curNeighborIdx);
			}
		}
	}
}

void OctTree::countParticles(int &particleCount)
{
	if (this->particle != NULL)
	{
		particleCount++;
	}
	for (int i = 0; i < 8; ++i)
	{
		if (subnodes[i] != NULL)
		{
			subnodes[i]->countParticles(particleCount);
		}
	}
}

/*
Taken from:: https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test
*/
bool OctTree::cubeIntersectsSphere(Vector3f C1, Vector3f C2, Vector3f S, float R)
{
	float dist_squared = R * R;
	/* assume C1 and C2 are element-wise sorted, if not, do that now */
	if (S.x() < C1.x()) dist_squared -= (S.x() - C1.x()) * (S.x() - C1.x());
	else if (S.x() > C2.x()) dist_squared -= (S.x() - C2.y()) * (S.x() - C2.y());
	if (S.y() < C1.y()) dist_squared -= (S.y() - C1.y()) * (S.y() - C1.y());
	else if (S.y() > C2.y()) dist_squared -= (S.y() - C2.y()) * (S.y() - C2.y());
	if (S.z() < C1.z()) dist_squared -= (S.z() - C1.z()) * (S.z() - C1.z());
	else if (S.z() > C2.z()) dist_squared -= (S.z() - C2.z()) * (S.z() - C2.z());
	return dist_squared > 0;
}

bool OctTree::inBoundary(Vector3f _pos)
{
	return(_pos.x() >= bottomLeftBack.x() && _pos.x() <= topRightFront.x() &&
		_pos.y() >= bottomLeftBack.y() && _pos.y() <= topRightFront.y() &&
		_pos.z() >= bottomLeftBack.z() && _pos.z() <= topRightFront.z());
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