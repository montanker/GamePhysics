#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>

class Quaternion
{
public:
	Quaternion(){ r = 1; i = 0; j = 0; k = 0; };
	Quaternion(double w, Vector3 vector);
	Quaternion(double w, double x, double y, double z);

	union
	{
		struct{
			double r, i, j, k;
		};

		double data[4];
	};

	double magnetude();
	double magnetudeSquared();
	void normalize();
	void rotateByVector(const Vector3& vector);
	void addScaledVector(const Vector3& vector, double scale);

	void operator *=(const Quaternion &multiplier) ;

};
#endif