#include "Vector3.h"
#include "Quaternion.h"


class Matrix3
{
public:
	Matrix3(double val1=0, double val2=0, double val3=0,
		    double val4=0, double val5=0, double val6=0,
			double val7=0, double val8=0, double val9=0);
	Matrix3(const Matrix3 &other);
	void setInverse(const Matrix3 &m);
	void setTranspose(const Matrix3 &m);
	void setOrientation(const Quaternion &q);
	Matrix3 invert();
	Matrix3 inverse() const;
	Matrix3 transpose() const;
	Vector3 transform(const Vector3 &vector) const;

	friend Vector3 operator*(const Matrix3 &m1, const Vector3 &v1);
	friend Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2);
	friend Matrix3 operator*=(Matrix3 &m1, const Matrix3 &m2);

	double values[9];
};