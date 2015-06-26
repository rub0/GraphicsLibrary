#include "FreeCamera.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix4.h"
#include "../Math/TransformUtils.h"

#include "../../../Dependencies/glm/glm/gtx/euler_angles.hpp"

using namespace Engine;
namespace Graphics
{
	CFreeCamera::CFreeCamera()
	{
		_translation =Vector3(0, 0, 0); 
		_speed = 0.5f; // 0.5 m/s
	}


	CFreeCamera::~CFreeCamera(void)
	{
	}
 
	void CFreeCamera::update() {
		Matrix4 R = Math::yawPitchRoll(yaw,pitch,roll); 
		position+=_translation;

		//set this when no movement decay is needed
		//_translation=Vector3(0); 
		Vector4 u = Vector4(0,0,1,0)*R;
		look = Vector3(u.x, u.y, u.z);	
		u = Vector4(0,1,0,0)*R;
		up   = Vector3(u.x, u.y, u.z);	
		right = look.crossProduct(up);

		Vector3 tgt  = position+look;
		V = Math::lookAt(position, tgt, up); 
	}

	void CFreeCamera::Walk(const float dt) {
		_translation += (look*_speed*dt);
		update();
	}

	void CFreeCamera::Strafe(const float dt) {
		_translation += (right*_speed*dt);
		update();
	}

	void CFreeCamera::Lift(const float dt) {
		_translation += (up*_speed*dt);
		update();
	}
 
	void CFreeCamera::setTranslation(const Vector3& t) {
		_translation = t;
		update();
	}

	Vector3 CFreeCamera::getTranslation() const {
		return _translation;
	}

	void CFreeCamera::setSpeed(const float s) {
		_speed = s;
	}

	const float CFreeCamera::getSpeed() const {
		return _speed;
	}
}