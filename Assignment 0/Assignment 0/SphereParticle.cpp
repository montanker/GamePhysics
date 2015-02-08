#include "SphereParticle.h"

SphereParticle::SphereParticle(float radius, float mass, Color color)
{
	init();
	mRadius = radius;
	mMass = mass;
	mColor = color;

	slices = 20;
	stacks = 20;
}

void SphereParticle::draw()
{
	GLUquadricObj* sphere;
	sphere = gluNewQuadric();

	glLoadIdentity();
	glPushMatrix();

	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glColor4f(mColor.r, mColor.g, mColor.b, mColor.a);
	gluSphere(sphere, mRadius, slices, stacks);
	
	glPopMatrix();
	gluDeleteQuadric(sphere);
}