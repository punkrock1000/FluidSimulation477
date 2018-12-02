#include "WaterParticle.h"

WaterParticle::WaterParticle()
{
	m = 0.05;//particle mass
	r << -1, -1, -1;
	p = 0.005;//particle density
	//a = new WaterParticle[8];//neighbours 0-7
	global_Index = -1;
}

void WaterParticle::setPosition(Vector3f v) {
	r(0) = v(0);
	r(1) = v(1);
	r(2) = v(2);
}

void WaterParticle::draw()
{
	glPushMatrix();
	glColor3f(0, 0.5, 1);
	glTranslatef(r(0), r(1), r(2));
	glutSolidSphere(rad, nSlices, nStacks);
	glPopMatrix();
}
