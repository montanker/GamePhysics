#pragma once

#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class Vector3
{
public:
	Vector3(double num1=0, double num2=0, double num3=0);
	double distance(Vector3 other);
	void addScaledVector(Vector3 other, double scale);
	double getMagnitude();
	void normalize();
	string print();

	friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator*(const Vector3 &v1, const float &scalar);
	friend Vector3 operator==(const Vector3 &v1, const Vector3 &v2);

	double x;
	double y;
	double z;
};