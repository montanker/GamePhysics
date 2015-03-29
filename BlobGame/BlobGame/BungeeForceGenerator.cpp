#include "BungeeForceGenerator.h"

BungeeForceGenerator::BungeeForceGenerator()
{
	mAnchor = Vector3(0,0,0);
	mSpringConstant = 0.0f;
	mRestLength = 0;
}

BungeeForceGenerator::BungeeForceGenerator(Vector3 anchor, float springConstant, double restLength)
{
	mAnchor = anchor;
	mSpringConstant = springConstant;
	mRestLength = restLength;
}

void BungeeForceGenerator::updateForce(Particle* particle, float duration)
{
	Vector3 force = particle->getPosition();
	force =  force - mAnchor;

	double magnitude = force.getMagnitude();
	if (magnitude <= mRestLength)
	{
		return;
	}
	magnitude = (mRestLength-magnitude) * mSpringConstant;

	force.normalize();
	force = force * magnitude;
	particle->addForce(force);
}