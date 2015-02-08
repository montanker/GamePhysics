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
	~ParticleSystem();

	void cleanUp();
	void update(float duration);
	void draw();
	void addParticle(Particle* newParticle);
	void addForce(ParticleForceGenerator* newForce);
	void applyForce(Particle* particle, ParticleForceGenerator* force);
	ParticleForceRegistry registry;
};