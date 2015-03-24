#define NOMINMAX

#include <limits>
#include "ParticleContact.h"

using namespace std;

class ParticleContactResolver
{
protected:
	unsigned mIterations;
	unsigned mIterationsUsed;

public:
	ParticleContactResolver();
	ParticleContactResolver(unsigned iterations);
	void setIterations(unsigned iterations);
	void resolveContacts(ParticleContact *contactArray, unsigned numContacts, float duration);
};