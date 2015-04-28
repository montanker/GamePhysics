#include "CollisionPrimitive.h"

CollisionPrimitive::CollisionPrimitive()
{
}

void CollisionPrimitive::calculateInternals()
{
	transform = body->getTransform() * offset;
}

Vector3 CollisionPrimitive::getAxis(unsigned index) const
{
	return transform.getAxisVector(index);
}

void CollisionPrimitive::setAxis(unsigned index, Vector3 vals)
{
	transform.setAxisVector(index,vals.x,vals.y,vals.z);
}