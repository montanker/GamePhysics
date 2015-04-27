#include "Quaternion.h"

double Quaternion::magnetude()
{
	return sqrt(magnetudeSquared());
}

double Quaternion::magnetudeSquared()
{
	return r*r + i*i + j*j + k*k;
}

void Quaternion::normalize()
{
	double d = magnetudeSquared();

	if (d == 0)
	{
		r = 1;
		return;
	}

	d = ((double)1.0) / sqrt(d);
	r *= d;
	i *= d;
	j *= d;
	k *= d;
}

void Quaternion::rotateByVector(const Vector3& vector)
{
	Quaternion q(0, vector.x, vector.y, vector.z);
	(*this) *= q;
}

void Quaternion::addScaledVector(const Vector3& vector, double scale)
{
	Quaternion q(0,
		vector.x * scale,
		vector.y * scale,
		vector.z * scale);

	q *= *this;
	r += q.r * ((double)0.5);
	i += q.i * ((double)0.5);
	j += q.j * ((double)0.5);
	k += q.k * ((double)0.5);
}

void Quaternion::operator *=(const Quaternion &multiplier)
{
	Quaternion q = *this;
	r = q.r*multiplier.r - q.i*multiplier.i - q.j*multiplier.j - q.k*multiplier.k;
	i = q.r*multiplier.i + q.i*multiplier.r + q.j*multiplier.k - q.k*multiplier.j;
	j = q.r*multiplier.j + q.j*multiplier.r + q.k*multiplier.i - q.i*multiplier.k;
	k = q.r*multiplier.k + q.k*multiplier.r + q.i*multiplier.j - q.j*multiplier.i;
}