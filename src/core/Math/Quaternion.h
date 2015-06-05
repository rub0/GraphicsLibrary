#ifndef __Quaternion_H
#define __Quaternion_H

#include <cmath>
#include "Vector3.h"

namespace Engine
{
	//FDs
	class Matrix3;

	class Quaternion
	{

	public:
		Quaternion():x(0),y(0),z(0),w(1){}
		Quaternion(float x, float y, float z, float w):x(x),y(y),z(z),w(w){}
		Quaternion(const Quaternion &other):x(other.x),y(other.y),z(other.z),w(other.w){}
		Quaternion(const Matrix3 &m);
		Quaternion(float pitch, float yaw, float roll)
		{
			float cr, cp, cy, sr, sp, sy, cpcy, spsy;
			// calculate trig identities
			cr = cos(roll*0.5f);
			cp = cos(pitch*0.5f);
			cy = cos(yaw*0.5f);
			sr = sin(roll*0.5f);
			sp = sin(pitch*0.5f);
			sy = sin(yaw*0.5f);
			cpcy = cp * cy;
			spsy = sp * sy;
			w = cr * cpcy + sr * spsy;
			x = sr * cpcy - cr * spsy;
			y = cr * sp * cy + sr * cp * sy;
			z = cr * cp * sy - sr * sp * cy;
		}

		/**
		* Builds a new quaternion with given axis and angle
		* Axis must be normalized
		*/
		Quaternion(const Vector3 &axis, float angle)
		{
			float sinAngle = sin(angle * 0.5f);
			 w = cos(angle);
			 x = axis.x * sinAngle;
			 y = axis.y * sinAngle;
			 z = axis.z * sinAngle;
		}

		/*******************
		* BASIC OPERATIONS *
		********************/

		const Quaternion operator+(const Quaternion &rhs)
		{
			return Quaternion(x+rhs.x, y+rhs.y, z+rhs.z, w+w);
		}

		Quaternion& operator+=(const Quaternion &rhs)
		{
			x+=rhs.x;
			y+=rhs.y;
			z+=rhs.z;
			w+=rhs.w;

			return *this;
		}

		const Quaternion operator-(const Quaternion &rhs)
		{
			return Quaternion(x+rhs.x, y+rhs.y, z+rhs.z, w+w);
		}

		Quaternion& operator-=(const Quaternion &rhs)
		{
			x-=rhs.x;
			y-=rhs.y;
			z-=rhs.z;
			w-=rhs.w;

			return *this;
		}

		const Quaternion operator*(float factor)
		{
			return Quaternion(x*factor, y*factor, z*factor, w*factor);
		}

		Quaternion& operator*=(float factor)
		{
			x*=factor;
			y*=factor;
			z*=factor;
			w*=factor;

			return *this;
		}

		const Quaternion operator*(const Quaternion &rhs)
		{
			return Quaternion(
				w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
				w*rhs.y + y*rhs.w - x*rhs.z + z*rhs.x, 
				w*rhs.y + y*rhs.w + x*rhs.y - y*rhs.x, 
				w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z
				);
		}

		/**
		*  Rotates a vector with this quaternion
		*/
		Vector3 Quaternion::operator* (const Vector3 &rhs) const
		{
			// nVidia SDK implementation
			Vector3 uv, uuv;
			Vector3 qvec(x, y, z);
			uv = qvec.crossProduct(rhs);
			uuv = qvec.crossProduct(uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;

			return uv + uuv + rhs;
		}

		const Quaternion operator/(float factor)
		{
			float div = 1/factor;
			return Quaternion(x*div, y*div, z*div, w*div);
		}

		Quaternion& operator/=(float factor)
		{
			float div = 1/factor;
			x*=div;
			y*=div;
			z*=div;
			w*=div;

			return *this;
		}

		bool operator==(const Quaternion &rhs)
		{
			return	( abs(x-rhs.x) < Math::EPSILON ) && 
					( abs(y-rhs.y) < Math::EPSILON ) && 
					( abs(z-rhs.z) < Math::EPSILON ) && 
					( abs(w-rhs.w) < Math::EPSILON );
		}

		bool operator!=(const Quaternion &rhs)
		{
			return !operator==(rhs);
		}

		friend std::ostream& operator<<( std::ostream& lhs, const Quaternion &rhs )
		{
			lhs << "(" << rhs.w << rhs.x << "i," << rhs.y << "j," << rhs.z  << "k)";
			return lhs;
		}

		/**********************
		* ADVANCED OPERATIONS *
		***********************/

		Vector3 xAxis(void) const;
		Vector3 yAxis(void) const;
		Vector3 zAxis(void) const;

		float getPitch()
		{
			return atan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
		}
		float getYaw()
		{
			return asin(-2*(x*z - w*y));
		}
		float getRoll()
		{
			return atan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);
		}

		Quaternion conjugate()
		{
			return Quaternion(-x,-y,-z,w);
		}

		void normalize()
		{
			operator/=(length());
		}

		float length()
		{
			return Math::sqrtf_fast(x*x + y*y + z*z + w*w);
		}

		/**
		* Inverse this quaternion
		* Inverse = q* / |q|^2
		*/
		Quaternion invert()
		{
			float l = length();
			return conjugate() / (l*l);
		}

		static Quaternion normalize(const Quaternion &q)
		{
			Quaternion result(q);
			result.normalize();
			return result;
		}

		Matrix3 getRotationMatrix();

		void toAngleAxis(float& angle, Vector3& axis) const;

		/**
		* Asks if the quaternion is pure (w = 0)
		*/
		bool isPure(){return w < 1.19209290E-07F;}
		bool isIdentity(){return w > 0.999999 && x < Math::EPSILON && y < Math::EPSILON && z < Math::EPSILON;}
	private:
		float x, y, z, w;
	};
}

#endif