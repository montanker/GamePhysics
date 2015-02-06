#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry()
{
	mForceRegistrations = Registry();
}

void ParticleForceRegistry::addForce(Particle* newParticle, ParticleForceGenerator* newGenerator)
{
	ForceRegistration newReg = ForceRegistration();
	newReg.particle = newParticle;
	newReg.generator = newGenerator;
	
	mForceRegistrations.push_back(newReg);
}

void ParticleForceRegistry::updateForces(float duration)
{
	auto i = mForceRegistrations.begin();
	for (; i != mForceRegistrations.end(); ++i)
	{
		i->generator->updateForce(i->particle, duration);
	}
}
