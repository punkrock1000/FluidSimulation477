#ifndef SIMPLEMATH_H
#define SIMPLEMATH_H

#include <math.h>
#include <cstring>
#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;

#define M_PI 3.14159265358979323846

double convertAngleToRadianFromDegrees(double angleInDegrees);

double convertAngleToDegreesFromRadian(double angleInRadian);

/*
//Vectors
struct Vec3
{
    double x, y, z;
    Vec3() {x=0; y=0; z=0;}
    Vec3(double a, double b, double c):x(a),y(b),z(c){}

	void normalize();
	double getNorm();

	friend Vec3 operator* (const float&, const Vec3&);
	friend Vec3 operator+ (const Vec3&, const Vec3&);
};*/

float distance(Vector3f a, Vector3f b);

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

struct Vec4
{
    double x, y, z, w;
    Vec4() {x=0; y=0; z=0; w=0;}
    Vec4(double a, double b, double c, double d):x(a),y(b),z(c),w(d){}
};

struct Vec2
{
    double x, y;
    Vec2() {x=0; y=0;}
    Vec2(double a, double b):x(a),y(b){}
};

struct Mat3
{
    double values[9];
    Mat3()
    {
        for (unsigned i=0; i<9; i++)
            values[i]=0;
    }
};

struct Mat4
{
    double values[16];
    Mat4()
    {
        for (unsigned i=0; i<16; i++)
            values[i]=0;
    }
};


//Vector operations
double dot2(Vec2 a, Vec2 b);

double dot3(Vec3 a, Vec3 b);

double dot4(Vec4 a, Vec4 b);

Vec3 cross3(Vec3 a, Vec3 b);

void trans(float* m, float* v, float* r);

void axisToMat(float* a, float ang, float* m);

void mult(float* m1, float* m2, float* r);
void mult(double* m1, float* m2, double* r);
void multv(float *m, float *v, float *r, float w=1.0);

void scalar(float s,float* m, float *r);

void add(float *s, float*m, float *r);

#endif
