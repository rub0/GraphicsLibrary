#pragma once

#ifndef __MatrixOperations_H
#define __MatrixOperations_H

#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Math
{
	using namespace Engine;

	Matrix4 createTransMatrix(const Matrix4 &mat, const Vector3 &tvec)
	{
		Matrix4 result(mat);

		result.m03 += tvec.x;
		result.m13 += tvec.y;
		result.m23 += tvec.z;

		return result;
	}

	Matrix4 createTransMatrix(const Vector4 &tvec)
	{
		Matrix4 result;

		result.m03 += tvec.x;
		result.m13 += tvec.y;
		result.m23 += tvec.z;
		result.m33 += tvec.w;

		return result;
	}

	void scale(const Matrix4 &mat, const Vector3 &scaleVector)
	{
	}

}

#endif