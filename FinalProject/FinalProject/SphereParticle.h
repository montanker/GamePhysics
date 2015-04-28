#include "Particle.h"


class SphereParticle:public Particle
{
protected:
	float mRadius;
	Color mColor;
	int slices;
	int stacks;

public:
	SphereParticle(float radius, double mass, Color color);

	void draw();
	float getRadius() {return mRadius;};
	void setColor(Color c) { mColor = c; }
};