#include "Particle.h"

class ParticleContact
{
public:
	ParticleContact(){};

	Particle* mParticle[2];
	Vector3 mParticleMovement[2];
	double mRestitution;
	double mPenetration;
	Vector3 mContactNormal;

protected:
	void init();
	void resolve(float duration);
	double calculateSeparatingVelocity();

private:
	void resolveVelocity(float duration);
	void resolveInterpenetration(float duration);
};