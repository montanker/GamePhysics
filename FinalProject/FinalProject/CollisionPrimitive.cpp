#include "CollisionPrimitive.h"

void CollisionPrimitive::calculateInternals()
{
	//transform = body->getTransform() * offset;
}

Vector3 CollisionPrimitive::getAxis(unsigned index) const
{
	return transform.getAxisVector(index);
}