#include "Matrix3.h"
#include "Vector3.h"

namespace Engine
{
	Matrix3::Matrix3()
	{
		setToIdentity();
	}

	Matrix3::Matrix3(const Vector3 &axis, float angle)
	{
		Vector3 axisn = axis;
		axisn.normalize();
		float cosin = cos(angle);
		float sinn = sin(angle);

		m00 = cosin + (1 - cosin) * axisn.x * axisn.x;
		m11 = cosin + (1 - cosin) * axisn.y * axisn.y;
		m22 = cosin + (1 - cosin) * axisn.z * axisn.z;

		m10 = (1 - cosin) * axisn.x * axisn.y + sinn * axisn.z;
		m20 = (1 - cosin) * axisn.z * axisn.x - sinn * axisn.y;

		m01 = (1 - cosin) * axisn.x * axisn.y - sinn * axisn.z;
		m21 = (1 - cosin) * axisn.z * axisn.y + sinn * axisn.x;

		m02 = (1 - cosin) * axisn.z * axisn.x + sinn * axisn.y;
		m12 = (1 - cosin) * axisn.z * axisn.y - sinn * axisn.x;
		
	}
}