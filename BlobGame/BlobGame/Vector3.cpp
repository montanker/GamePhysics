#include "Vector3.h"

Vector3::Vector3(double num1, double num2, double num3)
{
	x = num1;
	y = num2;
	z = num3;
}

double Vector3::distance(Vector3 other)
{
	double dist = sqrt(pow(other.x-x,2.0f)+pow(other.y-y,2.0f)+pow(other.z-z,2.0f));

	return dist;
}

void Vector3::addScaledVector(Vector3 other, double scale)
{
	x += other.x * scale;
	y += other.y * scale;
	z += other.z * scale;
}

double Vector3::getMagnitude()
{
	double magnitude = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
	magnitude = abs(magnitude);

	return magnitude;
}

double Vector3::dotProduct(Vector3 other)
{
	double dot = (x*other.x) + (y*other.y) + (z*other.z);

	return dot;
}

void Vector3::normalize()
{
	double mag = getMagnitude();

	x = x/mag;
	y = y/mag;
	z = z/mag;
}

string Vector3::print()
{
	stringstream ss;
	ss << "[" << x << ", " << y << ", " << z << "]";

	return ss.str();
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