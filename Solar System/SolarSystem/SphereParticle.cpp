#include "SphereParticle.h"

SphereParticle::SphereParticle(float radius, double mass, Color color)
{
	init();
	mRadius = radius;
	mMass = mass;
	mColor = color;

	slices = 40;
	stacks = 40;
	//slices = mRadius * 20;
	//stacks = mRadius * 20;
}

void SphereParticle::draw()
{
	GLUquadricObj* sphere;
	sphere = gluNewQuadric();

	glLoadIdentity();
	glPushMatrix();

	glTranslatef(mPosition.x/mPosScale, mPosition.y/mPosScale, mPosition.z/mPosScale);
	glColor4f(mColor.r, mColor.g, mColor.b, mColor.a);
	gluSphere(sphere, mRadius*mSizeScale, slices, stacks);
	
	glPopMatrix();
	gluDeleteQuadric(sphere);
}