#include "Camera.h"

#include "../Math/TransformUtils.h"
#include <iostream>

using namespace Engine;
namespace Graphics
{
	Vector3 ACamera::UP = Vector3(0,1,0);

	ACamera::ACamera(void) 
	{ 
		Znear = 0.1f;
		Zfar  = 1000;
	}


	ACamera::~ACamera(void)
	{
	}

	void ACamera::SetupProjection(const float fovy, const float aspRatio, const float nr, const float fr) {
		P = Math::perspectiveMatrix(fovy, aspRatio, nr, fr); 
		Znear = nr;
		Zfar = fr;
		fov = fovy;
		aspect_ratio = aspRatio; 
	} 

	const Matrix4 ACamera::GetViewMatrix() const {
		return V;
	}

	const Matrix4 ACamera::GetProjectionMatrix() const {
		return P;
	}

	const Vector3 ACamera::GetPosition() const {
		return position;
	}

	void ACamera::SetPosition(const Vector3& p) {
		position = p;
	}
  
	const float ACamera::GetFOV() const {
		return fov;
	} 
	void ACamera::SetFOV(const float fovInDegrees) {
		fov = fovInDegrees;
		P = Math::perspectiveMatrix(fovInDegrees, aspect_ratio, Znear, Zfar); 
	}
	const float ACamera::GetAspectRatio() const {
		return aspect_ratio;
	}

	 void ACamera::CalcFrustumPlanes() {
 	

		Vector3 cN = position + look*Znear;
		Vector3 cF = position + look*Zfar; 

		float Hnear = 2.0f * tan(Math::toRadians(fov / 2.0f) ) * Znear;
		float Wnear = Hnear * aspect_ratio;
		float Hfar = 2.0f * tan(Math::toRadians(fov / 2.0f)) * Zfar;
		float Wfar = Hfar * aspect_ratio;
		float hHnear = Hnear/2.0f;
		float hWnear = Wnear/2.0f;
		float hHfar = Hfar/2.0f;
		float hWfar = Wfar/2.0f;


		farPts[0] = cF + up*hHfar - right*hWfar;
		farPts[1] = cF - up*hHfar - right*hWfar;
		farPts[2] = cF - up*hHfar + right*hWfar;
		farPts[3] = cF + up*hHfar + right*hWfar;

		nearPts[0] = cN + up*hHnear - right*hWnear;
		nearPts[1] = cN - up*hHnear - right*hWnear;
		nearPts[2] = cN - up*hHnear + right*hWnear;
		nearPts[3] = cN + up*hHnear + right*hWnear;

		planes[0] = CPlane::FromPoints(nearPts[3],nearPts[0],farPts[0]);
		planes[1] = CPlane::FromPoints(nearPts[1],nearPts[2],farPts[2]);
		planes[2] = CPlane::FromPoints(nearPts[0],nearPts[1],farPts[1]);
		planes[3] = CPlane::FromPoints(nearPts[2],nearPts[3],farPts[2]);
		planes[4] = CPlane::FromPoints(nearPts[0],nearPts[3],nearPts[2]);
		planes[5] = CPlane::FromPoints(farPts[3] ,farPts[0] ,farPts[1]);
	 }

	bool ACamera::IsPointInFrustum(const Vector3& point) {
		for(int i=0; i < 6; i++) 
		{
			if (planes[i].GetDistance(point) < 0)
				return false;
		}
		return true;
	}
 
	 bool ACamera::IsSphereInFrustum(const Vector3& center, const float radius) {
		for(int i=0; i < 6; i++) 
		{
			float d = planes[i].GetDistance(center);
			if ( d < -radius)
				return false;	
		}
		return true;
	 }


	  bool ACamera::IsBoxInFrustum(const Vector3& min, const Vector3& max) {
		for(int i=0; i < 6; i++) 
		{
			Vector3 p=min, n=max;
			Vector3 N = planes[i].N;
			if(N.x>=0) {
				p.x = max.x;
				n.x = min.x;
			}
			if(N.y>=0) {
				p.y = max.y;
				n.y = min.y;
			}
			if(N.z>=0) {
				p.z = max.z;
				n.z = min.z;
			}

			if ( planes[i].GetDistance(p) < 0 ) {
				return false;
			} 
		}

		return true;
	  }

	void ACamera::GetFrustumPlanes(Vector4 fp[6]) {
		for(int i=0;i<6;i++) 
			fp[i]=Vector4(planes[i].N.x, planes[i].N.y, planes[i].N.z, planes[i].d);	
	}

	void ACamera::Rotate(const float y, const float p, const float r) {
		yaw=Math::toRadians(y);
		pitch=Math::toRadians(p);
		roll=Math::toRadians(r);
		update();
	}
}