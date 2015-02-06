#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	mParticleSet = vector<Particle*>();
	mParticleForceRegistry = vector<ParticleForceGenerator*>();
	registry = ParticleForceRegistry();
}

void ParticleSystem::update(float duration)
{
	for (int i=0; i<mParticleSet.size; i++)
	{
		mParticleSet[i]->update(duration);
	}

	registry.updateForces(duration);
}