#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

struct Vec3
{
	float x;
	float y;
	float z;
	
	Vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vec3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vec3 operator+(Vec3 _p)
	{
		x += _p.x;
		y += _p.y;
		z += _p.z;
		return *this;
	}
	Vec3 operator-(Vec3 _p)
	{
		x -= _p.x;
		y -= _p.y;
		z -= _p.z;
		return *this;
	}
	Vec3 operator*(float _f)
	{
		x *= _f;
		y *= _f;
		z *= _f;
		return *this;
	}
	Vec3 operator/(float _f)
	{
		x /= _f;
		y /= _f;
		z /= _f;
	}
	float distance(Vec3 other)
	{
		float dx = other.x - x;
		float dy = other.y - y;
		float dz = other.z - z;
		return sqrt((dx*dx) + (dy*dy) + (dz*dz));
	}
	friend std::ostream& operator<<(std::ostream &os, const Vec3 &p)
	{
		return os << "x: " << p.x << " y:" << p.y << " z:" << p.z;
	}
};

//Replace with Patrick's particle class
struct Particle
{
	Vec3 position;
	float mass;

	Particle()
	{
		this->position = Vec3(0, 0, 0);
		this->mass = 0;
	}
	Particle(Vec3 _position, float _mass)
	{
		this->position = _position;
		this->mass = _mass;
	}
};

class OctTree
{
	Vec3 center, bottomLeftBack, topRightFront;

	OctTree* parent;
	Particle* particle;

	/*Eight nodes are as follows
		0: Bottom left back  1: Bottom left front
		2: Top left back  3: Top left front
		4: Bottom right back  5: Bottom right front
		6: Top right back  7: Top right front
	
	*/
	OctTree* subnodes[8];

	bool isSubdivided = false;

	void initializeSubnodes();

public:
	OctTree();
	OctTree(Vec3, Vec3, OctTree* parent = nullptr);
	~OctTree();

	void reset();
	//TODO: Add particle class
	void insert(Particle*);
	bool inBoundary(Vec3);
	bool isRoot();
	void checkOctant(Particle*);
};
