#include "EarthGravityGenerator.h"

EarthGravityGenerator::EarthGravityGenerator()
{
	gravity = Vector3(0, -9.8f, 0);
}

void EarthGravityGenerator::updateForce(Particle* particle, float duration)
{
	particle->addForce(gravity*particle->getMass());
}