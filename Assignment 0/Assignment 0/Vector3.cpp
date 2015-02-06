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

void Vector3::addScaledVector(Vector3 other, float scale)
{
	x += other.x * scale;
	y += other.y * scale;
	z += other.z * scale;
}

Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

Vector3 operator*(const Vector3 &v1,  const float &scalar)
{
	return Vector3(v1.x*scalar, v1.y*scalar, v1.z*scalar);
}

Vector3 operator==(const Vector3 &v1, const Vector3 &v2)
{
	bool sameX = v1.x == v2.x;
	bool sameY = v1.y == v2.y;
	bool sameZ = v1.z == v2.z;

	return (sameX && sameY && sameZ);
}