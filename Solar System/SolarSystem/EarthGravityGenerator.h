#include "ParticleForceGenerator.h"

class EarthGravityGenerator:public ParticleForceGenerator
{
private:
	Vector3 gravity;

public:
	EarthGravityGenerator();

	void updateForce(Particle* particle, float duration);
	void updateForce(Particle* particle1, Particle* particle2, float duration) {};
};