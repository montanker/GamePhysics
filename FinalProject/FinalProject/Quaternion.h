#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "Vector3.h"

class Quaternion
{
public:
	Quaternion(){ r = 1; i = 0; j = 0; k = 0; };
	Quaternion(double w, Vector3 vector) : r(w), i(vector.x), j(vector.y), k(vector.z) { }  // FIXME??
	Quaternion(double w, double x, double y, double z) : r(w), i(x), j(y), k(z) { }  // FIXME??

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