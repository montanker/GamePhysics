#include "Shape.h"

class Cube:public Shape
{
protected:
	void init();
	float mWidth;
	float mLength;
	float mHeight;

public:
	Cube(float width, float length, float height);
	~Cube() {};

	void update();
	void draw();
};