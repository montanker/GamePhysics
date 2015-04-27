#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H

#include "RigidBody.h"
#include "Vector.h"

class ForceGenerator
{
public:
	virtual void updateForce(RigidBody *body, double duration);
};

class Gravity : public ForceGenerator
{
	Vector3 gravity;
public:
	Gravity(const Vector3 &gravity);
	virtual void updateForce(RigidBody *body, double duration);
};

class Spring : public ForceGenerator
{
	Vector3 connectionPoint;
	Vector3 otherConnectionPoint;
	RigidBody *other;
	double springConstant;
	double restLength;

public:
	Spring(const Vector3 &localConnectionPt,
		RigidBody *other,
		const Vector3 &otherConnectionPt,
		double springConstant,
		double length);

	virtual void updateForce(RigidBody *body, double duration);
};


#endif