#include "Particle.h"

void Particle::init()
{
	mAge = -1;
	mMass = 0;
	mDamping = 0;
	mPosScale = 1;
	mSizeScale = 1;
	mPosition = Vector3(0,0,0);
	mVelocity = Vector3(0,0,0);
	mAcceleration = Vector3(0,0,0);
	mForceAccumulation = Vector3(0,0,0);

	mLimitVelocity = false;
	mMaxVelocity = Vector3(99, 99, 99);
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

	if (mLimitVelocity)
	{
		if (mMaxVelocity.x < 0)
		{
			if (mVelocity.x < mMaxVelocity.x)
			{
				mVelocity.x = mMaxVelocity.x;
			}
		}
		else if (mMaxVelocity.x > 0)
		{
			if (mVelocity.x > mMaxVelocity.x)
			{
				mVelocity.x = mMaxVelocity.x;
			}
		}

		if (mMaxVelocity.z < 0)
		{
			if (mVelocity.z < mMaxVelocity.z)
			{
				mVelocity.z = mMaxVelocity.z;
			}
		}
		else if (mMaxVelocity.z > 0)
		{
			if (mVelocity.z > mMaxVelocity.z)
			{
				mVelocity.z = mMaxVelocity.z;
			}
		}
	}
	
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

void Particle::limitVelocity(bool shouldLimit)
{
	mLimitVelocity = shouldLimit;
}

void Particle::limitVelocity(bool shouldLimit, Vector3 limit)
{
	mLimitVelocity = shouldLimit;
	mMaxVelocity = limit;
}

double Particle::getInverseMass()
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

string Particle::printMass()
{
	stringstream ss;
	ss << mMass << " kg";

	return ss.str();
}