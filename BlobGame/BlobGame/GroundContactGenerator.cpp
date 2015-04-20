#include "GroundContactGenerator.h"

unsigned GroundContactGenerator::addContact(ParticleContact* contact, unsigned limit)
{
	contact->mParticle[0] = other;
	contact->mParticle[1] = ground;

	float groundHeight = ground->getPosition().y;
	float otherPos = other->getPosition().y-radius;

	if (otherPos < groundHeight)
	{
		//contact->mPenetration = otherPos - groundHeight;
		contact->mPenetration = groundHeight - otherPos;
		contact->mRestitution = 0.1f;
		contact->mContactNormal = Vector3(0.0f, 1.0f, 0.0f);
		return 1;
	}

	return 0;
}