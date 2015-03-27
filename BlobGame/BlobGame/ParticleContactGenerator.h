#ifndef PARTICLECONTACTGENERATOR_H
#define PARTICLECONTACTGENERATOR_H

#include "ParticleContact.h"

class ParticleContactGenerator
{
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) = 0;
};

#endif