#include "ParticleLink.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

double ParticleLink::currentLength()
{
	Vector3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
	return relativePos.getMagnitude();
}

unsigned ParticleCable::addContact(ParticleContact* contact, unsigned limit)
{
	double length = currentLength();

	contact->drawLine = true;
	contact->color = lineColor;
	contact->mParticle[0] = particle[0];
	contact->mParticle[1] = particle[1];

	if (length < maxLength)
	{
		return 0;
	}

	

	Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
	normal.normalize();

	contact->mContactNormal = normal;
	contact->mPenetration = length-maxLength;
	contact->mRestitution = restitution;

	return 1;
}

unsigned ParticleRod::addContact(ParticleContact* contact, unsigned limit)
{
	double currentLen = currentLength();

	contact->drawLine = true;
	contact->color = lineColor;
	contact->mParticle[0] = particle[0];
	contact->mParticle[1] = particle[1];

	if (currentLen == length)
	{
		return 0;
	}

	Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
	normal.normalize();

	if (currentLen > length)
	{
		contact->mContactNormal = normal;
		contact->mPenetration = currentLen - length;
	}
	else
	{
		contact->mContactNormal = normal * -1;
		contact->mPenetration = length - currentLen;
	}

	//Rods have no bounciness
	contact->mRestitution = 0.0f;

	return 1;
}