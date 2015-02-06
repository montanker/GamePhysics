#pragma once

#include <cmath>

class Vector3
{
public:
	Vector3(float num1=0, float num2=0, float num3=0);
	float distance(Vector3 other);
	void addScaledVector(Vector3 other, float scale);
	friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator*(const Vector3 &v1, const float &scalar);
	friend Vector3 operator==(const Vector3 &v1, const Vector3 &v2);

	float x;
	float y;
	float z;
};