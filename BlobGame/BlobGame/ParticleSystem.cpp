#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	mParticleSet = vector<Particle*>();
	mParticleForceRegistry = vector<ParticleForceGenerator*>();
	mContactGenerators = vector<ParticleContactGenerator*>();
	mContactResolver = ParticleContactResolver(0);
	//mParticleContacts = new ParticleContact;
	mParticleContacts = vector<ParticleContact*>();
	mDrawContacts = vector<ParticleContact*>();
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
		//mContactResolver.resolveContacts(mParticleContacts, usedContacts, duration);
		mContactResolver.resolveContacts(mParticleContacts, usedContacts, duration);
	}
}

void ParticleSystem::draw()
{
	for (size_t i=0; i<mParticleSet.size(); i++)
	{
		mParticleSet[i]->draw();
	}

	for (size_t j=0; j<mParticleContacts.size(); j++)
	{
		mParticleContacts[j]->draw();
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

void ParticleSystem::addContact(ParticleContactGenerator* newContact)
{
	mContactGenerators.push_back(newContact);
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

void ParticleSystem::applyContact(Particle* particle1, Particle* particle2, ParticleContactGenerator* contact)
{
	if (find(mParticleSet.begin(), mParticleSet.end(),particle1) == mParticleSet.end())
	{
		addParticle(particle1);
	}

	if (find(mParticleSet.begin(), mParticleSet.end(),particle2) == mParticleSet.end())
	{
		addParticle(particle2);
	}

	addContact(contact);
	/*if (find(mContactGenerators.begin(), mContactGenerators.end(), contact) == mContactGenerators.end())
	{
		addContact(contact);
	}*/

	//ParticleContact* particleContact = new ParticleContact();
	//particleContact->init();
	//particleContact->mParticle[0] = particle1;
	//particleContact->mParticle[1] = particle2;
	//contact->addContact(particleContact, 0);
	//registry.addForce(particle1, particle2, contact);
}

unsigned ParticleSystem::generateContacts()
{
	unsigned limit = mMaxContacts;
	//ParticleContact *nextContact = mParticleContacts;

	vector<ParticleContact*>::iterator contactIt = mParticleContacts.begin();
	for (contactIt; contactIt != mParticleContacts.end(); ++contactIt)
	{
		delete(*contactIt);
	}
	mParticleContacts.clear();

	vector<ParticleContactGenerator*>::iterator g;

	for(g = mContactGenerators.begin(); g != mContactGenerators.end(); g++)
	{
		ParticleContact* nextContact = new ParticleContact();
		nextContact->init();
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		//nextContact->use = (used > 0);
		if (used > 0)
		{
			nextContact->use = true;
			mParticleContacts.push_back(nextContact);
		}

		if (limit <= 0)
		{
			break;
		}
	}

	return mMaxContacts - limit;
}