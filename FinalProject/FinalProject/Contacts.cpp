#include "Contacts.h"

void Contact::setBodyData(RigidBody* one, RigidBody *two, double contactFriction, double contactRestitution)
{
	bodies[0] = one;
	bodies[1] = two;
	friction = contactFriction;
	restitution = contactRestitution;
}

Vector3 Contact::calculateLocalVelocity(unsigned bodyIndex, double duration)
{
	RigidBody *thisBody = bodies[bodyIndex];

	Vector3 velocity = thisBody->getRotation().crossProduct(relativeContactPosition[bodyIndex]);
    velocity += thisBody->getVelocity();

    Vector3 contactVelocity = contactToWorld.transformTranspose(velocity);
    Vector3 accVelocity = thisBody->getPreviousAcceleration() * duration;
    accVelocity = contactToWorld.transformTranspose(accVelocity);
    accVelocity.x = 0;
    contactVelocity += accVelocity;

    return contactVelocity;
}

Vector3 Contact::calculateFrictionlessImpulse(Matrix3 *inverseInertiaTensor)
{
	Vector3 impulseContact;

	Vector3 deltaVelWorld = relativeContactPosition[0].crossProduct(contactNormal);
    deltaVelWorld = inverseInertiaTensor[0].transform(deltaVelWorld);
	deltaVelWorld = deltaVelWorld.crossProduct(relativeContactPosition[0]);

	double deltaVelocity = deltaVelWorld.dotProduct(contactNormal);
    deltaVelocity += bodies[0]->getInverseMass();

    if (bodies[1])
    {
		Vector3 deltaVelWorld = relativeContactPosition[1].crossProduct(contactNormal);
        deltaVelWorld = inverseInertiaTensor[1].transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld.crossProduct(relativeContactPosition[1]);

		deltaVelocity += deltaVelWorld.dotProduct(contactNormal);
        deltaVelocity += bodies[1]->getInverseMass();
    }

    impulseContact.x = desiredDeltaVelocity / deltaVelocity;
    impulseContact.y = 0;
    impulseContact.z = 0;
    return impulseContact;
}

Vector3 Contact::calculateFrictionImpulse(Matrix3 *inverseInertiaTensor)
{
	Vector3 impulseContact;
    double inverseMass = bodies[0]->getInverseMass();

    Matrix3 impulseToTorque;
    impulseToTorque.setSkewSymmetric(relativeContactPosition[0]);

    Matrix3 deltaVelWorld = impulseToTorque;
    deltaVelWorld *= inverseInertiaTensor[0];
    deltaVelWorld *= impulseToTorque;
    deltaVelWorld *= -1;

    if (bodies[1])
    {
        impulseToTorque.setSkewSymmetric(relativeContactPosition[1]);

        Matrix3 deltaVelWorld2 = impulseToTorque;
        deltaVelWorld2 *= inverseInertiaTensor[1];
        deltaVelWorld2 *= impulseToTorque;
        deltaVelWorld2 *= -1;

        deltaVelWorld += deltaVelWorld2;

        inverseMass += bodies[1]->getInverseMass();
    }

    Matrix3 deltaVelocity = contactToWorld.transpose();
    deltaVelocity *= deltaVelWorld;
    deltaVelocity *= contactToWorld;

    deltaVelocity.values[0] += inverseMass;
    deltaVelocity.values[4] += inverseMass;
    deltaVelocity.values[8] += inverseMass;

    Matrix3 impulseMatrix = deltaVelocity.inverse();

    Vector3 velKill(desiredDeltaVelocity, -contactVelocity.y, -contactVelocity.z);

    impulseContact = impulseMatrix.transform(velKill);

    double planarImpulse = sqrt(impulseContact.y*impulseContact.y + impulseContact.z*impulseContact.z);
    if (planarImpulse > impulseContact.x * friction)
    {
        impulseContact.y /= planarImpulse;
        impulseContact.z /= planarImpulse;

        impulseContact.x = deltaVelocity.values[0] + deltaVelocity.values[1]*friction*impulseContact.y +
                           deltaVelocity.values[2]*friction*impulseContact.z;
        impulseContact.x = desiredDeltaVelocity / impulseContact.x;
        impulseContact.y *= friction * impulseContact.x;
        impulseContact.z *= friction * impulseContact.x;
    }
    return impulseContact;
}

void Contact::calculateInternals(double duration)
{
	if (bodies[0] == NULL) 
	{
		swapBodies();
	}
    
	assert(bodies[0]);

    calculateContactBasis();

    relativeContactPosition[0] = contactPoint - bodies[0]->getPosition();
    if (bodies[1]) 
	{
        relativeContactPosition[1] = contactPoint - bodies[1]->getPosition();
    }

    contactVelocity = calculateLocalVelocity(0, duration);
    if (bodies[1]) 
	{
        contactVelocity -= calculateLocalVelocity(1, duration);
    }

    calculateDesiredDeltaVelocity(duration);
}

void Contact::swapBodies()
{
	contactNormal *= -1;

    RigidBody *temp = bodies[0];
    bodies[0] = bodies[1];
    bodies[1] = temp;
}

void Contact::matchAwakeState()
{
	if (bodies[1] == NULL) 
	{
		return;
	}

    bool rigidBody1awake = bodies[0]->getAwake();
    bool rigidBody2awake = bodies[1]->getAwake();

    if (rigidBody1awake ^ rigidBody2awake)
	{
        if (rigidBody1awake) 
		{
			bodies[1]->setAwake();
		}
        else 
		{
			bodies[0]->setAwake();
		}
    }
}

void Contact::calculateContactBasis()
{
	Vector3 contactTangent[2];
	double scale;

    if (abs(contactNormal.x) > abs(contactNormal.y))
    {
        scale = (double)1.0f/sqrt(contactNormal.z*contactNormal.z + contactNormal.x*contactNormal.x);

        contactTangent[0].x = contactNormal.z*scale;
        contactTangent[0].y = 0;
        contactTangent[0].z = -contactNormal.x*scale;
        contactTangent[1].x = contactNormal.y*contactTangent[0].x;
        contactTangent[1].y = contactNormal.z*contactTangent[0].x - contactNormal.x*contactTangent[0].z;
        contactTangent[1].z = -contactNormal.y*contactTangent[0].x;
    }
    else
    {
        scale = (double)1.0/sqrt(contactNormal.z*contactNormal.z + contactNormal.y*contactNormal.y);

        contactTangent[0].x = 0;
        contactTangent[0].y = -contactNormal.z*scale;
        contactTangent[0].z = contactNormal.y*scale;
        contactTangent[1].x = contactNormal.y*contactTangent[0].z - contactNormal.z*contactTangent[0].y;
        contactTangent[1].y = -contactNormal.x*contactTangent[0].z;
        contactTangent[1].z = contactNormal.x*contactTangent[0].y;
    }

    contactToWorld.setColumns(contactNormal, contactTangent[0], contactTangent[1]);
}

void Contact::calculateDesiredDeltaVelocity(double duration)
{
	const static double velocityLimit = (double)0.25f;

    double velocityFromAcc = 0;

    if (bodies[0]->getAwake())
    {
		velocityFromAcc += (bodies[0]->getPreviousAcceleration() * duration).dotProduct(contactNormal);
    }

    if (bodies[1] && bodies[1]->getAwake())
    {
		velocityFromAcc -= (bodies[1]->getPreviousAcceleration() * duration).dotProduct(contactNormal);
    }

    double thisRestitution = restitution;
    if (abs(contactVelocity.x) < velocityLimit)
    {
        thisRestitution = (double)0.0f;
    }

    desiredDeltaVelocity = -contactVelocity.x -thisRestitution * (contactVelocity.x - velocityFromAcc);
}

void Contact::applyImpulse(const Vector3 &impulse, RigidBody *body, Vector3 *velocityChange, Vector3 *rotationChange)
{
	//Find definition for this
}

void Contact::applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
{
	Matrix3 inverseInertiaTensor[2];
    bodies[0]->setInertiaTensorWorld(inverseInertiaTensor[0]);
    if (bodies[1])
	{
        bodies[1]->setInertiaTensorWorld(inverseInertiaTensor[1]);
	}

	Vector3 impulseContact;

    if (friction == (double)0.0)
    {
        impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
    }
    else
    {
        impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
    }

    Vector3 impulse = contactToWorld.transform(impulseContact);

	Vector3 impulsiveTorque = relativeContactPosition[0].crossProduct(impulse);
    rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
    velocityChange[0].clear();
    velocityChange[0].addScaledVector(impulse, bodies[0]->getInverseMass());

    bodies[0]->addVelocity(velocityChange[0]);
    bodies[0]->addRotation(rotationChange[0]);

    if (bodies[1])
    {
		Vector3 impulsiveTorque = impulse.crossProduct(relativeContactPosition[1]);
        rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
        velocityChange[1].clear();
        velocityChange[1].addScaledVector(impulse, -bodies[1]->getInverseMass());

        bodies[1]->addVelocity(velocityChange[1]);
        bodies[1]->addRotation(rotationChange[1]);
    }
}

void Contact::applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], double penetration)
{
	const double angularLimit = (double)0.2f;
    double angularMove[2];
    double linearMove[2];

    double totalInertia = 0;
    double linearInertia[2];
    double angularInertia[2];

    for (unsigned i = 0; i < 2; i++) if (bodies[i])
    {
        Matrix3 inverseInertiaTensor;
        bodies[i]->setInertiaTensorWorld(inverseInertiaTensor);

        Vector3 angularInertiaWorld = relativeContactPosition[i].crossProduct(contactNormal);
        angularInertiaWorld = inverseInertiaTensor.transform(angularInertiaWorld);
		angularInertiaWorld = angularInertiaWorld.crossProduct(relativeContactPosition[i]);
		angularInertia[i] = angularInertiaWorld.dotProduct(contactNormal);

        linearInertia[i] = bodies[i]->getInverseMass();
        totalInertia += linearInertia[i] + angularInertia[i];
    }

    for (unsigned i = 0; i < 2; i++) if (bodies[i])
    {
        double sign = (i == 0)?1:-1;
        angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
        linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

        Vector3 projection = relativeContactPosition[i];
        projection.addScaledVector(contactNormal, -relativeContactPosition[i].scalarProduct(contactNormal));

		double maxMagnitude = angularLimit * projection.getMagnitude();

        if (angularMove[i] < -maxMagnitude)
        {
            double totalMove = angularMove[i] + linearMove[i];
            angularMove[i] = -maxMagnitude;
            linearMove[i] = totalMove - angularMove[i];
        }
        else if (angularMove[i] > maxMagnitude)
        {
            double totalMove = angularMove[i] + linearMove[i];
            angularMove[i] = maxMagnitude;
            linearMove[i] = totalMove - angularMove[i];
        }

        if (angularMove[i] == 0)
        {
            angularChange[i].clear();
        }
        else
        {
            Vector3 targetAngularDirection = relativeContactPosition[i].vectorProduct(contactNormal);

            Matrix3 inverseInertiaTensor;
            bodies[i]->setInertiaTensorWorld(inverseInertiaTensor);

            angularChange[i] = inverseInertiaTensor.transform(targetAngularDirection) *
                               (angularMove[i] / angularInertia[i]);
        }

        linearChange[i] = contactNormal * linearMove[i];

        Vector3 pos = bodies[i]->getPosition();
        pos.addScaledVector(contactNormal, linearMove[i]);
        bodies[i]->setPosition(pos);

        Quaternion q = bodies[i]->getOrientation();
        q.addScaledVector(angularChange[i], ((double)1.0));
        bodies[i]->setOrientation(q);

        if (!bodies[i]->getAwake()) 
		{
			bodies[i]->calculateDerivedData();
		}
    }
}

ContactResolver::ContactResolver(unsigned iterations, double velocityEpsilon, double positionEpsilon)
{
	setIterations(iterations, iterations);
    setEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned velocityIterations, unsigned positionIterations, double velocityEpsilon, double positionEpsilon)
{
	setIterations(velocityIterations);
    setEpsilon(velocityEpsilon, positionEpsilon);
}

bool ContactResolver::isValid()
{
	return (velocityIterations > 0) && (positionIterations > 0) &&
           (positionEpsilon >= 0.0f) && (positionEpsilon >= 0.0f);
}

void ContactResolver::setIterations(unsigned velocityIterations, unsigned positionIterations)
{
	velocityIterations = velocityIterations;
    positionIterations = positionIterations;
}

void ContactResolver::setIterations(unsigned iterations)
{
	setIterations(iterations, iterations);
}

void ContactResolver::setEpsilon(double velocityEpsilon, double positionEpsilon)
{
	velocityEpsilon = velocityEpsilon;
    positionEpsilon = positionEpsilon;
}

void ContactResolver::resolveContacts(Contact *contactArray, unsigned numContacts, double duration)
{
	if ((numContacts == 0) || !isValid())
	{
		return;
	}

    prepareContacts(contactArray, numContacts, duration);
    adjustPositions(contactArray, numContacts, duration);
    adjustVelocities(contactArray, numContacts, duration);
}

void ContactResolver::prepareContacts(Contact *contactArray, unsigned numContacts, double duration)
{
	Contact* lastContact = contactArray + numContacts;
    for (Contact* contact=contactArray; contact < lastContact; contact++)
    {
        contact->calculateInternals(duration);
    }
}

void ContactResolver::adjustVelocities(Contact *contactArray, unsigned numContacts, double duration)
{
	Vector3 velocityChange[2], rotationChange[2];
    Vector3 deltaVel;

    velocityIterationsUsed = 0;
    while (velocityIterationsUsed < velocityIterations)
    {
        double max = velocityEpsilon;
        unsigned index = numContacts;
        for (unsigned i = 0; i < numContacts; i++)
        {
            if (contactArray[i].desiredDeltaVelocity > max)
            {
                max = contactArray[i].desiredDeltaVelocity;
                index = i;
            }
        }
        if (index == numContacts) 
		{
			break;
		}

        contactArray[index].matchAwakeState();
        contactArray[index].applyVelocityChange(velocityChange, rotationChange);

        for (unsigned i = 0; i < numContacts; i++)
        {
            for (unsigned b = 0; b < 2; b++) if (contactArray[i].bodies[b])
            {
                for (unsigned d = 0; d < 2; d++)
                {
                    if (contactArray[i].bodies[b] == contactArray[index].bodies[d])
                    {
                        deltaVel = velocityChange[d] + rotationChange[d].vectorProduct(contactArray[i].relativeContactPosition[b]);

                        contactArray[i].contactVelocity += contactArray[i].contactToWorld.transformTranspose(deltaVel) * (b?-1:1);
                        contactArray[i].calculateDesiredDeltaVelocity(duration);
                    }
                }
            }
        }
        velocityIterationsUsed++;
    }
}

void ContactResolver::adjustPositions(Contact *contacts, unsigned numContacts, double duration)
{
	unsigned i,index;
    Vector3 linearChange[2], angularChange[2];
    double max;
    Vector3 deltaPosition;

    positionIterationsUsed = 0;
    while (positionIterationsUsed < positionIterations)
    {
        max = positionEpsilon;
        index = numContacts;
        for (i=0; i<numContacts; i++)
        {
            if (contacts[i].penetration > max)
            {
                max = contacts[i].penetration;
                index = i;
            }
        }
        if (index == numContacts) 
		{
			break;
		}

        contacts[index].matchAwakeState();
        contacts[index].applyPositionChange(linearChange, angularChange, max);

        for (i = 0; i < numContacts; i++)
        {
            for (unsigned b = 0; b < 2; b++) if (contacts[i].bodies[b])
            {
                for (unsigned d = 0; d < 2; d++)
                {
                    if (contacts[i].bodies[b] == contacts[index].bodies[d])
                    {
                        deltaPosition = linearChange[d] + angularChange[d].vectorProduct(contacts[i].relativeContactPosition[b]);
                        contacts[i].penetration += deltaPosition.scalarProduct(contacts[i].contactNormal) * (b?1:-1);
                    }
                }
            }
        }
        positionIterationsUsed++;
    }
}