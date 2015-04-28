#include "Matrix3.h"

const double DEFAULT_VALUE = 0;

Matrix3::Matrix3(double val1, double val2, double val3, double val4, double val5, 
				 double val6, double val7, double val8, double val9)
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
}

Matrix3::Matrix3(const Matrix3 &other)
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
}

void Matrix3::setInverse(const Matrix3 &m)
{
	double t1 = m.values[0] * m.values[4];
	double t2 = m.values[0] * m.values[5];
	double t3 = m.values[1] * m.values[3];
	double t4 = m.values[2] * m.values[3];
	double t5 = m.values[1] * m.values[6];
	double t6 = m.values[2] * m.values[6];

	double det = (t1*m.values[8] - t2*m.values[7] - t3*m.values[8] +
			      t4*m.values[7] + t5*m.values[5] - t6*m.values[4]);
	if (det == (double)0.0f)
	{
		return;
	}
	
	double invd = (double)1.0f/det;
	values[0] = (m.values[4]*m.values[8]-m.values[5]*m.values[7])*invd;
	values[1] = -(m.values[1]*m.values[8]-m.values[2]*m.values[7])*invd;
	values[2] = (m.values[1]*m.values[5]-m.values[2]*m.values[4])*invd;
	values[3] = -(m.values[3]*m.values[8]-m.values[5]*m.values[6])*invd;
	values[4] = (m.values[0]*m.values[8]-t6)*invd;
	values[5] = -(t2-t4)*invd;
	values[6] = (m.values[3]*m.values[7]-m.values[4]*m.values[6])*invd;
	values[7] = -(m.values[0]*m.values[7]-t5)*invd;
	values[8] = (t1-t3)*invd;
}

void Matrix3::setTranspose(const Matrix3 &m)
{
	values[0] = m.values[0];
	values[1] = m.values[3];
	values[2] = m.values[6];
	values[3] = m.values[1];
	values[4] = m.values[4];
	values[5] = m.values[7];
	values[6] = m.values[2];
	values[7] = m.values[5];
	values[8] = m.values[8];
}

void Matrix3::setOrientation(const Quaternion &q)
{
	//Fill once Quaternions are fixed
}

void Matrix3::setColumns(Vector3 col1, Vector3 col2, Vector3 col3)
{
	values[0] = col1.x;
	values[3] = col1.y;
	values[6] = col1.z;

	values[1] = col2.x;
	values[4] = col2.y;
	values[7] = col2.z;

	values[2] = col3.x;
	values[5] = col3.y;
	values[8] = col3.z;
}

void Matrix3::setSkewSymmetric(const Vector3 vector)
{
    values[0] = values[4] = values[8] = 0;
    values[1] = -vector.z;
    values[2] = vector.y;
    values[3] = vector.z;
    values[5] = -vector.x;
    values[6] = -vector.y;
    values[7] = vector.x;
}

Matrix3 Matrix3::invert()
{
	Matrix3 result;
	result.setInverse(*this);
	return result;
}

Matrix3 Matrix3::inverse() const
{
	Matrix3 result;
	result.setInverse(*this);
	return result;
}

Matrix3 Matrix3::transpose() const
{
	Matrix3 result;
	result.setTranspose(*this);
	return result;
}

Vector3 Matrix3::transform(const Vector3 &vector) const
{
	return (*this)*vector;
}

Vector3 Matrix3::transformTranspose(Vector3 &vector)
{
	return Vector3(vector.x * values[0] + vector.y * values[3] + vector.z * values[6],
                   vector.x * values[1] + vector.y * values[4] + vector.z * values[7],
                   vector.x * values[2] + vector.y * values[5] + vector.z * values[8]);
}

Vector3 operator*(const Matrix3 &m1, const Vector3 &v1)
{
	return Vector3(v1.x*m1.values[0]+v1.y*m1.values[1]+v1.z*m1.values[2],
		           v1.x*m1.values[3]+v1.y*m1.values[4]+v1.z*m1.values[5],
				   v1.x*m1.values[6]+v1.y*m1.values[7]+v1.z*m1.values[8]);
}

Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2)
{
	return Matrix3(m1.values[0]*m2.values[0] + m1.values[1]*m2.values[3] + m1.values[2]*m2.values[6],
				   m1.values[0]*m2.values[1] + m1.values[1]*m2.values[4] + m1.values[2]*m2.values[7],
				   m1.values[0]*m2.values[2] + m1.values[1]*m2.values[5] + m1.values[2]*m2.values[8],

				   m1.values[3]*m2.values[0] + m1.values[4]*m2.values[3] + m1.values[5]*m2.values[6],
				   m1.values[3]*m2.values[1] + m1.values[4]*m2.values[4] + m1.values[5]*m2.values[7],
				   m1.values[3]*m2.values[2] + m1.values[4]*m2.values[5] + m1.values[5]*m2.values[8],

				   m1.values[6]*m2.values[0] + m1.values[7]*m2.values[3] + m1.values[8]*m2.values[6],
				   m1.values[6]*m2.values[1] + m1.values[7]*m2.values[4] + m1.values[8]*m2.values[7],
				   m1.values[6]*m2.values[2] + m1.values[7]*m2.values[5] + m1.values[8]*m2.values[8]);
}

Matrix3& Matrix3::operator*=(const Matrix3 &m2)
{
	double t1;
	double t2;
	double t3;
	Matrix3& m1 = *this;

	t1 = m1.values[0]*m2.values[0] + m1.values[1]*m2.values[3] + m1.values[2]*m2.values[6];
	t2 = m1.values[0]*m2.values[1] + m1.values[1]*m2.values[4] + m1.values[2]*m2.values[7];
	t3 = m1.values[0]*m2.values[2] + m1.values[1]*m2.values[5] + m1.values[2]*m2.values[8];
	values[0] = t1;
	values[1] = t2;
	values[2] = t3;

	t1 = m1.values[3]*m2.values[0] + m1.values[4]*m2.values[3] + m1.values[5]*m2.values[6];
	t2 = m1.values[3]*m2.values[1] + m1.values[4]*m2.values[4] + m1.values[5]*m2.values[7];
	t3 = m1.values[3]*m2.values[2] + m1.values[4]*m2.values[5] + m1.values[5]*m2.values[8];
	values[3] = t1;
	values[4] = t2;
	values[5] = t3;

	t1 = m1.values[6]*m2.values[0] + m1.values[7]*m2.values[3] + m1.values[8]*m2.values[6];
	t2 = m1.values[6]*m2.values[1] + m1.values[7]*m2.values[4] + m1.values[8]*m2.values[7];
	t3 = m1.values[6]*m2.values[2] + m1.values[7]*m2.values[5] + m1.values[8]*m2.values[8];
	values[6] = t1;
	values[7] = t2;
	values[8] = t3;

	return *this;
}

Matrix3& Matrix3::operator+=(const Matrix3 &m2)
{
	values[0] = m2.values[0];
	values[1] = m2.values[1];
	values[2] = m2.values[2];
	values[3] = m2.values[3];
	values[4] = m2.values[4];
	values[5] = m2.values[5];
	values[6] = m2.values[6];
	values[7] = m2.values[7];
	values[8] = m2.values[8];

	return *this;
}

