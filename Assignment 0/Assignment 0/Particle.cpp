#include "Particle.h"

void Particle::init()
{
	mAge = -1;
	mMass = 0;
	mDamping = 0;
	mPosition = Vector3(0,0,0);
	mVelocity = Vector3(0,0,0);
	mAcceleration = Vector3(0,0,0);
	mForceAccumulation = Vector3(0,0,0);
}

void Particle::integrate(float duration)
{
	if (getInverseMass() <= 0.0f)
	{
		return;
	}

	assert(duration > 0.0f);

	mPosition.addScaledVector(mVelocity, duration);

	Vector3 resultingAcc = mAcceleration;
	resultingAcc.addScaledVector(mForceAccumulation, getInverseMass());

	mVelocity.addScaledVector(resultingAcc, duration);

	//mVelocity =  mVelocity * powf(mDamping, duration);
	
	clearAccumulator();
}

void Particle::handleAge(float duration)
{
	if (mAge <= -1)
	{
		return;
	}

	mAge -= duration;
	if (mAge < 0)
	{
		mAge = 0;
	}
}

void Particle::clearAccumulator()
{
	mForceAccumulation = Vector3(0,0,0);
}

void Particle::update(float duration)
{
	integrate(duration);
	handleAge(duration);
}

void Particle::addForce(Vector3 addedForce)
{
	mForceAccumulation = mForceAccumulation + addedForce;
}

float Particle::getInverseMass()
{
	if (mMass <= 0)
	{
		return 0;
	}

	return (1.0f/mMass);
}

bool Particle::isDead()
{
	if (mAge <= -1)
	{
		return true;
	}

	return (mAge <= 0);
}