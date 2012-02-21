
#ifndef _GIMBLE_QUATERNION_
#define _GIMBLE_QUATERNION_

#include "GimbleMath.h"
#include "GimbleVector3.h"

namespace Gimble
{
	class GIMBLE_API Quaternion
	{
	public:
		float x,y,z,w;

	public:
		inline Quaternion(float fW = 1.0, float fX = 0.0, float fY = 0.0, float fZ = 0.0) : w(fW), x(fX), y(fY), z(fZ) {}
        inline Quaternion(const Vector3& axis, const float& angle)
        {
            float fHalfAngle(0.5f*angle);
	        float fSin = Math::Sin(fHalfAngle);
	        w = Math::Cos(fHalfAngle);
	        x = fSin*axis.x;
	        y = fSin*axis.y;
	        z = fSin*axis.z;
        }

		inline Quaternion operator +(const Quaternion& rkQ) const
		{
			return Quaternion(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
		}

        inline Quaternion operator -(const Quaternion& rkQ) const
		{
			return Quaternion(w-rkQ.w,x-rkQ.x,y-rkQ.y,z-rkQ.z);
		}

        inline Quaternion operator *(const Quaternion& rkQ) const
		{
			return Quaternion
	        (
	            w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
	            w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
	            w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
	            w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
	        );
		}

        inline Quaternion operator *(float fScalar) const
		{
			return Quaternion(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
		}

        inline GIMBLE_API friend Quaternion operator *(float fScalar, const Quaternion& rkQ)
		{
			return Quaternion(fScalar*rkQ.w,fScalar*rkQ.x,fScalar*rkQ.y,fScalar*rkQ.z);
		}

        inline Quaternion operator -() const
		{
			return Quaternion(-w,-x,-y,-z);
		}

        inline bool operator ==(const Quaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
		}

        inline bool operator !=(const Quaternion& rhs) const
		{
			return !operator==(rhs);
		}

        inline Quaternion& operator =(const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}

        inline Vector3 xAxis() const
		{
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

        inline Vector3 yAxis() const
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

        inline Vector3 zAxis() const
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

		inline float dot(const Quaternion& rkQ) const
		{
			return w*rkQ.w+x*rkQ.x+y*rkQ.y+z*rkQ.z;
		}
        
		inline void toAxisAngle(Vector3& axis, float& angle) const
		{
			float fSqrLength = x*x+y*y+z*z;
	        if ( fSqrLength > 0.0 )
	        {
	            angle = 2.0f*Math::ACos(w);
	            float fInvLength = 1.0f / Math::Sqrt(fSqrLength);
	            axis.x = x*fInvLength;
	            axis.y = y*fInvLength;
	            axis.z = z*fInvLength;
	        }
	        else
	        {
	            angle = 0.0f;
	            axis.x = 1.0f;
	            axis.y = 0.0f;
	            axis.z = 0.0f;
	        }
		}
        
        inline float normalise()
		{
			float len = w*w+x*x+y*y+z*z;
	        float factor = 1.0f / Math::Sqrt(len);
	        *this = *this * factor;
	        return len;
		}

        inline Quaternion inverse() const
		{
			float fNorm = w*w+x*x+y*y+z*z;
	        if ( fNorm > 0.0 )
	        {
	            float fInvNorm = 1.0f/fNorm;
	            return Quaternion(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);
	        }
	        else
	            return ZERO;
		}

		inline Quaternion conjugate() const
		{
			return Quaternion(w, -x, -y, -z);
		}
        
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

        inline GIMBLE_API friend std::ostream& operator <<(std::ostream& o, const Quaternion& q)
        {
            o << "Quaternion[" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << "]";
            return o;
        }

        static const Quaternion ZERO;
        static const Quaternion IDENTITY;
	};

	inline Vector3 operator *(const Vector3& v, const Quaternion& q)
    {
        Vector3 uv, uuv;
		Vector3 qvec(q.x, q.y, q.z);
		uv = qvec.cross(v);
		uuv = qvec.cross(uv);
		uv *= (2.0f * q.w);
		uuv *= 2.0f;

		return v + uv + uuv;
    }
}

#endif