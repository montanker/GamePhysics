#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry()
{
	//mForceRegistrations = Registry();
	mForceRegistrations = vector<ForceRegistration>();
}

void ParticleForceRegistry::addForce(Particle* newParticle, ParticleForceGenerator* newGenerator)
{
	ForceRegistration newReg = ForceRegistration();
	newReg.multipleParticles = false;
	newReg.particle1 = newParticle;
	newReg.particle2 = NULL;
	newReg.generator = newGenerator;
	
	mForceRegistrations.push_back(newReg);
}

void ParticleForceRegistry::addForce(Particle* newParticle1, Particle* newParticle2, ParticleForceGenerator* newGenerator)
{
	ForceRegistration newReg = ForceRegistration();
	newReg.multipleParticles = true;
	newReg.particle1 = newParticle1;
	newReg.particle2 = newParticle2;
	newReg.generator = newGenerator;
	
	mForceRegistrations.push_back(newReg);
}

void ParticleForceRegistry::updateForces(float duration)
{
	auto i = mForceRegistrations.begin();
	for (; i != mForceRegistrations.end(); ++i)
	{
		if (!i->multipleParticles)
		{
			i->generator->updateForce(i->particle1, duration);
		}
		else
		{
			i->generator->updateForce(i->particle1, i->particle2, duration);
		}
	}
}
