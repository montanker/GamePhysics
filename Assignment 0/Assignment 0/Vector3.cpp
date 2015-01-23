#include "Vector3.h"

Vector3::Vector3(float num1, float num2, float num3)
{
	x = num1;
	y = num2;
	z = num3;
}

float Vector3::distance(Vector3 other)
{
	float dist = sqrt(powf(other.x-x,2.0f)+powf(other.y-y,2.0f)+powf(other.z-z,2.0f));

	return dist;
}