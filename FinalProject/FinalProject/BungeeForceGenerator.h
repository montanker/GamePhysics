#include "ParticleForceGenerator.h"

class BungeeForceGenerator:public ParticleForceGenerator
{
private:
	Vector3 mAnchor;
	float mSpringConstant;
	double mRestLength;

public:
	BungeeForceGenerator();
	BungeeForceGenerator(Vector3 anchor, float springConstant, double restLength);

	void updateForce(Particle* particle, float duration);
	void updateForce(Particle* particle1, Particle* particle2, float duration) {} ;
};