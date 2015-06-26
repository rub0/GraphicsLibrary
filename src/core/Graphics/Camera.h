#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Plane.h"

using namespace Engine;
namespace Graphics
{

	class ACamera
	{
	public:

		ACamera();
		~ACamera();
		virtual void update() = 0;

		void SetupProjection(const float fovy, const float aspectRatio, const float near=0.1f, const float far=1000.0f);
	
		virtual void Rotate(const float yaw, const float pitch, const float roll); 

		const Matrix4 GetViewMatrix() const;
		const Matrix4 GetProjectionMatrix() const;

		void SetPosition(const Vector3& v);
		const Vector3 GetPosition() const;
	 

		void SetFOV(const float fov);
		const float GetFOV() const;
		const float GetAspectRatio() const; 
	
	
		void CalcFrustumPlanes();
		bool IsPointInFrustum(const Vector3& point);
		bool IsSphereInFrustum(const Vector3& center, const float radius);
		bool IsBoxInFrustum(const Vector3& min, const Vector3& max);
		void GetFrustumPlanes(Vector4 planes[6]);

		//frustum points
		Vector3 farPts[4];
		Vector3 nearPts[4];

	protected:	 
		float yaw, pitch, roll, fov, aspect_ratio, Znear, Zfar;
		static Vector3 UP;
		Vector3 look;
		Vector3 up;
		Vector3 right; 
		Vector3 position;
		Matrix4 V; //view matrix
		Matrix4 P; //projection matrix

		//Frsutum planes
		CPlane planes[6];
	};
}

#endif