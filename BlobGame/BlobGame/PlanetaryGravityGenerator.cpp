#include "PlanetaryGravityGenerator.h"

PlanetaryGravityGenerator::PlanetaryGravityGenerator()
{
}

void PlanetaryGravityGenerator::updateForce(Particle* particle1, Particle* particle2, float duration)
{
	Vector3 direction = particle2->getPosition() - particle1->getPosition();
	direction.normalize();

	double masses = (double)particle1->getMass() * (double)particle2->getMass();
	double dist = particle1->getPosition().distance(particle2->getPosition());
	double gravScale = gravityConstant * (masses/pow(dist, 2));

	particle1->addForce(direction * gravScale);
}