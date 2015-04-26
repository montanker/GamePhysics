#pragma once

class Quaternion
{
public:
	Quaternion(double axisAngle=0, double axisX=0, double axisY=0, double axisZ=0);
	double getMagnitude();
	Quaternion getInverse();
	void normalize();

	friend Quaternion operator*(const Quaternion &q1, const Quaternion &q2);

	double angle;
	double x;
	double y;
	double z;
};