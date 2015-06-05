#pragma once

#include <ostream>

#ifndef __Matrix4_H
#define __Matrix4_H

#include "Math.h"

namespace Engine
{
	//forward decs
	class Quaternion;
	class Matrix3;
	class Vector4;
	class Matrix4
	{
	public:

		static const size_t MATRIX_SIZE = sizeof(float) * 16;

		Matrix4();
		Matrix4(const Quaternion &q);
		Matrix4(const Matrix4 &other)
		{
			std::memcpy( m, other.m, MATRIX_SIZE );
		}
		Matrix4(const Matrix3 &other);

		// from: https://github.com/cinder/Cinder/blob/master/include/cinder/Matrix44.h
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
			float m[16];
			struct {
				// This looks like it's transposed from the above, but it's really not.
				// It just has to be written this way so it follows the right ordering
				// in the memory layout as well as being mathematically correct.
				float m00, m10, m20, m30;
				float m01, m11, m21, m31;
				float m02, m12, m22, m32;
				float m03, m13, m23, m33;
			};
			// [Cols][Rows]
			float mcols[4][4];
		};


		/*******************
		* BASIC OPERATIONS *
		********************/

		const Matrix4 operator*(const Matrix4 &rhs);
		const Matrix4 operator*=(const Matrix4 &rhs);

		const Matrix4 operator/(float rhs);
		const Matrix4 operator/(const Matrix4 &rhs);
		
		const Matrix4 operator/=(const Matrix4 &rhs);
		const Matrix4 operator+(const Matrix4 &rhs);
		const Matrix4 operator+=(const Matrix4 &rhs);

		const Matrix4 operator-(const Matrix4 &rhs);
		const Matrix4 operator-=(const Matrix4 &rhs);

		bool operator==(const Matrix4 &rhs)
		{
			for( int i = 0; i < 16; ++i ) {
				if(abs(m[i] - rhs.m[i]) >= Math::EPSILON )
					return false;
			}
			return true;
		}

		bool operator!=(const Matrix4 &rhs)
		{
			return !operator==(rhs);
		}

		const Matrix4 operator=(const Matrix4 &rhs)
		{
			std::memcpy( m, rhs.m, MATRIX_SIZE );
			return *this;
		}

		const Matrix4 operator=(const Matrix3 &rhs);

		friend std::ostream& operator<<( std::ostream& lhs, const Matrix4 &rhs )
		{
			lhs << "[" << rhs.m00 << " " << rhs.m10 << " " << rhs.m20 << " " << rhs.m30 << "]" << std::endl;
			lhs << "[" << rhs.m01 << " " << rhs.m11 << " " << rhs.m21 << " " << rhs.m31 << "]" << std::endl;
			lhs << "[" << rhs.m02 << " " << rhs.m12 << " " << rhs.m22 << " " << rhs.m32 << "]" << std::endl;
			lhs << "[" << rhs.m03 << " " << rhs.m13 << " " << rhs.m23 << " " << rhs.m33 << "]" << std::endl;
			return lhs;
		}

		/**********************
		* ADVANCED OPERATIONS *
		***********************/

		void setToIdentity()
		{
			m[ 0] = 1; m[ 4] = 0; m[ 8] = 0; m[12] = 0;
			m[ 1] = 0; m[ 5] = 1; m[ 9] = 0; m[13] = 0;
			m[ 2] = 0; m[ 6] = 0; m[10] = 1; m[14] = 0;
			m[ 3] = 0; m[ 7] = 0; m[11] = 0; m[15] = 1;
		}

		void setToZero()
		{
			m[ 0] = 0; m[ 4] = 0; m[ 8] = 0; m[12] = 0;
			m[ 1] = 0; m[ 5] = 0; m[ 9] = 0; m[13] = 0;
			m[ 2] = 0; m[ 6] = 0; m[10] = 0; m[14] = 0;
			m[ 3] = 0; m[ 7] = 0; m[11] = 0; m[15] = 0;
		}

		Matrix4 transpose();
		Matrix4 inverse();
		float determinant()
		{
			float a0 = m[ 0]*m[ 5] - m[ 1]*m[ 4];
			float a1 = m[ 0]*m[ 6] - m[ 2]*m[ 4];
			float a2 = m[ 0]*m[ 7] - m[ 3]*m[ 4];
			float a3 = m[ 1]*m[ 6] - m[ 2]*m[ 5];
			float a4 = m[ 1]*m[ 7] - m[ 3]*m[ 5];
			float a5 = m[ 2]*m[ 7] - m[ 3]*m[ 6];
			float b0 = m[ 8]*m[13] - m[ 9]*m[12];
			float b1 = m[ 8]*m[14] - m[10]*m[12];
			float b2 = m[ 8]*m[15] - m[11]*m[12];
			float b3 = m[ 9]*m[14] - m[10]*m[13];
			float b4 = m[ 9]*m[15] - m[11]*m[13];
			float b5 = m[10]*m[15] - m[11]*m[14];

			return a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
		}
	private:


	};
}
#endif