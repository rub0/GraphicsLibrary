#pragma once
#include "Camera.h"

namespace Graphics
{
	class CFreeCamera :
	public ACamera
	{
	public:
		CFreeCamera(void);
		~CFreeCamera(void);

		void update();
	 
		void Walk(const float dt);
		void Strafe(const float dt);
		void Lift(const float dt);
	  
		void setTranslation(const Vector3& t);
		Vector3 getTranslation() const;

		void setSpeed(const float speed);
		const float getSpeed() const;

	protected:
	
		float _speed;			//move speed of camera in m/s
		Vector3 _translation;
	};

}
