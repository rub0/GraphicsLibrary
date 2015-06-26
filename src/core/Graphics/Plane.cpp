#include "Plane.h"

const float EPSILON= 0.0001f;

CPlane::CPlane(void)
{
	N = Vector3(0,1,0);
	d = 0;
}


CPlane::~CPlane(void)
{
}

CPlane::CPlane(const Vector3& normal, const Vector3& p) {
	N = normal;
	d = -N.dotProduct(p);
}

CPlane CPlane::FromPoints(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
	CPlane temp;
	Vector3 e1 = v2-v1;
	Vector3 e2 = v3-v1;
	temp.N = Vector3::normalize(e1.crossProduct(e2)); 
	temp.d = -temp.N.dotProduct(v1);
	return temp;
}

float CPlane::GetDistance(const Vector3& p) {
	return N.dotProduct(p)+d;
}

CPlane::Where CPlane::Classify(const Vector3& p) {
	float res = GetDistance(p);
	if( res > EPSILON)
		return FRONT;	
	else if(res < EPSILON)
		return BACK;
	else
		return COPLANAR;
}
