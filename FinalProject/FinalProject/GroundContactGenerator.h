#include "ParticleContactGenerator.h"

class GroundContactGenerator: public ParticleContactGenerator
{
public:
	Particle* ground;
	Particle* other;
	double radius;
	virtual unsigned addContact(ParticleContact* contact, unsigned limit);
};