#include "CollisionDetection.h"

BoundingSphere::BoundingSphere(const Vector3 &sphereCenter, double sphereRadius)
{
	center = sphereCenter;
	radius = sphereRadius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &one, const BoundingSphere &two)
{
	Vector3 centerOffset = two.center - one.center;
	double distance = centerOffset.squareMagnitude();
	double radiusDiff = two.radius - one.radius;

	if (pow(radiusDiff,2) >= distance)
	{
		if (one.radius > two.radius)
		{
			center = one.center;
			radius = one.radius;
		}
		else
		{
			center = two.center;
			radius = two.radius;
		}
	}
	else
	{
		distance = sqrt(distance);
		radius = (distance+one.radius+two.radius) * ((double)0.5f);
		center = one.center;
		if (distance > 0)
		{
			center += centerOffset * ((radius - one.radius)/distance);
		}
	}
}

bool BoundingSphere::overlaps(const BoundingSphere* other) const
{
	double distanceSquared = (center - other->center).squareMagnitude();
	
	return distanceSquared < (radius*other->radius)*(radius+other->radius);
}

double BoundingSphere::getSize() const
{
	return ((double)1.333333) * PI * pow(radius,3);
}

template<class BoundingVolumeClass>
BVHNode<BoundingVolumeClass>::~BVHNode()
{
}

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::isLeaf() const
{
	return (mBody != NULL);
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::insert(RigidBody* body, const BoundingVolumeClass &newVolume)
{
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, unsigned limit) const
{
	if (isLeaf() || limit==0)
	{
		return 0;
	}

	return mChildren[0]->getPotentialContactsWith(mChildren[1], contacts, limit);
}

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass>* other) const
{
	return mVolume->overlaps(other->mVolume);
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const
{
	if (!overlaps(other) || limit==0)
	{
		return 0;
	}

	if (isLeaf() && other->isLeaf())
	{
		contacts->body[0] = mBody;
		contacts->body[1] = other->mBody;
		return 1;
	}

	if (other->isLeaf() || (!isLeaf() && mVolume->getSize() >= other->mVolume->getSize()))
	{
		unsigned count = mChildren[0]->getPotentialContactsWith(other, contacts, limit);
	}
}