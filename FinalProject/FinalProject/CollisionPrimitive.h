#ifndef __COLLISIONPRIMITIVE_H
#define __COLLISIONPRIMITIVE_H

#include "RigidBody.h"
#include "Matrix4.h"
#include "Color.h"
//#include "CollisionDetection.h"

class CollisionPrimitive
{
public:
	enum Shapes { None, Sphere, Box, Plane };

	void calculateInternals();
	Vector3 getAxis(unsigned index) const;
	void setAxis(unsigned index, Vector3 vals);
	void setPosition(Vector3 pos) {body->setPosition(pos);}
	void setVelocity(Vector3 vel) {body->setVelocity(vel);}
	void integrate(double duration);
	const Matrix4& getTransform() const {return transform;}

	RigidBody* body;
	Matrix4 offset;

	Shapes shape;
	Color color;
	void draw();
	virtual void drawShape(Matrix4 &transform)=0;
	virtual bool DetectCollision(CollisionPrimitive *other,struct CollisionData *data) = 0;

	friend class IntersectionTests;
    friend class CollisionDetector;

protected:

	CollisionPrimitive(Shapes s=None,Color c=Color(1.0,0.0,0.0)) : color(c), shape(s) { }
	Matrix4 transform;
};

class CollisionSphere: public CollisionPrimitive
{
public:
	CollisionSphere(double rad=1.0,Color c = Color(1,0,0)) : CollisionPrimitive(Sphere,c), radius(rad) { }
	double radius;
	void drawShape(Matrix4 &transform);
	bool DetectCollision(CollisionPrimitive *other,struct CollisionData *data);
};

class CollisionBox: public CollisionPrimitive
{
public:
	CollisionBox(Vector3 hsize,Color c = Color(1,0,0)) : CollisionPrimitive(Box,c), halfSize(hsize) { }
	Vector3 halfSize;
	void drawShape(Matrix4 &transform);
	bool DetectCollision(CollisionPrimitive *other,struct CollisionData *data);
};

class CollisionPlane : public CollisionPrimitive
{
public:
	CollisionPlane(Vector3 dir,Color c = Color(1,0,0)) : CollisionPrimitive(Plane,c), direction(dir) {  }
	Vector3 direction;
	double offset;
	void drawShape(Matrix4 &transform);
	bool DetectCollision(CollisionPrimitive *other,struct CollisionData *data);
};

#endif