#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	mParticleSet = vector<Particle*>();
	mParticleForceRegistry = vector<ParticleForceGenerator*>();
	registry = ParticleForceRegistry();
}

ParticleSystem::~ParticleSystem()
{
	cleanUp();
}

void ParticleSystem::cleanUp()
{
	vector<Particle*>::iterator partIt = mParticleSet.begin();
	vector<ParticleForceGenerator*>::iterator forceIt = mParticleForceRegistry.begin();

	for (partIt; partIt != mParticleSet.end(); ++partIt)
	{
		delete(*partIt);
	}
	mParticleSet.clear();

	for(forceIt; forceIt != mParticleForceRegistry.end(); ++forceIt)
	{
		delete(*forceIt);
	}
	mParticleForceRegistry.clear();
}

void ParticleSystem::update(float duration)
{
	for (size_t i=0; i<mParticleSet.size(); i++)
	{
		mParticleSet[i]->update(duration);
	}

	registry.updateForces(duration);
}

void ParticleSystem::draw()
{
	for (size_t i=0; i<mParticleSet.size(); i++)
	{
		mParticleSet[i]->draw();
	}
}

void ParticleSystem::addParticle(Particle* newParticle)
{
	mParticleSet.push_back(newParticle);
}

void ParticleSystem::addForce(ParticleForceGenerator* newForce)
{
	mParticleForceRegistry.push_back(newForce);
}

void ParticleSystem::applyForce(Particle* particle, ParticleForceGenerator* force)
{
	//Check if sets already contain particle and force.  If not, add them.
	if (find(mParticleSet.begin(), mParticleSet.end(),particle) == mParticleSet.end())
	{
		addParticle(particle);
	}

	if (find(mParticleForceRegistry.begin(), mParticleForceRegistry.end(), force) == mParticleForceRegistry.end())
	{
		addForce(force);
	}

	registry.addForce(particle, force);
}