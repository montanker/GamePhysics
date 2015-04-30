#include "RigidBodyForces.h"

RbGravity::RbGravity(const Vector3& grav)
{
	gravity = grav;
}

void RbGravity::updateForce(RigidBody* body, double duration)
{
    if (!body->hasFiniteMass()) 
	{
		return;
	}

    body->addForce(gravity * body->getMass());
}

RbSpring::RbSpring(const Vector3 &localConnectionPt, RigidBody *other, const Vector3 &otherConnectionPt,
                   double springConstant, double restLength)
{
	connectionPoint = localConnectionPt;
	otherConnectionPoint = otherConnectionPt;
	otherBody = other;
	springConst = springConstant;
	restitution = restLength;
}

void RbSpring::updateForce(RigidBody* body, double duration)
{
    Vector3 lws = body->getPointInWorldSpace(connectionPoint);
    Vector3 ows = otherBody->getPointInWorldSpace(otherConnectionPoint);
    Vector3 force = lws - ows;

	double magnitude = force.getMagnitude();
	magnitude = abs(magnitude - restitution);
    magnitude *= springConst;

	force.normalize();
    force *= -magnitude;
    body->addForceAtPoint(force, lws);
}