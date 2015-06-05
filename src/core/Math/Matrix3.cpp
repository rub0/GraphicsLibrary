#include "Matrix3.h"
#include "Vector3.h"

namespace Engine
{
	Matrix3::Matrix3()
	{
	}
	Matrix3::Matrix3(const Matrix3 &other)
	{
	}

	Matrix3::Matrix3(const Vector3 &axis, float angle)
	{
		float cosin = cos(angle);
		float sinn = sin(angle);

		m00 = cosin + (1 - cosin) * axis.x * axis.x;
		m11 = cosin + (1 - cosin) * axis.y * axis.y;
		m22 = cosin + (1 - cosin) * axis.z * axis.z;

		m10 = (1 - cosin) * axis.x * axis.y + sinn * axis.z;
		m01 = (1 - cosin) * axis.x * axis.y - sinn * axis.z;
		m20 = (1 - cosin) * axis.z * axis.x - sinn * axis.y;
		m02 = (1 - cosin) * axis.z * axis.x + sinn * axis.y;
		m12 = (1 - cosin) * axis.z * axis.y - sinn * axis.x;
		m21 = (1 - cosin) * axis.z * axis.y + sinn * axis.x;
	}
}