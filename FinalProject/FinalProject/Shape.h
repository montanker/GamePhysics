#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <GLFW/glfw3.h>
#include "Vector3.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class Shape
{
protected:
	void init();
	void applyRotations();
	Vector3 mPos;
	vector<Vector3> mRotations;
	vector<float> mRotationAngles;

public:
	Shape() {};
	~Shape() {};

	virtual void update() = 0;
	virtual void draw() = 0;
	void setPos(float newX, float newY, float newZ);
	void setPos(Vector3 newPos);
	void setRotation(float angle, float rotateX, float rotateY, float rotateZ);
	void setRotation(float angle, Vector3 rotateVector);
	void move(float moveX, float moveY, float moveZ);
	void move(Vector3 moveVector);
	void rotate(float angle, float rotateX, float rotateY, float rotateZ);
	void rotate(float angle, Vector3 rotateVector);
};