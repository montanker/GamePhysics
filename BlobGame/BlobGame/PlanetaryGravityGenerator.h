#include "ParticleForceGenerator.h"

const double gravityConstant = 6.67384 * pow(10, -11);

class PlanetaryGravityGenerator:public ParticleForceGenerator
{
public:
	PlanetaryGravityGenerator();

	void updateForce(Particle* particle, float duration) {};
	void updateForce(Particle* particle1, Particle* particle2, float duration) ;
};