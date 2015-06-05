#pragma once

#ifndef __TransformUtils_H
#define __TransformUtils_H

#include "Vector3.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace Math
{
	inline Vector3 rotateAround(Vector3 &in, const Vector3 &axis, float angle)
	{
		Matrix3 rotation(axis, angle);

		return in * rotation;
	}

	inline Matrix4 rotateAround(Matrix4 &in, const Vector3 &axis, float angle)
	{
		Matrix3 rotation(axis, angle);

		Matrix4 trans(rotation);

		return in * trans;
	}

	inline Matrix4 scale(Matrix4 &in, const Vector3 &scale)
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

	Matrix4 perspectiveMatrix(float fovy, float aspectRatio, float zNear, float zFar)
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
}

#endif