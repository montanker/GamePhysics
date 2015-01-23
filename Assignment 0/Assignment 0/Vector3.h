#pragma once

#include <cmath>

class Vector3
{
public:
	Vector3(float num1=0, float num2=0, float num3=0);
	float distance(Vector3 other);

	float x;
	float y;
	float z;
};