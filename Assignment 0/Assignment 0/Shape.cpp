#include "Shape.h"

void Shape::init()
{
	mPos = Vector3(0,0,0);
	mRotations = vector<Vector3>();
	mRotationAngles = vector<float>();
}

void Shape::setPos(float newX, float newY, float newZ)
{
	mPos = Vector3(newX, newY, newZ);
}

void Shape::setPos(Vector3 newPos)
{
	setPos(newPos.x, newPos.y, newPos.z);
}

void Shape::setRotation(float angle, float rotateX, float rotateY, float rotateZ)
{
	setRotation(angle, Vector3(rotateX, rotateY, rotateZ));
}

void Shape::setRotation(float angle, Vector3 rotateVector)
{
	mRotations.clear();
	mRotationAngles.clear();

	rotate(angle, rotateVector);
}

void Shape::move(float newX, float newY, float newZ)
{
	mPos.x += newX;
	mPos.y += newY;
	mPos.z += newZ;
}

void Shape::move(Vector3 moveVector)
{
	move(moveVector.x, moveVector.y, moveVector.z);
}

void Shape::applyRotations()
{
	float angle;
	Vector3 rot;

	glTranslatef(mPos.x, mPos.y, mPos.z);

	for (int i=mRotationAngles.size(); i>0; i--)
	{
		angle = mRotationAngles[i-1];
		rot = mRotations[i-1];
		glRotatef(angle, rot.x, rot.y, rot.z);
	}

	glTranslatef(-mPos.x, -mPos.y, -mPos.z);
	
}

void Shape::rotate(float angle, float rotateX, float rotateY, float rotateZ)
{
	rotate(angle, Vector3(rotateX, rotateY, rotateZ));
}

void Shape::rotate(float angle, Vector3 rotateVector)
{
	mRotationAngles.push_back(angle);
	mRotations.push_back(rotateVector);
}