#include "Shape.h";

void Shape::move(float newX, float newY, float newZ)
{
	mPosX += newX;
	mPosY += newY;
	mPosZ += newZ;
}