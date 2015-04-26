#include "Matrix4.h"

Matrix4::Matrix4(double val1, double val2, double val3, double val4,
		         double val5, double val6, double val7, double val8,
		         double val9, double val10, double val11, double val12)
{
	values[0] = val1;
	values[1] = val2;
	values[2] = val3;
	values[3] = val4;
	values[4] = val5;
	values[5] = val6;
	values[6] = val7;
	values[7] = val8;
	values[8] = val9;
	values[9] = val10;
	values[10] = val11;
	values[11] = val12;
}

Matrix4::Matrix4(const Matrix4 &other)
{
	values[0] = other.values[0];
	values[1] = other.values[1];
	values[2] = other.values[2];
	values[3] = other.values[3];
	values[4] = other.values[4];
	values[5] = other.values[5];
	values[6] = other.values[6];
	values[7] = other.values[7];
	values[8] = other.values[8];
	values[9] = other.values[9];
	values[10] = other.values[10];
	values[11] = other.values[11];
}

double Matrix4::getDeterminant() const
{
	return values[8]*values[5]*values[2]+
		   values[4]*values[9]*values[2]+
		   values[8]*values[1]*values[6]-
		   values[0]*values[9]*values[6]-
		   values[4]*values[1]*values[10]+
		   values[0]*values[5]*values[10];
}

void Matrix4::setInverse(const Matrix4 &m)
{
	double det = getDeterminant();
	if (det == 0)
	{
		return;
	}
	det = ((double)1.0f)/det;

	values[0] = (-m.values[9]*m.values[6]+m.values[5]*m.values[10])*det;
    values[4] = (m.values[8]*m.values[6]-m.values[4]*m.values[10])*det;
    values[8] = (-m.values[8]*m.values[5]+m.values[4]*m.values[9])*det;

    values[1] = (m.values[9]*m.values[2]-m.values[1]*m.values[10])*det;
    values[5] = (-m.values[8]*m.values[2]+m.values[0]*m.values[10])*det;
    values[9] = (m.values[8]*m.values[1]-m.values[0]*m.values[9])*det;

    values[2] = (-m.values[5]*m.values[2]+m.values[1]*m.values[6])*det;
    values[6] = (m.values[4]*m.values[2]-m.values[0]*m.values[6])*det;
    values[10] = (-m.values[4]*m.values[1]+m.values[0]*m.values[5])*det;

    values[3] = (m.values[9]*m.values[6]*m.values[3]
               -m.values[5]*m.values[10]*m.values[3]
               -m.values[9]*m.values[2]*m.values[7]
               +m.values[1]*m.values[10]*m.values[7]
               +m.values[5]*m.values[2]*m.values[11]
               -m.values[1]*m.values[6]*m.values[11])*det;
    values[7] = (-m.values[8]*m.values[6]*m.values[3]
               +m.values[4]*m.values[10]*m.values[3]
               +m.values[8]*m.values[2]*m.values[7]
               -m.values[0]*m.values[10]*m.values[7]
               -m.values[4]*m.values[2]*m.values[11]
               +m.values[0]*m.values[6]*m.values[11])*det;
    values[11] =(m.values[8]*m.values[5]*m.values[3]
               -m.values[4]*m.values[9]*m.values[3]
               -m.values[8]*m.values[1]*m.values[7]
               +m.values[0]*m.values[9]*m.values[7]
               +m.values[4]*m.values[1]*m.values[11]
               -m.values[0]*m.values[5]*m.values[11])*det;
}

void Matrix4::invert()
{
	setInverse(*this);
}

void Matrix4::setOrientationAndPos(const Quaternion &q, const Vector3 &pos)
{
	//Needs quaternion
}

Matrix4 Matrix4::inverse() const
{
	Matrix4 result;
	result.setInverse(*this);
	return result;
}

Vector3 Matrix4::transform(const Vector3 &vector) const
{
	return (*this) * vector;
}

Vector3 Matrix4::transformInverse(const Vector3 &vector) const
{
	Vector3 temp = vector;
	temp.x -= values[3];
	temp.y -= values[7];
	temp.z -= values[11];

	return Vector3(temp.x*values[0]+temp.y*values[4]+temp.z*values[8],
		           temp.x*values[1]+temp.y*values[5]+temp.z*values[9],
				   temp.x*values[2]+temp.y*values[6]+temp.z*values[10]);
}

Vector3 Matrix4::transformDirection(const Vector3 &vector) const
{
	return Vector3(vector.x*values[0]+vector.y*values[1]+vector.z*values[2],
		           vector.x*values[4]+vector.y*values[5]+vector.z*values[6],
				   vector.x*values[8]+vector.y*values[9]+vector.z*values[10]);
}

Vector3 Matrix4::transformInverseDirection(const Vector3 &vector) const
{
	return Vector3(vector.x*values[0]+vector.y*values[4]+vector.z*values[8],
		           vector.x*values[1]+vector.y*values[5]+vector.z*values[9],
				   vector.x*values[2]+vector.y*values[6]+vector.z*values[10]);
}

Vector3 Matrix4::localToWorld(const Vector3 &local)
{
	return transform(local);
}

Vector3 Matrix4::worldToLocal(const Vector3 &world)
{
	return transformInverse(world);
}

Vector3 Matrix4::localToWorldDirn(const Vector3 &local)
{
	return transformDirection(local);
}

Vector3 Matrix4::worldToLocalDirn(const Vector3 &world)
{
	return transformInverseDirection(world);
}

Vector3 operator*(const Matrix4 &m, const Vector3 &v)
{
	return Vector3(v.x*m.values[0]+v.y*m.values[1]+v.z*m.values[2]+m.values[3],
		           v.x*m.values[4]+v.y*m.values[5]+v.z*m.values[6]+m.values[7],
				   v.x*m.values[8]+v.y*m.values[9]+v.z*m.values[10]+m.values[11]);
}

Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2)
{
	Matrix4 result;
	result.values[0] = m2.values[0]*m1.values[0]+m2.values[4]*m1.values[1]+m2.values[8]*m1.values[2];
	result.values[4] = m2.values[0]*m1.values[4]+m2.values[4]*m1.values[5]+m2.values[8]*m1.values[6];
	result.values[8] = m2.values[0]*m1.values[8]+m2.values[4]*m1.values[9]+m2.values[8]*m1.values[10];
	result.values[1] = m2.values[1]*m1.values[0]+m2.values[5]*m1.values[1]+m2.values[9]*m1.values[2];
	result.values[5] = m2.values[1]*m1.values[4]+m2.values[5]*m1.values[5]+m2.values[9]*m1.values[6];
	result.values[9] = m2.values[1]*m1.values[8]+m2.values[5]*m1.values[9]+m2.values[9]*m1.values[10];
	result.values[2] = m2.values[2]*m1.values[0]+m2.values[6]*m1.values[1]+m2.values[10]*m1.values[2];
	result.values[6] = m2.values[2]*m1.values[4]+m2.values[6]*m1.values[5]+m2.values[10]*m1.values[6];
	result.values[10] = m2.values[2]*m1.values[8]+m2.values[6]*m1.values[9]+m2.values[10]*m1.values[10];
	result.values[3] = m2.values[3]*m1.values[0]+m2.values[7]*m1.values[1]+m2.values[11]*m1.values[2]+m1.values[3];
	result.values[7] = m2.values[3]*m1.values[4]+m2.values[7]*m1.values[5]+m2.values[11]*m1.values[6]+m1.values[7];
	result.values[11] = m2.values[3]*m1.values[8]+m2.values[7]*m1.values[9]+m2.values[11]*m1.values[10]+m1.values[11];

	return result;
}