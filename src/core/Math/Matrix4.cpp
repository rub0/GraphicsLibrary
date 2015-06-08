#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include <xmmintrin.h>
#include "Vector4.h"


namespace Engine
{
	Matrix4::Matrix4()
	{
		setToIdentity();
	}

	Matrix4::Matrix4(const Quaternion &q)
	{
		setToIdentity();
	}
	Matrix4::Matrix4(const Matrix3 &other)
	{
		setToIdentity();

		m00 = other.m00; m01 = other.m01; m02 = other.m02;
		m10 = other.m10; m11 = other.m11; m12 = other.m12;
		m20 = other.m20; m21 = other.m21; m22 = other.m22;
	}

	const Matrix4 Matrix4::operator=(const Matrix3 &rhs)
	{
		setToIdentity();

		m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02;
		m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12;
		m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22;

		return *this;
	}

	const Matrix4 Matrix4::operator*(const Matrix4 &rhs)
	{
		Matrix4 result;
		result.m00 = m00 * rhs.m00 + m10 * rhs.m01 + m20 * rhs.m02 + m30 * rhs.m03;
		result.m10 = m00 * rhs.m10 + m10 * rhs.m11 + m20 * rhs.m12 + m30 * rhs.m13;
		result.m20 = m00 * rhs.m20 + m10 * rhs.m21 + m20 * rhs.m22 + m30 * rhs.m23;
		result.m30 = m00 * rhs.m30 + m10 * rhs.m31 + m20 * rhs.m32 + m30 * rhs.m33;

		result.m01 = m01 * rhs.m00 + m11 * rhs.m01 + m21 * rhs.m02 + m31 * rhs.m03;
		result.m11 = m01 * rhs.m10 + m11 * rhs.m11 + m21 * rhs.m12 + m31 * rhs.m13;
		result.m21 = m01 * rhs.m20 + m11 * rhs.m21 + m21 * rhs.m22 + m31 * rhs.m23;
		result.m31 = m01 * rhs.m30 + m11 * rhs.m31 + m21 * rhs.m32 + m31 * rhs.m33;

		result.m02 = m02 * rhs.m00 + m12 * rhs.m01 + m22 * rhs.m02 + m32 * rhs.m03;
		result.m12 = m02 * rhs.m10 + m12 * rhs.m11 + m22 * rhs.m12 + m32 * rhs.m13;
		result.m22 = m02 * rhs.m20 + m12 * rhs.m21 + m22 * rhs.m22 + m32 * rhs.m23;
		result.m32 = m02 * rhs.m30 + m12 * rhs.m31 + m22 * rhs.m32 + m32 * rhs.m33;

		result.m03 = m03 * rhs.m00 + m13 * rhs.m01 + m23 * rhs.m02 + m33 * rhs.m03;
		result.m13 = m03 * rhs.m10 + m13 * rhs.m11 + m23 * rhs.m12 + m33 * rhs.m13;
		result.m23 = m03 * rhs.m20 + m13 * rhs.m21 + m23 * rhs.m22 + m33 * rhs.m23;
		result.m33 = m03 * rhs.m30 + m13 * rhs.m31 + m23 * rhs.m32 + m33 * rhs.m33;

		return result;

	}

	const Matrix4 Matrix4::operator*=(const Matrix4 &rhs)
	{
		return (*this = *this * rhs);
	}

	const Matrix4 Matrix4::operator/(float rhs)
	{
		Matrix4 result;

		float div = 1/rhs;

		result.m00 = m00 * div;
		result.m01 = m01 * div;
		result.m02 = m02 * div;
		result.m03 = m03 * div;

		result.m10 = m10 * div;
		result.m11 = m11 * div;
		result.m12 = m12 * div;
		result.m13 = m13 * div;

		result.m20 = m20 * div;
		result.m21 = m21 * div;
		result.m22 = m22 * div;
		result.m23 = m23 * div;

		result.m30 = m30 * div;
		result.m31 = m31 * div;
		result.m32 = m32 * div;
		result.m33 = m33 * div;

		return result;
	}

	const Matrix4 Matrix4::operator+(const Matrix4 &rhs)
	{
		Matrix4 result;
		result.m00 = m00 + rhs.m00;
		result.m01 = m01 + rhs.m01;
		result.m02 = m02 + rhs.m02;
		result.m03 = m03 + rhs.m03;

		result.m10 = m10 + rhs.m10;
		result.m11 = m11 + rhs.m11;
		result.m12 = m12 + rhs.m12;
		result.m13 = m13 + rhs.m13;

		result.m20 = m20 + rhs.m20;
		result.m21 = m21 + rhs.m21;
		result.m22 = m22 + rhs.m22;
		result.m23 = m23 + rhs.m23;

		result.m30 = m30 + rhs.m30;
		result.m31 = m31 + rhs.m31;
		result.m32 = m32 + rhs.m32;
		result.m33 = m33 + rhs.m33;

		return result;
	}

	const Matrix4 Matrix4::operator+=(const Matrix4 &rhs)
	{
		m00 += rhs.m00;
		m01 += rhs.m01;
		m02 += rhs.m02;
		m03 += rhs.m03;

		m10 += rhs.m10;
		m11 += rhs.m11;
		m12 += rhs.m12;
		m13 += rhs.m13;

		m20 += rhs.m20;
		m21 += rhs.m21;
		m22 += rhs.m22;
		m23 += rhs.m23;

		m30 += rhs.m30;
		m31 += rhs.m31;
		m32 += rhs.m32;
		m33 += rhs.m33;

		return *this;
	}

	const Matrix4 Matrix4::operator-(const Matrix4 &rhs)
	{
		Matrix4 result;
		result.m00 = m00 - rhs.m00;
		result.m01 = m01 - rhs.m01;
		result.m02 = m02 - rhs.m02;
		result.m03 = m03 - rhs.m03;

		result.m10 = m10 - rhs.m10;
		result.m11 = m11 - rhs.m11;
		result.m12 = m12 - rhs.m12;
		result.m13 = m13 - rhs.m13;

		result.m20 = m20 - rhs.m20;
		result.m21 = m21 - rhs.m21;
		result.m22 = m22 - rhs.m22;
		result.m23 = m23 - rhs.m23;

		result.m30 = m30 - rhs.m30;
		result.m31 = m31 - rhs.m31;
		result.m32 = m32 - rhs.m32;
		result.m33 = m33 - rhs.m33;

		return result;
	}

	const Matrix4 Matrix4::operator-=(const Matrix4 &rhs)
	{
		Matrix4 result;
		m00 -= rhs.m00;
		m01 -= rhs.m01;
		m02 -= rhs.m02;
		m03 -= rhs.m03;

		m10 -= rhs.m10;
		m11 -= rhs.m11;
		m12 -= rhs.m12;
		m13 -= rhs.m13;

		m20 -= rhs.m20;
		m21 -= rhs.m21;
		m22 -= rhs.m22;
		m23 -= rhs.m23;

		m30 -= rhs.m30;
		m31 -= rhs.m31;
		m32 -= rhs.m32;
		m33 -= rhs.m33;

		return *this;
	}


	Matrix4 Matrix4::transpose()
	{
		Matrix4 result;
		result.m00 = m00;
		result.m11 = m11;
		result.m22 = m22;
		result.m33 = m33;

		result.m01 = m10;
		result.m02 = m20;
		result.m03 = m30;

		result.m10 = m01;
		result.m12 = m21;
		result.m13 = m31;

		result.m20 = m02;
		result.m21 = m12;
		result.m23 = m32;

		result.m30 = m03;
		result.m31 = m13;
		result.m32 = m23;

		return result;
	}

	Matrix4 Matrix4::inverse()
	{
		float det = determinant();

		return transpose()/det;
	}

	Matrix3 Matrix4::getRotationMatrix()
	{
		Matrix3 result;

		result.m00 = m00;
		result.m10 = m10;
		result.m20 = m20;

		result.m01 = m01;
		result.m11 = m11;
		result.m21 = m21;

		result.m02 = m02;
		result.m12 = m12;
		result.m22 = m22;

		return result;
	}

	void Matrix4::setRotationMatrix(const Matrix3 &rhs)
	{
		m00 = rhs.m00;
		m10 = rhs.m10;
		m20 = rhs.m20;

		m01 = rhs.m01;
		m11 = rhs.m11;
		m21 = rhs.m21;

		m02 = rhs.m02;
		m12 = rhs.m12;
		m22 = rhs.m22;
	}
}