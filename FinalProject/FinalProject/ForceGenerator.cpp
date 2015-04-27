#include "ForceGenerator.h"

void Gravity::updateForce(RigidBody *body, double duration)
{
	if (!body->hasFiniteMass()) return;

	body->addForce(gravity * body->getMass());
}

void Spring::updateForce(RigidBody *body, double duration)
{
	Vector3 lws = body->getPointInWorldSpace(connectionPoint);
	Vector3 ows = other->getPointInWorldSpace(otherConnectionPoint);

	Vector3 force = lws - ows;
	double magnetude = force.getMagnitude();
	magnetude *= springConstant;
	force.normalize();
	force *= -magnetude;
	body->addForceAtPoint(force, lws);
}