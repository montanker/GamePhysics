#include "Vector3.h"
#include "Quaternion.h"

class Matrix4
{
public:
	Matrix4(double val1=0, double val2=0, double val3=0, double val4=0,
		    double val5=0, double val6=0, double val7=0, double val8=0,
			double val9=0, double val10=0, double val11=0, double val12=0);
	Matrix4(const Matrix4 &other);
	double getDeterminant() const;
	void setInverse(const Matrix4 &m);
	void invert();
	void setOrientationAndPos(const Quaternion &q, const Vector3 &pos);
	Matrix4 inverse() const;
	Vector3 getAxisVector(int i) const;
	Vector3 transform(const Vector3 &vector) const;
	Vector3 transformInverse(const Vector3 &vector) const;
	Vector3 transformDirection(const Vector3 &vector) const;
	Vector3 transformInverseDirection(const Vector3 &vector) const;
	Vector3 localToWorld(const Vector3 &local);
	Vector3 worldToLocal(const Vector3 &world);
	Vector3 localToWorldDirn(const Vector3 &local);
	Vector3 worldToLocalDirn(const Vector3 &world);

	friend Vector3 operator*(const Matrix4 &m, const Vector3 &v);
	friend Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2);

	double values[16];
};