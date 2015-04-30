#include "CollisionPrimitive.h"
#include "CollisionDetection.h"
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLFW/glfw3.h>
#include "Vector3.h"
#include <assert.h>
#include "Color.h"

void CollisionPrimitive::calculateInternals()
{
	transform = body->getTransform() * offset;
}

Vector3 CollisionPrimitive::getAxis(unsigned index) const
{
	return transform.getAxisVector(index);
}

void CollisionPrimitive::setAxis(unsigned index, Vector3 vals)
{
	transform.setAxisVector(index,vals.x,vals.y,vals.z);
}

void CollisionPrimitive::integrate(double duration)
{
	body->integrate(duration);
}

void CollisionPrimitive::draw() 
{
	glLoadIdentity();
	glPushMatrix();

	Vector3 pos = body->getPosition();
	pos += offset.getAxisVector(3);

	glTranslatef((GLfloat)pos.x, (GLfloat)pos.y, (GLfloat)pos.z);
	glColor4f(color.r, color.g, color.b, color.a);

	drawShape(transform);
	
	glPopMatrix();
}

void CollisionPrimitive::drawShape(Matrix4 &transform) 
{
	GLUquadricObj* sphere;
	sphere = gluNewQuadric();

	double radius = 1.0;
	gluSphere(sphere, radius, 20,20);

	gluDeleteQuadric(sphere);
}

void CollisionSphere::drawShape(Matrix4 &transform) 
{
	GLUquadricObj* sphere;
	sphere = gluNewQuadric();

	gluSphere(sphere, radius, 20,20);

	gluDeleteQuadric(sphere);
}

bool CollisionSphere::DetectCollision(CollisionPrimitive *other,CollisionData *data) 
{
	switch(other->shape) {
	case Sphere:
		return(CollisionDetector::sphereAndSphere(*this,*(static_cast<CollisionSphere *>(other)),data));
	case Box:
		return(other->DetectCollision(this,data));
	}
	return(false);
}

bool CollisionBox::DetectCollision(CollisionPrimitive *other,CollisionData *data) 
{
	switch(other->shape) {
	case Box:
		return(CollisionDetector::boxAndBox(*this,*(static_cast<CollisionBox *>(other)),data));
	case Sphere:
		return(CollisionDetector::boxAndSphere(*this,*(static_cast<CollisionSphere *>(other)),data));
	}
	return(false);
}

void CollisionBox::drawShape(Matrix4 &transform) 
{
	GLfloat w = halfSize.x*2.0;
	GLfloat h = halfSize.y*2.0;
	GLfloat z = halfSize.z*2.0;

	glBegin(GL_QUADS);
		//Top face (y = 1.0f) 
		glVertex3f(+w, h, -z);
		glVertex3f(-w, h, -z);
		glVertex3f(-w, h, z);
		glVertex3f(+w, h, z);
		//Bottom face (y = -1.0f)
		glVertex3f(+w, -h, z);
		glVertex3f(-w, -h, z);
		glVertex3f(-w, -h, -z);
		glVertex3f(+w, -h, -z);
		// Front face  (z = 1.0f)
		glVertex3f(+w, h, z);
		glVertex3f(-w, h, z);
		glVertex3f(-w, -h, z);
		glVertex3f(+w, -h, z);
		// Back face (z = -1.0f)
		glVertex3f(+w, -h, -z);
		glVertex3f(-w, -h, -z);
		glVertex3f(-w, h, -z);
		glVertex3f(+w, h, -z);
		// Left face (x = -1.0f)
		glVertex3f(-w, h, z);
		glVertex3f(-w, h, -z);
		glVertex3f(-w, -h, -z);
		glVertex3f(-w, -h, z);
		// Right face (x = 1.0f)
		glVertex3f(+w, h, -z);
		glVertex3f(+w, h, z);
		glVertex3f(+w, -h, z);
		glVertex3f(+w, -h, -z);
	glEnd();
}

void CollisionPlane::drawShape(Matrix4 &transform) 
{
	GLfloat size = 100;
	GLfloat height = 0.1f;

	glBegin(GL_QUADS);
		//Top face (y = 1.0f) 
		glVertex3f(size, height, -size);
		glVertex3f(-size, height, -size);
		glVertex3f(-size, height, size);
		glVertex3f(+size, height, size);
	glEnd();
}

bool CollisionPlane::DetectCollision(CollisionPrimitive *other,CollisionData *data) 
{
	switch(other->shape) 
	{
	case Box:
		return(CollisionDetector::boxAndHalfSpace(*(static_cast<CollisionBox *>(other)),*this,data));
	case Sphere:
		return(CollisionDetector::sphereAndHalfSpace(*(static_cast<CollisionSphere *>(other)),*this,data));
	}
	return(false);
}