#include "ParticleForceGenerator.h"
#include <vector>

using namespace std;

struct ForceRegistration
{
	bool multipleParticles;
	Particle* particle1;
	Particle* particle2;
	ParticleForceGenerator *generator;
};

typedef vector<ForceRegistration> Registry;

class ParticleForceRegistry
{
private:
	Registry mForceRegistrations;
public:
	ParticleForceRegistry();

	void addForce(Particle* newParticle, ParticleForceGenerator* newGenerator);
	void addForce(Particle* newParticle1, Particle* newParticle2, ParticleForceGenerator* newGenerator);
	void updateForces(float duration);
};