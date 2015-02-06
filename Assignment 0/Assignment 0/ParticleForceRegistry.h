#include "ParticleForceGenerator.h"
#include <vector>

using namespace std;

struct ForceRegistration
{
	Particle* particle;
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
	void updateForces(float duration);
};