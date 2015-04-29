#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <assert.h>
#include <float.h>

#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Matrix3.h"

class RigidBody
{
protected:
	double inverseMass;
	
	double linearDampening;
	double angularDampening;

	bool isAwake;

	Vector3 position;
	Quaternion orientation;
	Vector3 velocity;
	Vector3 rotation; //holds angular velocity

	Matrix4 transformMatrix;
	Matrix3 inverseInertiaTensor;
	Matrix3 inverseInertiaTensorWorld;

	Vector3 forceAccum;
	Vector3 torqueAccum;

	Vector3 acceleration;
	Vector3 lastFrameAcceleration;

public:
	RigidBody() { init(); }
	void init();

	void calculateDerivedData();
	void setInertiaTensor(const Matrix3 &inertiaTensor);
	void setInertiaTensorWorld(Matrix3 &inertiaTensor);
	Vector3 getPointInWorldSpace(Vector3 point);
	Matrix4 getTransform() { return transformMatrix; }

	void addForce(const Vector3 &force);
	void addForceAtPoint(const Vector3 &force, const Vector3 &point);
	void addForceAtBodyPoint(const Vector3 &force, const Vector3 &point);
	void addVelocity(const Vector3 &deltaVelocity);
	void setVelocity(const Vector3 &velocity);
	void addRotation(const Vector3 &deltaRotation);

	void clearAccumulators();
	void integrate(double duration);

	bool getAwake() {return isAwake;}
	void setAwake(bool awake=true) {isAwake = awake;}
	Vector3 getPosition() {return position;}
	void setPosition(Vector3 pos) {position = pos;}
	Vector3 getVelocity() {return velocity;}
	Vector3 getRotation() {return rotation;}
	Quaternion getOrientation() {return orientation;}
	void setOrientation(Quaternion rot) {orientation = rot;}
	Vector3 getPreviousAcceleration() {return lastFrameAcceleration;}

	double getMass();
	double getInverseMass();
	void setMass(double mass);
	bool hasFiniteMass();
	
};

#endif