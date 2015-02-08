#include "EarthGravityGenerator.h"

EarthGravityGenerator::EarthGravityGenerator()
{
	gravity = Vector3(0, 0, 0);
}

void EarthGravityGenerator::updateForce(Particle* particle, float duration)
{
	particle->addForce(gravity*particle->getMass());
}