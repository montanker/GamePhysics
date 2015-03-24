#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver()
{
	mIterations = 0;
	mIterationsUsed = 0;
}

ParticleContactResolver::ParticleContactResolver(unsigned iterations)
{
	mIterations = iterations;
	mIterationsUsed = 0;
}

void ParticleContactResolver::setIterations(unsigned iterations)
{
	mIterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact *contactArray, unsigned numContacts, float duration)
{
	unsigned i;
	mIterationsUsed = 0;

	while(mIterationsUsed < mIterations)
	{
		double max = numeric_limits<double>::max();
		unsigned maxIndex = numContacts;

		for (i=0; i<numContacts; i++)
		{
			double sepVel = contactArray[i].calculateSeparatingVelocity();
			if ((sepVel < max) && ((sepVel < 0) || (contactArray[i].mPenetration > 0)))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		if (maxIndex == numContacts)
		{
			break;
		}

		contactArray[maxIndex].resolve(duration);
		mIterationsUsed++;
	}
}