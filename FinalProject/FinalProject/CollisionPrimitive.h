#ifndef __COLLISIONPRIMITIVE_H
#define __COLLISIONPRIMITIVE_H

#include "RigidBody.h"
#include "Matrix4.h"

class CollisionPrimitive
{
public:
	CollisionPrimitive();
	void calculateInternals();
	Vector3 getAxis(unsigned index) const;
	void setAxis(unsigned index, Vector3 vals);
	const Matrix4& getTransform() const {return transform;}

	RigidBody* body;
	Matrix4 offset;

	friend class IntersectionTests;
    friend class CollisionDetector;

protected:
	Matrix4 transform;
};

class CollisionSphere: public CollisionPrimitive
{
public:
	double radius;
};

class CollisionBox: public CollisionPrimitive
{
public:
	Vector3 halfSize;
};

class CollisionPlane
{
public:
	Vector3 direction;
	double offset;
};

#endif