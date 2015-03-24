#include "ParticleContactGenerator.h"

class ParticleLink: public ParticleContactGenerator
{
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) = 0;
	Particle* particle[2];

protected:
	double currentLength();
};

class ParticleCable: public ParticleLink
{
public:
	double maxLength;
	double restitution;

	virtual unsigned addContact(ParticleContact* contact, unsigned limit);
};

class ParticleRod: public ParticleLink
{
public:
	double length;
	virtual unsigned addContact(ParticleContact* contact, unsigned limit);
};