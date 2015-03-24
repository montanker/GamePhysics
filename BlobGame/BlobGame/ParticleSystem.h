#include "ParticleForceRegistry.h"
#include "ParticleContactResolver.h"
#include "ParticleContactGenerator.h"
#include <vector>

using namespace std;

class ParticleSystem
{
private:
	vector<Particle*> mParticleSet;
	vector<ParticleForceGenerator*> mParticleForceRegistry;
	vector<ParticleContactGenerator*> mContactGenerators;
	ParticleContactResolver mContactResolver;
	ParticleContact* mParticleContacts;
	unsigned mMaxContacts;
public:
	ParticleSystem();
	~ParticleSystem();

	void cleanUp();
	void update(float duration);
	void draw();
	void addParticle(Particle* newParticle);
	void addForce(ParticleForceGenerator* newForce);
	void applyForce(Particle* particle, ParticleForceGenerator* force);
	void applyForce(Particle* particle1, Particle* particle2, ParticleForceGenerator* force);
	unsigned generateContacts();
	ParticleForceRegistry registry;
};