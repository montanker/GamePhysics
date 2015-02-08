#include "Particle.h"
#include "Color.h"

class SphereParticle:public Particle
{
protected:
	float mRadius;
	Color mColor;
	int slices;
	int stacks;

public:
	SphereParticle(float radius, float mass, Color color);

	void draw();
};