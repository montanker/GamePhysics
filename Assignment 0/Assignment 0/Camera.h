//Thanks to:
//http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/

#define _USE_MATH_DEFINES 
#pragma once

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Vector3.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class Camera
{
private:
	float degToRad(float angle);
	void handleMouseMove(GLFWwindow* window, float screenWidth, float screenHeight);
	void handleMovement();

	Vector3 mCamPos;
	float mCamRotX, mCamRotY;
	float mSpeedX, mSpeedY, mSpeedZ;
	float mMoveSpeed;
	float mSensitivity;

	float mFOV;
	float mRatio;
	float mNear;
	float mFar;

	bool mForward, mBack, mLeft, mRight;
public:
	Camera(float fov, float aspectRatio, float nearClip, float farClip, float sensitivity, float speed);
	~Camera() {};

	void setPos(float newX, float newY, float newZ);
	void setPos(Vector3 newPos);
	void press(char key, int isPressed);
	void move();
	void update(GLFWwindow* window, float screenWidth, float screenHeight);
	void draw();
};