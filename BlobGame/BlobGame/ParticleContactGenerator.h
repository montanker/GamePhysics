#include "ParticleContact.h"

class ParticleContactGenerator
{
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) = 0;
};