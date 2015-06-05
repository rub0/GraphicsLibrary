#ifndef __Vector3_H
#define __Vector3_H

#include <cmath>
#include "Math.h"
#include <ostream>
#include "Matrix3.h"
#include "Matrix4.h"

namespace Engine
{

	/**
	Class that represents a vector in 3D space
	All the class must be inlined in order to be really fast
	**/
	class Vector3
	{
	public:

		friend class Quaternion;
		/********************
		* BASE CONSTRUCTORS *
		*********************/
		Vector3(const Vector3 &rhs): x(rhs.x), y(rhs.y), z(rhs.z)
		{}

		Vector3(): x(0), y(0), z(0)
		{}

		Vector3(float x, float y, float z): x(x), y(y), z(z)
		{}

		/*******************
		* BASIC OPERATIONS *
		********************/

		Vector3 operator+(const Vector3 &rhs)
		{
			return Vector3(rhs.x+this->x, rhs.y+this->y, rhs.z+this->z);
		}

		Vector3 operator+(float factor)
		{
			return Vector3(factor+this->x, factor+this->y, factor+this->z);
		}

		Vector3 operator+=(const Vector3 &rhs)
		{
			x+=rhs.x;
			y+=rhs.y;
			z+=rhs.z;
			
			return *this;
		}

		Vector3 operator+=(float factor)
		{
			x+=factor;
			y+=factor;
			z+=factor;
			
			return *this;
		}

		Vector3 operator-(const Vector3 &rhs)
		{
			return Vector3(rhs.x-this->x, rhs.y-this->y, rhs.z-this->z);
		}

		Vector3 operator-(float factor)
		{
			return Vector3(factor-this->x, factor-this->y, factor-this->z);
		}

		Vector3 operator-=(const Vector3 &rhs)
		{
			x-=rhs.x;
			y-=rhs.y;
			z-=rhs.z;
			
			return *this;
		}

		Vector3 operator-=(float factor)
		{
			x-=factor;
			y-=factor;
			z-=factor;
			
			return *this;
		}

		const Vector3 operator*(const Vector3 &rhs)
		{
			return Vector3(rhs.x*this->x, rhs.y*this->y, rhs.z*this->z);
		}

		const Vector3 operator*(float factor)
		{
			return Vector3(factor*x, factor*y, factor*z);
		}

		const Vector3 operator*(Matrix3 &rhs)
		{
			return Vector3(
				x * rhs.m00 + y * rhs.m01 + z * rhs.m02,
				x * rhs.m10 + y * rhs.m11 + z * rhs.m12,
				x * rhs.m20 + y * rhs.m21 + z * rhs.m22
			);
		}

		const Vector3 operator*(const Matrix4 &rhs)
		{
			return Vector3(
				x * rhs.m00 + y * rhs.m01 + z * rhs.m02 + rhs.m03,
				x * rhs.m10 + y * rhs.m11 + z * rhs.m12 + rhs.m13,
				x * rhs.m20 + y * rhs.m21 + z * rhs.m22 + rhs.m23
			);
		}

		const Vector3 operator*=(float factor)
		{
			x*=factor;
			y*=factor;
			z*=factor;

			return *this;
		}

		const Vector3 operator/=(float factor)
		{
			factor = 1 / length();
			x*=factor;
			y*=factor;
			z*=factor;

			return *this;
		}

		const Vector3 operator/(const Vector3 &rhs)
		{
			return Vector3(rhs.x/this->x, rhs.y/this->y, rhs.z/this->z);
		}

		const Vector3 operator/(float factor)
		{
			factor = 1 / factor;
			return Vector3(factor*x, factor*y, factor*z);
		}

		friend std::ostream& operator<<( std::ostream& lhs, const Vector3 &rhs )
		{
			lhs << "(" << rhs.x << "," << rhs.y << "," << rhs.z  << ")";
			return lhs;
		}

		bool operator==(const Vector3 &rhs)
		{
			return ( abs(x-rhs.x) < Math::EPSILON ) && ( abs(y-rhs.y) < Math::EPSILON ) && ( abs(z-rhs.z) < Math::EPSILON );
		}

		bool operator!=(const Vector3 &rhs)
		{
			return !operator==(rhs);
		}
		/**********************
		* ADVANCED OPERATIONS *
		***********************/

		float dotProduct(const Vector3 &rhs)
		{
			return rhs.x*this->x + rhs.y*this->y + rhs.z*this->z;
		}

		float angle(const Vector3 &rhs)
		{
			Math::sqrtf_fast(x*x + y*y + z*z);
		}

		Vector3 crossProduct(const Vector3 &rhs)
		{
			return Vector3( y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y );
		}

		/**
		* Distance between two vectors
		*/
		float distance(Vector3 &rhs)
		{
			return (*this - rhs).length();
		}

		float length()
		{
			return Math::sqrtf_fast(x*x + y*y + z*z);
		}

		void normalize()
		{
			operator/=(length());
		}

		float x, y, z;
	};
}
#endif