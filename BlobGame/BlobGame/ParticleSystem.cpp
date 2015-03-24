#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	mParticleSet = vector<Particle*>();
	mParticleForceRegistry = vector<ParticleForceGenerator*>();
	mContactGenerators = vector<ParticleContactGenerator*>();
	mContactResolver = ParticleContactResolver(0);
	mParticleContacts = new ParticleContact;
	registry = ParticleForceRegistry();
	mMaxContacts = 99;
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

	unsigned usedContacts = generateContacts();

	if (usedContacts)
	{
		bool calculateIterations = true;  //<-- The hell is this supposed to be?
		if (calculateIterations)  
		{
			mContactResolver.setIterations(usedContacts * 2);
		}
		mContactResolver.resolveContacts(mParticleContacts, usedContacts, duration);
	}
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

void ParticleSystem::applyForce(Particle* particle1, Particle* particle2, ParticleForceGenerator* force)
{
	//Check if sets already contain particle and force.  If not, add them.
	if (find(mParticleSet.begin(), mParticleSet.end(),particle1) == mParticleSet.end())
	{
		addParticle(particle1);
	}

	if (find(mParticleSet.begin(), mParticleSet.end(),particle2) == mParticleSet.end())
	{
		addParticle(particle2);
	}

	if (find(mParticleForceRegistry.begin(), mParticleForceRegistry.end(), force) == mParticleForceRegistry.end())
	{
		addForce(force);
	}

	registry.addForce(particle1, particle2, force);
}

unsigned ParticleSystem::generateContacts()
{
	unsigned limit = mMaxContacts;
	ParticleContact *nextContact = mParticleContacts;

	vector<ParticleContactGenerator*>::iterator g;

	for(g = mContactGenerators.begin(); g != mContactGenerators.end(); g++)
	{
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0)
		{
			break;
		}
	}

	return mMaxContacts - limit;
}