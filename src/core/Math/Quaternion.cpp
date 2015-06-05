#include "Quaternion.h"
#include "Matrix3.h" 

namespace Engine
{
	Quaternion::Quaternion(const Matrix3 &m)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".

        /*float fTrace = m[0][0]+m[1][1]+m[2][2];
        float fRoot;

        if ( fTrace > 0.0 )
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = Math::sqrtf_fast(fTrace + 1.0f);  // 2w
            w = 0.5f*fRoot;
            fRoot = 0.5f/fRoot;  // 1/(4w)
            x = (m[2][1]-m[1][2])*fRoot;
            y = (m[0][2]-m[2][0])*fRoot;
            z = (m[1][0]-m[0][1])*fRoot;
        }
        else
        {
            // |w| <= 1/2
            static size_t s_iNext[3] = { 1, 2, 0 };
            size_t i = 0;
            if ( m[1][1] > m[0][0] )
                i = 1;
            if ( m[2][2] > m[i][i] )
                i = 2;
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            fRoot = Math::sqrtf_fast(m[i][i]-m[j][j]-m[k][k] + 1.0f);
            float* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = 0.5f*fRoot;
            fRoot = 0.5f/fRoot;
            w = (m[k][j]-m[j][k])*fRoot;
            *apkQuat[j] = (m[j][i]+m[i][j])*fRoot;
            *apkQuat[k] = (m[k][i]+m[i][k])*fRoot;
        }*/
	}

	Vector3 Quaternion::xAxis(void) const
    {
        //float fTx  = 2.0*x;
        float fTy  = 2.0f*y;
        float fTz  = 2.0f*z;
        float fTwy = fTy*w;
        float fTwz = fTz*w;
        float fTxy = fTy*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTzz = fTz*z;

        return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
    }
    //-----------------------------------------------------------------------
    Vector3 Quaternion::yAxis(void) const
    {
        float fTx  = 2.0f*x;
        float fTy  = 2.0f*y;
        float fTz  = 2.0f*z;
        float fTwx = fTx*w;
        float fTwz = fTz*w;
        float fTxx = fTx*x;
        float fTxy = fTy*x;
        float fTyz = fTz*y;
        float fTzz = fTz*z;

        return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
    }
    //-----------------------------------------------------------------------
    Vector3 Quaternion::zAxis(void) const
    {
        float fTx  = 2.0f*x;
        float fTy  = 2.0f*y;
        float fTz  = 2.0f*z;
        float fTwx = fTx*w;
        float fTwy = fTy*w;
        float fTxx = fTx*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTyz = fTz*y;

        return Vector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
    }


	Matrix3 Quaternion::getRotationMatrix()
	{
		Matrix3 result;

		float fTx  = x+x;
        float fTy  = y+y;
        float fTz  = z+z;
        float fTwx = fTx*w;
        float fTwy = fTy*w;
        float fTwz = fTz*w;
        float fTxx = fTx*x;
        float fTxy = fTy*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTyz = fTz*y;
        float fTzz = fTz*z;

        /*result[0][0] = 1.0f-(fTyy+fTzz);
        result[0][1] = fTxy-fTwz;
        result[0][2] = fTxz+fTwy;
        result[1][0] = fTxy+fTwz;
        result[1][1] = 1.0f-(fTxx+fTzz);
        result[1][2] = fTyz-fTwx;
        result[2][0] = fTxz-fTwy;
        result[2][1] = fTyz+fTwx;
        result[2][2] = 1.0f-(fTxx+fTyy);
		*/
		return result;
	}

    //-----------------------------------------------------------------------
    void Quaternion::toAngleAxis(float& angle, Vector3& axis) const
    {
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        float fSqrLength = x*x+y*y+z*z;
        if ( fSqrLength > 0.0 )
        {
            angle = 2.0f*acos(w);
            float fInvLength = Math::sqrtf_fast(fSqrLength);
            axis.x = x*fInvLength;
            axis.y = y*fInvLength;
            axis.z = z*fInvLength;
        }
        else
        {
            // angle is 0 (mod 2*pi), so any axis will do
            angle = 0.0;
            axis.x = 1.0;
            axis.y = 0.0;
            axis.z = 0.0;
        }
    }

}