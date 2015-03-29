#include "AnchoredSpringGenerator.h"

AnchoredSpringGenerator::AnchoredSpringGenerator()
{
	mAnchor = Vector3(0,0,0);
	mSpringConstant = 0.0f;
	mRestLength = 0;
}

AnchoredSpringGenerator::AnchoredSpringGenerator(Vector3 anchor, float springConstant, double restLength)
{
	mAnchor = anchor;
	mSpringConstant = springConstant;
	mRestLength = restLength;
}

void AnchoredSpringGenerator::updateForce(Particle* particle, float duration)
{
	Vector3 force = particle->getPosition();
	force =  force - mAnchor;

	double magnitude = force.getMagnitude();
	magnitude = (mRestLength-magnitude) * mSpringConstant;

	force.normalize();
	force = force * magnitude;
	particle->addForce(force);
}