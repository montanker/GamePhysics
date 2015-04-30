#ifndef CONTACTS_H
#define CONTACTS_H

#include "RigidBody.h"
#include "Vector3.h"
#include "Matrix3.h"

class Contact
{
	friend class ContactResolver;

public:
	void setBodyData(RigidBody* one, RigidBody *two, double contactFriction, double contactRestitution);

	RigidBody* bodies[2];
	double friction;
	double restitution;
	double penetration;
	Vector3 contactPoint;
	Vector3 contactNormal;

protected:
	Vector3 calculateLocalVelocity(unsigned bodyIndex, double duration);
	Vector3 calculateFrictionlessImpulse(Matrix3 *inverseInertiaTensor);
    Vector3 calculateFrictionImpulse(Matrix3 *inverseInertiaTensor);
	void calculateInternals(double duration);
	void swapBodies();
	void matchAwakeState();
	void calculateContactBasis();
	void calculateDesiredDeltaVelocity(double duration);
	void applyImpulse(const Vector3 &impulse, RigidBody *body, Vector3 *velocityChange, Vector3 *rotationChange);
    void applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);
    void applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], double penetration);

	Matrix3 contactToWorld;
	Vector3 contactVelocity;
	Vector3 relativeContactPosition[2];
	double desiredDeltaVelocity;
	
};

class ContactResolver
{
public:
	ContactResolver();
	ContactResolver(unsigned iterations, double velocityEpsilon=(double)0.01, double positionEpsilon=(double)0.01);
    ContactResolver(unsigned velocityIterations, unsigned positionIterations, double velocityEpsilon=(double)0.01, double positionEpsilon=(double)0.01);
	bool isValid();
	void setIterations(unsigned velocityIterations, unsigned positionIterations);
	void setIterations(unsigned iterations);
	void setEpsilon(double velocityEpsilon, double positionEpsilon);
	void resolveContacts(Contact *contactArray, unsigned numContacts, double duration);

	unsigned velocityIterationsUsed;
	unsigned positionIterationsUsed;

private:
	bool validSettings;

protected:
	void prepareContacts(Contact *contactArray, unsigned numContacts, double duration);
    void adjustVelocities(Contact *contactArray, unsigned numContacts, double duration);
    void adjustPositions(Contact *contacts, unsigned numContacts, double duration);

	unsigned velocityIterations;
	unsigned positionIterations;
	double velocityEpsilon;
	double positionEpsilon;
};

class ContactGenerator
{
public:
    virtual unsigned addContact(Contact *contact, unsigned limit) const = 0;
};

#endif