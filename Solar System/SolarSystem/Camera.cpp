#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip, float sensitivity, float speed)
{
	mFOV = fov;
	mRatio = aspectRatio;
	mNear = nearClip;
	mFar = farClip;

	mCamPos = Vector3(0,0,0);
	mTarget = Vector3(0,0,0);
	mCamRotX = mCamRotY = mCamRotZ = 0;
	mSpeedX = mSpeedY = mSpeedZ = 0;

	//mMoveSpeed = 0.1f;
	mMoveSpeed = speed;

	//mTarget = Target();
	mUseTarget = false;
	oldCamPos = mCamPos;
	oldCamRotX = mCamRotX;
	oldCamRotY = mCamRotY;

	mForward = mBack = mLeft = mRight = false;

	mSensitivity = sensitivity;
}

float Camera::degToRad(float angle)
{
	return angle * ((float)M_PI/180.0f);
}

void Camera::handleMouseMove(GLFWwindow* window, float screenWidth, float screenHeight)
{
	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	double horizontalMovement = mouseX - screenWidth/2;
	double verticalMovement = mouseY - screenHeight/2;

	mCamRotX += (float)verticalMovement/mSensitivity;
	mCamRotY += (float)horizontalMovement/mSensitivity;

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
	}
	if (mCamRotY > 180.0f)
	{
		mCamRotY -= 360.0f;
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
	mCamPos = Vector3(newX, newY, newZ);
}

void Camera::setPos(Vector3 newPos)
{
	setPos(newPos.x, newPos.y, newPos.z);
}

void Camera::setRot(float newRotX, float newRotY, float newRotZ)
{
	mCamRotX = newRotX;
	mCamRotY = newRotY;
	mCamRotZ = newRotZ;
}

void Camera::setRot(Vector3 newRot)
{
	setRot(newRot.x, newRot.y, newRot.z);
}

void Camera::savePos()
{
	oldCamPos = mCamPos;
	oldCamRotX = mCamRotX;
	oldCamRotY = mCamRotY;
}

void Camera::loadPos()
{
	mCamPos = oldCamPos;
	mCamRotX = oldCamRotX;
	mCamRotY = oldCamRotY;
}

void Camera::press(char key, int isPressed)
{
	bool pressed = isPressed != 0;

	switch (key)
	{
	case 'w': 
		mForward = pressed;
		break;
	case 's': 
		mBack = pressed;
		break;
	case 'a': 
		mLeft = pressed;
		break;
	case 'd': 
		mRight = pressed;
		break;
	}
}

void Camera::focus(Vector3 pos, Vector3 target)
{
	mCamPos = Vector3(pos.x, pos.y, pos.z);
	mTarget = target;
	mUseTarget = true;
}

void Camera::move()
{
	mCamPos.x += mSpeedX;
	mCamPos.y += mSpeedY;
	mCamPos.z += mSpeedZ;
}

void Camera::update(GLFWwindow* window, float screenWidth, float screenHeight)
{
	if (!mUseTarget)
	{
		handleMouseMove(window, screenWidth, screenHeight);
		handleMovement();
		move();
	}
}

void Camera::draw()
{
	glLoadIdentity();
	gluPerspective(mFOV, mRatio, mNear, mFar);
	if (!mUseTarget)
	{
		glRotatef(mCamRotX, 1.0f, 0.0f, 0.0f);
		glRotatef(mCamRotY, 0.0f, 1.0f, 0.0f);
		glRotatef(mCamRotZ, 0.0f, 0.0f, 1.0f);
		glTranslatef(-mCamPos.x, -mCamPos.y, -mCamPos.z);
	}
	else
	{
		gluLookAt(mCamPos.x, mCamPos.y, mCamPos.z,
			      mTarget.x, mTarget.y, mTarget.z,
				  0.0f, 1.0f, 0.0f);
		mUseTarget = false;
	}
}