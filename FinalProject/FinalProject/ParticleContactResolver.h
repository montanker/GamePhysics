#define NOMINMAX

#include <limits>
#include "ParticleContact.h"
#include <vector>

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
	//void resolveContacts(ParticleContact *contactArray, unsigned numContacts, float duration);
	void resolveContacts(vector<ParticleContact*> contactArray, unsigned numContacts, float duration);
};