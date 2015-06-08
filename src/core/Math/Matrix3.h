#pragma once

#ifndef __Matrix3_H
#define __Matrix3_H

#include <ostream>

namespace Engine
{
	//forward decs
	class Quaternion;
	class Vector3;

	class Matrix3
	{
	public:

		static const size_t MATRIX_SIZE = sizeof(float) * 9;

		// from: https://github.com/cinder/Cinder/blob/master/include/cinder/Matrix33.h
		// This class is OpenGL friendly and stores the m as how OpenGL would expect it.
		// m[i,j]:
		// | m[0,0] m[0,1] m[0,2] |
		// | m[1,0] m[1,1] m[1,2] |
		// | m[2,0] m[2,1] m[2,2] |
		//
		// m[idx]
		// | m[0] m[3] m[6] |
		// | m[1] m[4] m[7] |
		// | m[2] m[5] m[8] |
		//
		union {
			float m[9];
			struct {
				// This looks like it's transposed from the above, but it's really not.
				// It just has to be written this way so it follows the right ordering
				// in the memory layout as well as being mathematically correct.
				float m00, m10, m20;
				float m01, m11, m21;
				float m02, m12, m22;
			};
			// [Cols][Rows]
			float mcols[3][3];
		};


		Matrix3();
		Matrix3(const Matrix3 &other)
		{
			std::memcpy( m, other.m, MATRIX_SIZE );
		}
		Matrix3(const Vector3 &axis, float angle);


		/*******************
		* BASIC OPERATIONS *
		********************/

		const Matrix3 operator*(const Matrix3 &rhs)
		{
			Matrix3 result;
			result.m00 = m00 * rhs.m00 + m10 * rhs.m01 + m20 * rhs.m02;
			result.m10 = m00 * rhs.m10 + m10 * rhs.m11 + m20 * rhs.m12;
			result.m20 = m00 * rhs.m20 + m10 * rhs.m21 + m20 * rhs.m22;

			result.m01 = m01 * rhs.m00 + m11 * rhs.m01 + m21 * rhs.m02;
			result.m11 = m01 * rhs.m10 + m11 * rhs.m11 + m21 * rhs.m12;
			result.m21 = m01 * rhs.m20 + m11 * rhs.m21 + m21 * rhs.m22;

			result.m02 = m02 * rhs.m00 + m12 * rhs.m01 + m22 * rhs.m02;
			result.m12 = m02 * rhs.m10 + m12 * rhs.m11 + m22 * rhs.m12;
			result.m22 = m02 * rhs.m20 + m12 * rhs.m21 + m22 * rhs.m22;

			return result;
		}

		const Matrix3 operator/(const Matrix3 &rhs)
		{

		}

		const Matrix3 operator+(const Matrix3 &rhs)
		{

		}

		const Matrix3 operator-(const Matrix3 &rhs)
		{

		}

		const Matrix3 operator==(const Matrix3 &rhs)
		{

		}

		const Matrix3 operator!=(const Matrix3 &rhs)
		{

		}

		friend std::ostream& operator<<( std::ostream& lhs, const Matrix3 &rhs )
		{
			lhs << "[" << rhs.m00 << " " << rhs.m10 << " " << rhs.m20 << "]" << std::endl;
			lhs << "[" << rhs.m01 << " " << rhs.m11 << " " << rhs.m21 << "]" << std::endl;
			lhs << "[" << rhs.m02 << " " << rhs.m12 << " " << rhs.m22 << "]" << std::endl;
			return lhs;
		}

		/**********************
		* ADVANCED OPERATIONS *
		***********************/

		float getPitch()
		{
			return asin(mcols[0][2]);
		}
		float getYaw()
		{
			return atan2(m20, m21);
		}

		float getRoll()
		{
			return - atan2(m02, m12);
		}

		void setToIdentity()
		{
			m[ 0] = 1; m[ 3] = 0; m[ 6] = 0;
			m[ 1] = 0; m[ 4] = 1; m[ 7] = 0;
			m[ 2] = 0; m[ 5] = 0; m[ 8] = 1;
		}

		void setToZero()
		{
			m[ 0] = 0; m[ 3] = 0; m[ 6] = 0;
			m[ 1] = 0; m[ 4] = 0; m[ 7] = 0;
			m[ 2] = 0; m[ 5] = 0; m[ 8] = 0;
		}

	private:


	};
}
#endif