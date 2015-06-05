#pragma once
#ifndef __Vector4_H
#define __Vector4_H

#include <cmath>
#include "Math.h"
#include <ostream>
#include "Matrix4.h"

namespace Engine
{

	/**
	Class that represents a vector in 3D space
	All the class must be inlined in order to be really fast
	**/
	class Vector4
	{
	public:

		friend class Quaternion;
		/********************
		* BASE CONSTRUCTORS *
		*********************/
		Vector4(const Vector4 &rhs): x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
		{}

		Vector4(): x(0), y(0), z(0), w(0)
		{}

		Vector4(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
		{}

		/*******************
		* BASIC OPERATIONS *
		********************/

		Vector4 operator+(const Vector4 &rhs)
		{
			return Vector4(rhs.x+this->x, rhs.y+this->y, rhs.z+this->z, rhs.w+this->w);
		}

		Vector4 operator+(float factor)
		{
			return Vector4(factor + x, factor + y, factor + z, factor + w);
		}

		Vector4 operator+=(const Vector4 &rhs)
		{
			x+=rhs.x;
			y+=rhs.y;
			z+=rhs.z;
			
			return *this;
		}

		Vector4 operator+=(float factor)
		{
			x+=factor;
			y+=factor;
			z+=factor;
			w+=factor;
			return *this;
		}

		Vector4 operator-(const Vector4 &rhs)
		{
			return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		Vector4 operator-(float factor)
		{
			return Vector4(x - factor, y - factor, z - factor, w - factor);
		}

		Vector4 operator-=(const Vector4 &rhs)
		{
			x-=rhs.x;
			y-=rhs.y;
			z-=rhs.z;
			w-=rhs.w;
			return *this;
		}

		Vector4 operator-=(float factor)
		{
			x-=factor;
			y-=factor;
			z-=factor;
			w-=factor;
			return *this;
		}

		const Vector4 operator*(const Vector4 &rhs)
		{
			return Vector4(rhs.x*x, rhs.y*y, rhs.z*z, rhs.w*w);
		}

		const Vector4 operator*(float factor)
		{
			return Vector4(factor*x, factor*y, factor*z, factor*w);
		}

		const Vector4 operator*=(float factor)
		{
			x*=factor;
			y*=factor;
			z*=factor;
			w*=factor;
			return *this;
		}

		const Vector4 operator/=(float factor)
		{
			factor = 1 / length();
			x*=factor;
			y*=factor;
			z*=factor;
			w*=factor;
			return *this;
		}

		const Vector4 operator/(const Vector4 &rhs)
		{
			return Vector4(x/rhs.x, y/rhs.y, z/rhs.z, w/rhs.w);
		}

		const Vector4 operator/(float factor)
		{
			factor = 1 / factor;
			return Vector4(factor*x, factor*y, factor*z, factor * w);
		}

		friend std::ostream& operator<<( std::ostream& lhs, const Vector4 &rhs )
		{
			lhs << "(" << rhs.x << "," << rhs.y << "," << rhs.z  << "," << rhs.w  << ")";
			return lhs;
		}

		bool operator==(const Vector4 &rhs)
		{
			return ( abs(x-rhs.x) < Math::EPSILON ) && ( abs(y-rhs.y) < Math::EPSILON ) && ( abs(z-rhs.z) < Math::EPSILON ) && ( abs(w-rhs.w) < Math::EPSILON );
		}

		bool operator!=(const Vector4 &rhs)
		{
			return !operator==(rhs);
		}

		

		const Vector4 operator*(const Matrix4 &rhs)
		{
			return Vector4(
				x * rhs.m00 + y * rhs.m01 + z * rhs.m02 + w * rhs.m03,
				x * rhs.m10 + y * rhs.m11 + z * rhs.m12 + w * rhs.m13,
				x * rhs.m20 + y * rhs.m21 + z * rhs.m22 + w * rhs.m23,
				x * rhs.m30 + y * rhs.m31 + z * rhs.m32 + w * rhs.m33
			);
		}

		/**********************
		* ADVANCED OPERATIONS *
		***********************/

		float dotProduct(const Vector4 &rhs)
		{
			return rhs.x*this->x + rhs.y*this->y + rhs.z*this->z + rhs.w*this->w;
		}

		/**
		* Distance between two vectors
		*/
		float distance(Vector4 &rhs)
		{
			return (*this - rhs).length();
		}

		float length()
		{
			return Math::sqrtf_fast(x*x + y*y + z*z + w*w);
		}

		void normalize()
		{
			operator/=(length());
		}

		float x, y, z, w;
	};
}
#endif