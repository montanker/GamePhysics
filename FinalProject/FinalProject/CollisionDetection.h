#include <vector>
#include "Vector3.h"
#include "RigidBody.h"
#include <cmath>

const double PI = 3.1415926535897;

struct BoundingSphere
{
	Vector3 center;
	double radius;

public:
	BoundingSphere(const Vector3 &sphereCenter, double sphereRadius);
	BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);
	bool overlaps(const BoundingSphere* other) const;
	double getSize() const;
};

struct BoundingBox
{
	Vector3 center;
	Vector3 halfSize;

public:
	double getSize();
};

struct PotentialContact
{
	RigidBody* body[2];
};

template<class BoundingVolumeClass>
class BVHNode
{
public:
	BVHNode(BVHNode* parent, const BoundingVolumeClass &volume, RigidBody* rigidbody=NULL)
		:mParent(parent), mVolume(volume), mBody(rigidbody)
	{
		mChildren[0] = mChildren[1] = NULL;
	}
	~BVHNode();
	bool isLeaf() const;
	void insert(RigidBody* body, const BoundingVolumeClass &newVolume);
	unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const;

	BVHNode* mParent;
	BVHNode* mChildren[2];
	BoundingVolumeClass mVolume;
	RigidBody* mBody;
protected:
	bool overlaps(const BVHNode<BoundingVolumeClass>* other) const;
	unsigned getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const;
};