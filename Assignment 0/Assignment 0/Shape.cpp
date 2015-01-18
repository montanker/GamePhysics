#include "Shape.h";

void Shape::setPos(float newX, float newY, float newZ)
{
	mPosX = newX;
	mPosY = newY;
	mPosZ = newZ;
}

void Shape::move(float newX, float newY, float newZ)
{
	mPosX += newX;
	mPosY += newY;
	mPosZ += newZ;
}