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
	virtual void init() = 0;
	float mPosX;
	float mPosY;
	float mPosZ;

public:
	Shape() {};
	~Shape() {};

	virtual void update() = 0;
	virtual void draw() = 0;
	void setPos(float newX, float newY, float newZ);
	void move(float moveX, float moveY, float moveZ);
};