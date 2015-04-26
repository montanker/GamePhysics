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

/*struct Target
{
	Vector3 position;
	Vector3 target;
};*/

class Camera
{
private:
	float degToRad(float angle);
	void handleMouseMove(GLFWwindow* window, float screenWidth, float screenHeight);
	void handleMovement();

	Vector3 mCamPos;
	float mCamRotX, mCamRotY, mCamRotZ;
	float mSpeedX, mSpeedY, mSpeedZ;
	float mMoveSpeed;
	float mSensitivity;

	float mFOV;
	float mRatio;
	float mNear;
	float mFar;

	//Target mTarget;
	bool mUseTarget;
	Vector3 mTarget;
	Vector3 oldCamPos;
	float oldCamRotX, oldCamRotY;

	bool mForward, mBack, mLeft, mRight;
public:
	Camera(float fov, float aspectRatio, float nearClip, float farClip, float sensitivity, float speed);
	~Camera() {};

	void setPos(float newX, float newY, float newZ);
	void setPos(Vector3 newPos);
	void setRot(float newRotX, float newRotY, float newRotZ);
	void setRot(Vector3 newRot);
	Vector3 getPos() {return mCamPos;};
	void savePos();
	void loadPos();
	void press(char key, int isPressed);
	void focus(Vector3 pos, Vector3 target);
	//void loseFocus();
	void move();
	void update(GLFWwindow* window, float screenWidth, float screenHeight);
	void draw();
};