#include "WaterParticle.h"



WaterParticle::WaterParticle()
{
	j = 0.05;//particle mass
	r << -1, -1, -1;
	p = 0.005;//particle density
	//a = new WaterParticle[8];//neighbours 0-7
	global_Index = -1;
}

void WaterParticle::setThisParticlePosition(Vector3f v) {
		r(0) = v(0);
		r(1) = v(1);
		r(2) = v(2);
}

void WaterParticle::drawThisParticle()
{
	glPushMatrix();
	glTranslatef(r(0), r(1), r(2));
	glutSolidSphere(rad, nSlices, nStacks);
	glPopMatrix();
}

WaterParticle::~WaterParticle()
{
}
