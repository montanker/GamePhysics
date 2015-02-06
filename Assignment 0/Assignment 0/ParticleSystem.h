#include "ParticleForceRegistry.h"
#include <vector>

using namespace std;

class ParticleSystem
{
private:
	vector<Particle*> mParticleSet;
	vector<ParticleForceGenerator*> mParticleForceRegistry;
public:
	ParticleSystem();

	void update(float duration);
	void addParticle(Particle* newParticle);
	void addForce(ParticleForceGenerator* newForce);
	ParticleForceRegistry registry;
};