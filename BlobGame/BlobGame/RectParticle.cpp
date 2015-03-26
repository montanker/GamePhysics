#include "RectParticle.h"

RectParticle::RectParticle(float width, float length, float height, double mass, Color color)
{
	init();
	mWidth = width;
	mLength = length;
	mHeight = height;
	mMass = mass;
	mColor = color;
}

void RectParticle::draw()
{
	double w = mWidth/2;
	double l = mLength/2;
	double h = mHeight/2;

	glLoadIdentity();
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(mColor.r, mColor.g, mColor.b);
		//Top face (y = 1.0f) 
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z+l));
		//Bottom face (y = -1.0f)
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z-l));
		// Front face  (z = 1.0f)
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z+l));
		// Back face (z = -1.0f)
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z-l));
		// Left face (x = -1.0f)
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x-w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z+l));
		// Right face (x = 1.0f)
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z-l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y+h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z+l));
		glVertex3f((GLfloat)(mPosition.x+w), (GLfloat)(mPosition.y-h), (GLfloat)(mPosition.z-l));
	glEnd();
	glPopMatrix();
	/*GLUquadricObj* sphere;
	sphere = gluNewQuadric();
	 
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(mPosition.x/mPosScale, mPosition.y/mPosScale, mPosition.z/mPosScale);
	glColor4f(mColor.r, mColor.g, mColor.b, mColor.a);
	gluSphere(sphere, mRadius*mSizeScale, slices, stacks);
	
	glPopMatrix();
	gluDeleteQuadric(sphere);*/
}