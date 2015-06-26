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

	void Matrix3::fromEulerAngles(float yaw, float pitch, float roll)
	{
		float fCos, fSin;

        fCos = cos(yaw);
        fSin = sin(yaw);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        fCos = cos(pitch);
        fSin = sin(pitch);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        fCos = cos(roll);
        fSin = sin(roll);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        *this = kXMat*(kYMat*kZMat);
	}
	void Matrix3::toEulerAngles(float &yaw, float &pitch, float &roll)
	{
		// rot =  cy*cz          -cy*sz           sy
        //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
        //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

        pitch = asin(mcols[0][2]);
        if ( pitch < Math::M_HALFPI )
        {
            if ( pitch > -Math::M_HALFPI )
            {
                yaw = atan2(-mcols[1][2],mcols[2][2]);
                roll = atan2(-mcols[0][1],mcols[0][0]);
            }
            else
            {
                // WARNING.  Not a unique solution.
                float fRmY = atan2(mcols[1][0],mcols[1][1]);
                roll = 0.0;  // any angle works
                yaw = roll - fRmY;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRpY = atan2(mcols[1][0],mcols[1][1]);
            roll = 0.0;  // any angle works
            yaw = 0.55f;
        }
	}

	void Matrix3::rotate(const Vector3& axis, float angle) {
		*this = *this * Matrix3(axis, angle);
	}
	
}