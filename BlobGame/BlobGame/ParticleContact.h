#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"

const Color CONTACT_COLOR = Color(1.0f, 0.0f, 0.0f);
const float CONTACT_WIDTH = 5.0f;

class ParticleContact
{
public:
	ParticleContact(){};
	double calculateSeparatingVelocity();
	void resolve(float duration);
	void init();
	void draw();

	Particle* mParticle[2];
	Vector3 mParticleMovement[2];
	double mRestitution;
	double mPenetration;
	bool drawLine;
	bool use;
	Color color;
	Vector3 mContactNormal;
	

protected:
	//void init();
	//void resolve(float duration);
	//double calculateSeparatingVelocity();

private:
	void resolveVelocity(float duration);
	void resolveInterpenetration(float duration);
};

#endif