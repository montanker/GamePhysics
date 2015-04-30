#include "CollisionDetection.h"

bool CollisionData::hasMoreContacts()
{
	return contactsLeft > 0;
}

void CollisionData::reset(unsigned maxContacts)
{
	if(contactArray != NULL) 
	{
		delete contactArray;
	}
	contactsLeft = maxContacts;
    contactCount = 0;
    contactArray = new Contact[maxContacts];
	contacts = contactArray;
}

void CollisionData::addContacts(unsigned count)
{
	contactsLeft -= count;
    contactCount += count;
    contacts += count;
}

double IntersectionTests::transformToAxis(const CollisionBox &box,const Vector3 &axis)
{
	Vector3 temp = axis;

	return box.halfSize.x * abs(temp.dotProduct(box.getAxis(0))) +
		   box.halfSize.y * abs(temp.dotProduct(box.getAxis(1))) +
		   box.halfSize.z * abs(temp.dotProduct(box.getAxis(2)));
}

bool IntersectionTests::overlapOnAxis( const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCentre)
{
	double oneProject = transformToAxis(one, axis);
    double twoProject = transformToAxis(two, axis);

	Vector3 dir = toCentre;
	double distance = abs(dir.dotProduct(axis));

    return (distance < oneProject + twoProject);
}

bool IntersectionTests::sphereAndPlane(const CollisionSphere &sphere, const CollisionPlane &plane)
{
	Vector3 dir = plane.direction;
	double ballDistance = dir.dotProduct(sphere.getAxis(3)) - sphere.radius;

    return ballDistance <= plane.offset;
}

bool IntersectionTests::sphereAndSphere(const CollisionSphere &one, const CollisionSphere &two)
{
    Vector3 midline = one.getAxis(3) - two.getAxis(3);

    return midline.squareMagnitude() < pow((one.radius+two.radius),2);
}

bool IntersectionTests::boxAndBox(const CollisionBox &one, const CollisionBox &two)
{
    Vector3 toCenter = two.getAxis(3) - one.getAxis(3);

    return (
        overlapOnAxis(one, two, one.getAxis(0), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(1), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(2), toCenter) &&

        overlapOnAxis(one, two, two.getAxis(0), toCenter) &&
        overlapOnAxis(one, two, two.getAxis(1), toCenter) &&
        overlapOnAxis(one, two, two.getAxis(2), toCenter) &&

		overlapOnAxis(one, two, one.getAxis(0).crossProduct(two.getAxis(0)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(0).crossProduct(two.getAxis(1)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(0).crossProduct(two.getAxis(2)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(1).crossProduct(two.getAxis(0)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(1).crossProduct(two.getAxis(1)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(1).crossProduct(two.getAxis(2)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(2).crossProduct(two.getAxis(0)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(2).crossProduct(two.getAxis(1)), toCenter) &&
        overlapOnAxis(one, two, one.getAxis(2).crossProduct(two.getAxis(2)), toCenter)
    );
}

bool IntersectionTests::boxAndPlane(const CollisionBox &box, const CollisionPlane &plane)
{
    double projectedRadius = transformToAxis(box, plane.direction);

	Vector3 planeDir = plane.direction;
	double boxDistance = planeDir.dotProduct(box.getAxis(3)) - projectedRadius;

    return boxDistance <= plane.offset;
}

bool IntersectionTests::boxAndHalfSpace(const CollisionBox &box, const CollisionPlane &plane)
{
    double projectedRadius = transformToAxis(box, plane.direction);
    double boxDistance = plane.direction.dotProduct(box.getAxis(3)) - projectedRadius;

    return boxDistance <= plane.offset;
}

double CollisionDetector::penetrationOnAxis(const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCentre)
{
	double oneProject = IntersectionTests::transformToAxis(one, axis);
    double twoProject = IntersectionTests::transformToAxis(two, axis);

	double distance = abs(toCentre.dotProduct(axis));

    return oneProject + twoProject - distance;
}

bool CollisionDetector::tryAxis(const CollisionBox &one, const CollisionBox &two, Vector3 axis, const Vector3& toCentre,
		     unsigned index, double& smallestPenetration, unsigned &smallestCase)
{
	if (axis.squareMagnitude() < 0.0001) 
	{
		return true;
	}

	axis.normalize();

    double penetration = penetrationOnAxis(one, two, axis, toCentre);

    if (penetration < 0) 
	{
		return false;
	}

    if (penetration < smallestPenetration) 
	{
        smallestPenetration = penetration;
        smallestCase = index;
    }
    return true;
}

void CollisionDetector::fillPointFaceBoxBox(const CollisionBox &one, const CollisionBox &two, const Vector3 &toCentre,
                         CollisionData *data, unsigned best, double pen)
{
	Contact* contact = data->contacts;

    Vector3 normal = one.getAxis(best);
	if (one.getAxis(best).dotProduct(toCentre) > 0)
    {
        normal = normal * -1.0f;
    }

    Vector3 vertex = two.halfSize;
	if (two.getAxis(0).dotProduct(normal) < 0) 
	{
		vertex.x = -vertex.x;
	}
    if (two.getAxis(1).dotProduct(normal) < 0)
	{
		vertex.y = -vertex.y;
	}
    if (two.getAxis(2).dotProduct(normal) < 0)
	{
		vertex.z = -vertex.z;
	}

    contact->contactNormal = normal;
    contact->penetration = pen;
    contact->contactPoint = two.getTransform() * vertex;
    contact->setBodyData(one.body, two.body, data->friction, data->restitution);
}

Vector3 CollisionDetector::contactPoint(const Vector3 &pOne, const Vector3 &dOne, double oneSize, const Vector3 &pTwo,
                     const Vector3 &dTwo, double twoSize, bool useOne)
{
	Vector3 toSt, cOne, cTwo;
    double dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    double denom, mua, mub;

    smOne = dOne.squareMagnitude();
    smTwo = dTwo.squareMagnitude();
	dpOneTwo = dTwo.dotProduct(dOne);

    toSt = pOne - pTwo;
	dpStaOne = dOne.dotProduct(toSt);
	dpStaTwo = dTwo.dotProduct(toSt);

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    if (abs(denom) < 0.0001f) 
	{
        return useOne?pOne:pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    if (mua > oneSize || mua < -oneSize || mub > twoSize || mub < -twoSize)
    {
        return useOne?pOne:pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5 + cTwo * 0.5;
    }
}

unsigned CollisionDetector::sphereAndHalfSpace(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData *data)
{
    if (data->contactsLeft <= 0) 
	{
		return 0;
	}

    Vector3 position = sphere.getAxis(3);
	double ballDistance = plane.direction.dotProduct(position) - sphere.radius - plane.offset;

    if (ballDistance >= 0) 
	{
		return 0;
	}

    Contact* contact = data->contacts;
    contact->contactNormal = plane.direction;
    contact->penetration = -ballDistance;
    contact->contactPoint =  position - plane.direction * (ballDistance + sphere.radius);
    contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);
    data->addContacts(1);

    return 1;
}

unsigned CollisionDetector::sphereAndTruePlane(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData *data)
{
    if (data->contactsLeft <= 0)
	{
		return 0;
	}

    Vector3 position = sphere.getAxis(3);
	double centreDistance = plane.direction.dotProduct(position) - plane.offset;

    if (centreDistance*centreDistance > sphere.radius*sphere.radius)
    {
        return 0;
    }

    Vector3 normal = plane.direction;
    double penetration = -centreDistance;
    if (centreDistance < 0)
    {
        normal *= -1;
        penetration = -penetration;
    }
    penetration += sphere.radius;

    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = position - plane.direction * centreDistance;
    contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);
    data->addContacts(1);

    return 1;
}

unsigned CollisionDetector::sphereAndSphere(const CollisionSphere &one, const CollisionSphere &two, CollisionData *data)
{
    if (data->contactsLeft <= 0) 
	{
		return 0;
	}

    Vector3 positionOne = one.getAxis(3);
    Vector3 positionTwo = two.getAxis(3);

    Vector3 midline = positionOne - positionTwo;
    double size = midline.getMagnitude();

    if (size <= 0.0f || size >= one.radius+two.radius)
    {
        return 0;
    }

    Vector3 normal = midline * (((double)1.0)/size);

    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = positionOne + midline * (double)0.5;
    contact->penetration = (one.radius+two.radius - size);
    contact->setBodyData(one.body, two.body, data->friction, data->restitution);
    data->addContacts(1);

    return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(const CollisionBox &box, const CollisionPlane &plane, CollisionData *data)
{
	if (data->contactsLeft <= 0) return 0;

    if (!IntersectionTests::boxAndHalfSpace(box, plane))
    {
        return 0;
    }

    static double mults[8][3] = {{1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                                 {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1}};

    Contact* contact = data->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++) {
        Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		Vector3 h = box.halfSize;
		vertexPos = Vector3(vertexPos.x*h.x,vertexPos.y*h.y,vertexPos.z*h.z);
        vertexPos = box.transform.transform(vertexPos);

        double vertexDistance = vertexPos.dotProduct(plane.direction);

        if (vertexDistance <= plane.offset)
        {
            contact->contactPoint = plane.direction;
            contact->contactPoint *= (vertexDistance-plane.offset);
            contact->contactPoint += vertexPos;
            contact->contactNormal = plane.direction;
            contact->penetration = plane.offset - vertexDistance;

            contact->setBodyData(box.body, NULL, data->friction, data->restitution);

            contact++;
            contactsUsed++;
            if (contactsUsed == (unsigned)data->contactsLeft) return contactsUsed;
        }
    }

    data->addContacts(contactsUsed);
    return contactsUsed;
}

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) \
	{                                                             \
		return 0;                                                 \
	}

unsigned CollisionDetector::boxAndBox(const CollisionBox &one, const CollisionBox &two, CollisionData *data)
{
	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

	double pen = DBL_MAX;
    unsigned best = 0xffffff;

    CHECK_OVERLAP(one.getAxis(0), 0);
    CHECK_OVERLAP(one.getAxis(1), 1);
    CHECK_OVERLAP(one.getAxis(2), 2);

    CHECK_OVERLAP(two.getAxis(0), 3);
    CHECK_OVERLAP(two.getAxis(1), 4);
    CHECK_OVERLAP(two.getAxis(2), 5);

    unsigned bestSingleAxis = best;

	CHECK_OVERLAP(one.getAxis(0).crossProduct(two.getAxis(0)), 6);
    CHECK_OVERLAP(one.getAxis(0).crossProduct(two.getAxis(1)), 7);
    CHECK_OVERLAP(one.getAxis(0).crossProduct(two.getAxis(2)), 8);
    CHECK_OVERLAP(one.getAxis(1).crossProduct(two.getAxis(0)), 9);
    CHECK_OVERLAP(one.getAxis(1).crossProduct(two.getAxis(1)), 10);
    CHECK_OVERLAP(one.getAxis(1).crossProduct(two.getAxis(2)), 11);
    CHECK_OVERLAP(one.getAxis(2).crossProduct(two.getAxis(0)), 12);
    CHECK_OVERLAP(one.getAxis(2).crossProduct(two.getAxis(1)), 13);
    CHECK_OVERLAP(one.getAxis(2).crossProduct(two.getAxis(2)), 14);

    assert(best != 0xffffff);

    if (best < 3)
    {
        fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
        data->addContacts(1);
        return 1;
    }
    else if (best < 6)
    {
        fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best-3, pen);
        data->addContacts(1);
        return 1;
    }
    else
    {
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        Vector3 oneAxis = one.getAxis(oneAxisIndex);
        Vector3 twoAxis = two.getAxis(twoAxisIndex);
		Vector3 axis = oneAxis.crossProduct(twoAxis);
        axis.normalize();

        if (axis.dotProduct(toCentre) > 0) 
		{
			axis = axis * -1.0f;
		}

        Vector3 ptOnOneEdge = one.halfSize;
        Vector3 ptOnTwoEdge = two.halfSize;
        for (unsigned i = 0; i < 3; i++)
        {
            if (i == oneAxisIndex)
			{
				ptOnOneEdge[i] = 0;
			}
			else if (one.getAxis(i).dotProduct(axis) > 0) 
			{
				ptOnOneEdge[i] = -ptOnOneEdge[i];
			}

            if (i == twoAxisIndex)
			{
				ptOnTwoEdge[i] = 0;
			}
            else if (two.getAxis(i).dotProduct(axis) < 0) 
			{
				ptOnTwoEdge[i] = -ptOnTwoEdge[i];
			}
        }

        ptOnOneEdge = one.transform * ptOnOneEdge;
        ptOnTwoEdge = two.transform * ptOnTwoEdge;

        Vector3 vertex = contactPoint(
            ptOnOneEdge, oneAxis, one.halfSize[(int)oneAxisIndex],
            ptOnTwoEdge, twoAxis, two.halfSize[(int)twoAxisIndex],
            bestSingleAxis > 2
            );

        Contact* contact = data->contacts;

        contact->penetration = pen;
        contact->contactNormal = axis;
        contact->contactPoint = vertex;
        contact->setBodyData(one.body, two.body, data->friction, data->restitution);
        data->addContacts(1);

        return 1;
    }
    return 0;
}

unsigned CollisionDetector::boxAndPoint(const CollisionBox &box, const Vector3 &point, CollisionData *data)
{
    Vector3 relPt = box.transform.transformInverse(point);

    Vector3 normal;

    double min_depth = box.halfSize.x - abs(relPt.x);
    if (min_depth < 0) return 0;
    normal = box.getAxis(0) * ((relPt.x < 0)?-1:1);

    double depth = box.halfSize.y - abs(relPt.y);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(1) * ((relPt.y < 0)?-1:1);
    }

    depth = box.halfSize.z - abs(relPt.z);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(2) * ((relPt.z < 0)?-1:1);
    }

    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->penetration = min_depth;

    contact->setBodyData(box.body, NULL,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::boxAndSphere(const CollisionBox &box, const CollisionSphere &sphere, CollisionData *data)
{
	Vector3 centre = sphere.getAxis(3);
    Vector3 relCentre = box.transform.transformInverse(centre);

    if (abs(relCentre.x) - sphere.radius > box.halfSize.x ||
        abs(relCentre.y) - sphere.radius > box.halfSize.y ||
        abs(relCentre.z) - sphere.radius > box.halfSize.z)
    {
        return 0;
    }

    Vector3 closestPt(0,0,0);
    double dist;

    dist = relCentre.x;
    if (dist > box.halfSize.x) dist = box.halfSize.x;
    if (dist < -box.halfSize.x) dist = -box.halfSize.x;
    closestPt.x = dist;

    dist = relCentre.y;
    if (dist > box.halfSize.y) dist = box.halfSize.y;
    if (dist < -box.halfSize.y) dist = -box.halfSize.y;
    closestPt.y = dist;

    dist = relCentre.z;
    if (dist > box.halfSize.z) dist = box.halfSize.z;
    if (dist < -box.halfSize.z) dist = -box.halfSize.z;
    closestPt.z = dist;

    dist = (closestPt - relCentre).squareMagnitude();
    if (dist > sphere.radius * sphere.radius) return 0;

    Vector3 closestPtWorld = box.transform.transform(closestPt);

    Contact* contact = data->contacts;
    contact->contactNormal = (closestPtWorld - centre);
    contact->contactNormal.normalize();
    contact->contactPoint = closestPtWorld;
    contact->penetration = sphere.radius - sqrt(dist);
    contact->setBodyData(box.body, sphere.body,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}