#pragma once

#ifndef PLANE_H
#define PLANE_H

#include "../Math/Vector3.h"
#include "../Math/Matrix4.h"
using namespace Engine;
class CPlane
{
	enum Where {COPLANAR, FRONT, BACK};

public:
	CPlane(void);
	CPlane(const Vector3& N, const Vector3& p);
	~CPlane(void);

	static CPlane FromPoints(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	Where Classify(const Vector3& p);
	float GetDistance(const Vector3& p);


	Vector3 N;
	float d;
};
#endif
