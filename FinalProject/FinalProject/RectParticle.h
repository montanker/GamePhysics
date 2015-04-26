#include "Particle.h"

class RectParticle:public Particle
{
protected:
	float mWidth;
	float mLength;
	float mHeight;
	Color mColor;

public:
	RectParticle(float width, float length, float height, double mass, Color color);

	void draw();
	float getWidth() {return mWidth;};
	float getLength() {return mLength;};
	float getHeight() {return mHeight;};
};