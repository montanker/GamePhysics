#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip, float sensitivity, float speed)
{
	mFOV = fov;
	mRatio = aspectRatio;
	mNear = nearClip;
	mFar = farClip;

	mCamX = mCamY = mCamZ = 0;
	mCamRotX = mCamRotY = mCamRotZ = 0;
	mSpeedX = mSpeedY = mSpeedZ = 0;

	mMoveSpeed = 0.1f;

	mForward = mBack = mLeft = mRight = false;

	mSensitivity = sensitivity;
}

float Camera::degToRad(float angle)
{
	return angle * (M_PI/180.0f);
}

void Camera::handleMouseMove(GLFWwindow* window, float screenWidth, float screenHeight)
{
	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	int horizontalMovement = mouseX - screenWidth/2;
	int verticalMovement = mouseY - screenHeight/2;

	mCamRotX += verticalMovement/mSensitivity;
	mCamRotY += horizontalMovement/mSensitivity;

	if (mCamRotX < -90.0f)
	{
		mCamRotX = -90.0f;
	}
	if (mCamRotX > 90.0f)
	{
		mCamRotX = 90.0f;
	}

	if (mCamRotY < -180.0f)
	{
		mCamRotY += 360.0f;
		//mCamRotY = 180.0f;
	}
	if (mCamRotY > 180.0f)
	{
		mCamRotY -= 360.0f;
		//mCamRotY = -180.0f;
	}

	glfwSetCursorPos(window, screenWidth/2, screenHeight/2);
}

void Camera::handleMovement()
{
	float camMoveX = 0.0f;
	float camMoveY = 0.0f;
	float camMoveZ = 0.0f;
	float pitch;
	float yaw;
	float yRotRad;

	if (mForward)
	{
		pitch = cos(degToRad(mCamRotX));
		yaw = float(cos(degToRad(mCamRotX)));

		camMoveX += (mMoveSpeed * float(sin(degToRad(mCamRotY)))) * pitch;
		camMoveY += mMoveSpeed * float(sin(degToRad(mCamRotX))) * -1.0f;
		camMoveZ += (mMoveSpeed * float(cos(degToRad(mCamRotY))) * -1.0f) * yaw;
	}
	if (mBack)
	{
		pitch = cos(degToRad(mCamRotX));
		yaw = float(cos(degToRad(mCamRotX)));

		camMoveX += (mMoveSpeed * float(sin(degToRad(mCamRotY))) * -1.0f) * pitch;
        camMoveY += mMoveSpeed * float(sin(degToRad(mCamRotX)));
		camMoveZ += (mMoveSpeed * float(cos(degToRad(mCamRotY)))) * yaw;
	}
	if (mLeft)
	{
		yRotRad = degToRad(mCamRotY);
		camMoveX += -mMoveSpeed * float(cos(yRotRad));
		camMoveZ += -mMoveSpeed * float(sin(yRotRad));
	}
	if (mRight)
	{
		yRotRad = degToRad(mCamRotY);
		camMoveX += mMoveSpeed * float(cos(yRotRad));
		camMoveZ += mMoveSpeed * float(sin(yRotRad));
	}

	mSpeedX = camMoveX;
	mSpeedY = camMoveY;
	mSpeedZ = camMoveZ;

	if (mSpeedX > mMoveSpeed)
	{
		mSpeedX = mMoveSpeed;
	}
	else if (mSpeedX < -mMoveSpeed)
	{
		mSpeedX = -mMoveSpeed;
	}

	if (mSpeedY > mMoveSpeed)
	{
		mSpeedY = mMoveSpeed;
	}
	else if (mSpeedY < -mMoveSpeed)
	{
		mSpeedY = -mMoveSpeed;
	}

	if (mSpeedZ > mMoveSpeed)
	{
		mSpeedZ = mMoveSpeed;
	}
	else if (mSpeedZ < -mMoveSpeed)
	{
		mSpeedZ = -mMoveSpeed;
	}
}

void Camera::setPos(float newX, float newY, float newZ)
{
	mCamX = newX;
	mCamY = newY;
	mCamZ = newZ;
}

void Camera::press(char key, int isPressed)
{
	switch (key)
	{
	case 'w': 
		mForward = isPressed;
		break;
	case 's': 
		mBack = isPressed;
		break;
	case 'a': 
		mLeft = isPressed;
		break;
	case 'd': 
		mRight = isPressed;
		break;
	}
}

void Camera::move()
{
	mCamX += mSpeedX;
	mCamY += mSpeedY;
	mCamZ += mSpeedZ;
}

void Camera::update(GLFWwindow* window, float screenWidth, float screenHeight)
{
	handleMouseMove(window, screenWidth, screenHeight);
	handleMovement();
	move();
}

void Camera::draw()
{
	glLoadIdentity();
	gluPerspective(mFOV, mRatio, mNear, mFar);
	glRotatef(mCamRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(mCamRotY, 0.0f, 1.0f, 0.0f);
	glTranslatef(-mCamX, -mCamY, -mCamZ);
}