#ifndef __COLLISIONDETECTION_H
#define __COLLISIONDETECTION_H

#include "CollisionPrimitive.h"
#include "Contacts.h"
#include <float.h>

struct CollisionData
{
	CollisionData() : contactArray(NULL) { }
	~CollisionData()
	{
		if(contactArray != NULL) {
			delete contactArray;
		}
	}

	Contact *contactArray;
    Contact *contacts;
    int contactsLeft;
    unsigned contactCount;
    double friction;
    double restitution;
    double tolerance;

    bool hasMoreContacts();
    void reset(unsigned maxContacts);
    void addContacts(unsigned count);
};

class IntersectionTests
{
public:
	static double transformToAxis(const CollisionBox &box,const Vector3 &axis);
	static bool overlapOnAxis( const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCentre);
    static bool sphereAndPlane(const CollisionSphere &sphere, const CollisionPlane &plane);
    static bool sphereAndSphere(const CollisionSphere &one, const CollisionSphere &two);
    static bool boxAndBox(const CollisionBox &one, const CollisionBox &two);
    static bool boxAndPlane(const CollisionBox &box, const CollisionPlane &plane);
	static bool boxAndHalfSpace(const CollisionBox &box, const CollisionPlane &plane);

};

class CollisionDetector
{
private:
	static double penetrationOnAxis(const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCentre);
	static bool tryAxis(const CollisionBox &one, const CollisionBox &two, Vector3 axis, const Vector3& toCentre,
		                unsigned index, double& smallestPenetration, unsigned &smallestCase);
	static void fillPointFaceBoxBox(const CollisionBox &one, const CollisionBox &two, const Vector3 &toCentre,
                                    CollisionData *data, unsigned best, double pen);
	static Vector3 contactPoint(const Vector3 &pOne, const Vector3 &dOne, double oneSize, const Vector3 &pTwo,
                                const Vector3 &dTwo, double twoSize, bool useOne);
public:
    static unsigned sphereAndHalfSpace(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData *data);
    static unsigned sphereAndTruePlane(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData *data);
    static unsigned sphereAndSphere(const CollisionSphere &one, const CollisionSphere &two, CollisionData *data);
    static unsigned boxAndHalfSpace(const CollisionBox &box, const CollisionPlane &plane, CollisionData *data);
    static unsigned boxAndBox(const CollisionBox &one, const CollisionBox &two, CollisionData *data);
    static unsigned boxAndPoint(const CollisionBox &box, const Vector3 &point, CollisionData *data);
    static unsigned boxAndSphere(const CollisionBox &box, const CollisionSphere &sphere, CollisionData *data);
};

#endif