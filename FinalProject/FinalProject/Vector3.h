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
	double dotProduct(Vector3 other) const;
	Vector3 crossProduct(Vector3 other);
	double squareMagnitude() const;
	void normalize();
	string print();

	friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
	Vector3& operator+=(const Vector3 &v2);
	friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator*(const Vector3 &v1, const float &scalar);
	Vector3& operator*=(const float &scalar);
	friend Vector3 operator==(const Vector3 &v1, const Vector3 &v2);
	double& operator[](int index);
	double operator[](int index) const;
	//friend Vector3 operator%(Vector3 &v1, Vector3 &v2);

	double x;
	double y;
	double z;
};