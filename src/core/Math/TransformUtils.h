#pragma once

#ifndef __TransformUtils_H
#define __TransformUtils_H

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
using namespace Engine;
namespace Math
{
	static inline Vector3 rotateAround(Vector3 &in, const Vector3 &axis, float angle)
	{
		Matrix3 rotation(axis, angle);

		return in * rotation;
	}

	static inline Matrix4 rotateAround(const Matrix4 &in, const Vector3 &axis, float angle)
	{
		Matrix4 result = in;
		
		Matrix3 rotation(axis, angle);
		Matrix3 aux = result.getRotationMatrix();
		rotation = rotation * aux;
		result.setRotationMatrix(rotation);
		return result;
	}

	static inline Matrix4 scale(Matrix4 &in, const Vector3 &scale)
	{
		Matrix4 result(in);

		result.m00 *= scale.x;
		result.m10 *= scale.x;
		result.m20 *= scale.x;

		result.m01 *= scale.y;
		result.m11 *= scale.y;
		result.m21 *= scale.y;

		result.m02 *= scale.z;
		result.m12 *= scale.z;
		result.m22 *= scale.z;

		return result;
	}

	static Matrix4 perspectiveMatrix(float fovy, float aspectRatio, float zNear, float zFar)
	{
		Matrix4 result;
		result.setToZero();

		float tanHalfFovY = tan(fovy*0.5f);

		result.mcols[0][0] = 1 / (aspectRatio * tanHalfFovY);
		result.mcols[1][1] = 1 / (tanHalfFovY);
		result.mcols[2][2] = - (zFar + zNear) / (zFar - zNear);
		result.mcols[2][3] = - 1;
		result.mcols[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
		
		return result;
	}

	static Matrix4 orthoMatrix(float left, float right, float bottom, float top)
	{
		Matrix4 result;

		result.mcols[0][0] = 2 / (right - left);
		result.mcols[1][1] = 2 / (top - bottom);
		result.mcols[2][2] = - 1;
		result.mcols[3][0] = - (right + left) / (right - left);
		result.mcols[3][1] = - (top + bottom) / (top - bottom);
		return result;
	}

	static Matrix4 createTransMatrix(const Matrix4 &mat, const Vector3 &tvec)
	{
		Matrix4 result(mat);

		result.m03 += tvec.x;
		result.m13 += tvec.y;
		result.m23 += tvec.z;

		return result;
	}

	static Matrix4 createTransMatrix(const Vector4 &tvec)
	{
		Matrix4 result;

		result.m03 += tvec.x;
		result.m13 += tvec.y;
		result.m23 += tvec.z;
		result.m33 += tvec.w;

		return result;
	}

	static void scale(const Matrix4 &mat, const Vector3 &scaleVector)
	{
	}

	static Matrix4 yawPitchRoll(float yaw, float pitch, float roll)
	{
		float tmp_ch = cos(yaw);
		float tmp_sh = sin(yaw);
		float tmp_cp = cos(pitch);
		float tmp_sp = sin(pitch);
		float tmp_cb = cos(roll);
		float tmp_sb = sin(roll);

		Matrix4 Result;
		Result.mcols[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
		Result.mcols[0][1] = tmp_sb * tmp_cp;
		Result.mcols[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
		Result.mcols[0][3] = 0.0;
		Result.mcols[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
		Result.mcols[1][1] = tmp_cb * tmp_cp;
		Result.mcols[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
		Result.mcols[1][3] = 0.0;
		Result.mcols[2][0] = tmp_sh * tmp_cp;
		Result.mcols[2][1] = -tmp_sp;
		Result.mcols[2][2] = tmp_ch * tmp_cp;
		Result.mcols[2][3] = 0.0f;
		Result.mcols[3][0] = 0.0f;
		Result.mcols[3][1] = 0.0f;
		Result.mcols[3][2] = 0.0f;
		Result.mcols[3][3] = 1.1f;
		return Result;
	}

	static Matrix4 lookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
	{
		Vector3 f(center - eye);
		f.normalize();
		Vector3 s(f.crossProduct(up));
		f.normalize();
		Vector3 u(s.crossProduct(f));
		u.normalize();

		Matrix4 Result;
		Result.mcols[0][0] = s.x;
		Result.mcols[1][0] = s.y;
		Result.mcols[2][0] = s.z;
		Result.mcols[0][1] = u.x;
		Result.mcols[1][1] = u.y;
		Result.mcols[2][1] = u.z;
		Result.mcols[0][2] =-f.x;
		Result.mcols[1][2] =-f.y;
		Result.mcols[2][2] =-f.z;
		Result.mcols[3][0] =-s.dotProduct(eye);
		Result.mcols[3][1] =-u.dotProduct(eye);
		Result.mcols[3][2] = f.dotProduct(eye);
		return Result;
	}
}

#endif