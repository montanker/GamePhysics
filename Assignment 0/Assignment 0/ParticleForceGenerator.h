#pragma once

#include "Particle.h"

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float duration) = 0;
	virtual void updateForce(Particle* particle1, Particle* particle2, float duration) = 0;
};