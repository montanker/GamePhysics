#include "RigidBody.h"

static inline void _calculateTransformMatrix(
	Matrix4 &transformMatrix,
	const Vector3 &position,
	const Quaternion &orientation)
{
	transformMatrix.values[0] = 1 - 2 * orientation.j*orientation.j - 2 * orientation.k*orientation.k;
	transformMatrix.values[1] = 2*orientation.i*orientation.j - 2*orientation.r*orientation.k;
	transformMatrix.values[2] = 2*orientation.i*orientation.j + 2*orientation.r*2*orientation.j;
	transformMatrix.values[3] = position.x;

	transformMatrix.values[4] = 2*orientation.i*orientation.j + 2*orientation.r*orientation.k;
	transformMatrix.values[5] = 1 - 2*orientation.i*orientation.i - 2*orientation.k*orientation.k;
	transformMatrix.values[6] = 2*orientation.j*orientation.k - 2*orientation.r*orientation.i;
	transformMatrix.values[7] = position.y;

	transformMatrix.values[8] = 2*orientation.i*orientation.k - 2*orientation.r*orientation.j;
	transformMatrix.values[9] = 2*orientation.j*orientation.k + 2*orientation.r*orientation.i;
	transformMatrix.values[10] = 1 - 2*orientation.i*orientation.i - 2*orientation.j*orientation.j;
	transformMatrix.values[11] = position.z;
}

static inline void _transformInertiaTensor(
	Matrix3 &iitWorld,
	const Quaternion &q,
	const Matrix3 &iitBody,
	const Matrix4 &rotmat)
{
	double t4 = rotmat.values[0] * iitBody.values[0] +
				rotmat.values[1] * iitBody.values[3] +
				rotmat.values[2] * iitBody.values[6];
		
	double t9 = rotmat.values[0] * iitBody.values[1] +
				rotmat.values[1] * iitBody.values[4] +
				rotmat.values[2] * iitBody.values[7];

	double t14 = rotmat.values[0] * iitBody.values[2] +
				rotmat.values[1] * iitBody.values[5] +
				rotmat.values[2] * iitBody.values[8];

	double t28 = rotmat.values[4] * iitBody.values[0] +
				rotmat.values[5] * iitBody.values[3] +
				rotmat.values[6] * iitBody.values[6];

	double t33 = rotmat.values[4] * iitBody.values[1] +
				rotmat.values[5] * iitBody.values[4] +
				rotmat.values[6] * iitBody.values[7];

	double t38 = rotmat.values[4] * iitBody.values[2] +
				rotmat.values[5] * iitBody.values[5] +
				rotmat.values[6] * iitBody.values[8];

	double t52 = rotmat.values[8] * iitBody.values[0] +
				rotmat.values[9] * iitBody.values[3] +
				rotmat.values[10] * iitBody.values[6];

	double t57 = rotmat.values[8] * iitBody.values[1] +
				rotmat.values[9] * iitBody.values[4] +
				rotmat.values[10] * iitBody.values[7];

	double t62 = rotmat.values[8] * iitBody.values[2] +
				rotmat.values[9] * iitBody.values[5] +
				rotmat.values[10] * iitBody.values[8];


	iitWorld.values[0] = t4*rotmat.values[0] + t9*rotmat.values[1] + t14*rotmat.values[2];
	iitWorld.values[1] = t4*rotmat.values[4] + t9*rotmat.values[5] + t14*rotmat.values[6];
	iitWorld.values[2] = t4*rotmat.values[8] + t9*rotmat.values[9] + t14*rotmat.values[10];
	
	iitWorld.values[3] = t28*rotmat.values[0] + t33*rotmat.values[1] + t38*rotmat.values[2];
	iitWorld.values[4] = t28*rotmat.values[4] + t33*rotmat.values[5] + t38*rotmat.values[6];
	iitWorld.values[5] = t28*rotmat.values[8] + t33*rotmat.values[9] + t38*rotmat.values[10];

	iitWorld.values[6] = t52*rotmat.values[0] + t57*rotmat.values[1] + t62*rotmat.values[2];
	iitWorld.values[7] = t52*rotmat.values[4] + t57*rotmat.values[5] + t62*rotmat.values[6];
	iitWorld.values[8] = t52*rotmat.values[8] + t57*rotmat.values[9] + t62*rotmat.values[10];
}

void RigidBody::calculateDerivedData()
{
	orientation.normalize();
	_calculateTransformMatrix(transformMatrix, position, orientation);
	_transformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
}

void RigidBody::setInertiaTensor(const Matrix3 &inertiaTensor)
{
	inverseInertiaTensor.setInverse(inertiaTensor);
}

void RigidBody::addForce(const Vector3 &force)
{
	forceAccum += force;
	isAwake = true;
}
void RigidBody::addForceAtBodyPoint(const Vector3 &force, const Vector3 &point)
{
	Vector3 pt = getPointInWorldSpace(point);
	addForceAtPoint(force, pt);
	isAwake = true;
}

void RigidBody::addForceAtPoint(const Vector3 &force, const Vector3 &point)
{
	Vector3 pt = point;
	pt -= position;
	forceAccum += force;
	torqueAccum += pt.crossProduct(force);
	isAwake = true;
}

void RigidBody::clearAccumulators()
{
	forceAccum.clear();
	torqueAccum.clear();
}

void RigidBody::integrate(double duration)
{
	lastFrameAcceleration = acceleration;
	lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

	Vector3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);

	velocity.addScaledVector(lastFrameAcceleration, duration);
	rotation.addScaledVector(angularAcceleration, duration);

	velocity *= pow(linearDampening, duration);
	rotation *= pow(angularDampening, duration);

	position.addScaledVector(velocity, duration);
	orientation.addScaledVector(rotation, duration);

	calculateDerivedData();

	clearAccumulators();
}


double RigidBody::getMass()
{
	if (inverseMass == 0) {
		return FLT_MAX;
	}
	else {
		return ((double)(1.0)) / inverseMass;
	}
}

double RigidBody::getInverseMass()
{
	return inverseMass;
}

void RigidBody::setMass(double mass)
{
	assert(mass != 0);
	inverseMass = ((double)(1.0)) / mass;
}

bool RigidBody::hasFiniteMass()
{
	return inverseMass >= 0.0f;
}

Vector3 RigidBody::getPointInWorldSpace(Vector3 point)
{
	return transformMatrix.transform(point);
}