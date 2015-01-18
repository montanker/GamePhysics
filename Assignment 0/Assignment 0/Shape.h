#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class Shape
{
protected:
	virtual void init();
	float mPosX;
	float mPosY;
	float mPosZ;

public:
	Shape() { init(); };
	virtual void update();
	virtual void draw();
	void move(float moveX, float moveY, float moveZ);
};