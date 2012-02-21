
#ifndef _GIMBLE_VECTOR4_
#define _GIMBLE_VECTOR4_

#include "GimbleMath.h"
#include "GimbleVector3.h"

namespace Gimble
{
	class GIMBLE_API Vector4
	{
	public:
		float x,y,z,w;

	public:
		inline Vector4() { }
        inline Vector4(const float fX, const float fY, const float fZ, const float fW) : x(fX), y(fY), z(fZ), w(fW) {}
        inline explicit Vector4(const float val) : x(val), y(val), z(val), w(val) {}
        inline explicit Vector4(const Vector3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f) {}

		inline float operator [](const uint i) const
        {
            assert( i < 4 );
            return *(&x+i);
        }

		inline float& operator [](const uint i)
        {
            assert( i < 4 );
            return *(&x+i);
        }

        inline Vector4& operator = ( const Vector4& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;
            w = rkVector.w;

            return *this;
        }

		inline Vector4& operator = ( const float fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return *this;
		}

        inline bool operator == ( const Vector4& rkVector ) const
        {
            return ( x == rkVector.x &&
                y == rkVector.y &&
                z == rkVector.z &&
                w == rkVector.w );
        }

        inline bool operator != ( const Vector4& rkVector ) const
        {
            return ( x != rkVector.x ||
                y != rkVector.y ||
                z != rkVector.z ||
                w != rkVector.w );
        }

        inline Vector4& operator = (const Vector3& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = 1.0f;
            return *this;
        }

        inline Vector4 operator + ( const Vector4& rkVector ) const
        {
            return Vector4(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z,
                w + rkVector.w);
        }

        inline Vector4 operator - ( const Vector4& rkVector ) const
        {
            return Vector4(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z,
                w - rkVector.w);
        }

        inline Vector4 operator * ( const float fScalar ) const
        {
            return Vector4(
                x * fScalar,
                y * fScalar,
                z * fScalar,
                w * fScalar);
        }

        inline Vector4 operator * ( const Vector4& rhs) const
        {
            return Vector4(
                rhs.x * x,
                rhs.y * y,
                rhs.z * z,
                rhs.w * w);
        }

        inline Vector4 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector4(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline Vector4 operator / ( const Vector4& rhs) const
        {
            return Vector4(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w);
        }

        inline const Vector4& operator + () const
        {
            return *this;
        }

        inline Vector4 operator - () const
        {
            return Vector4(-x, -y, -z, -w);
        }

        inline friend Vector4 operator * ( const float fScalar, const Vector4& rkVector )
        {
            return Vector4(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z,
                fScalar * rkVector.w);
        }

        inline friend Vector4 operator / ( const float fScalar, const Vector4& rkVector )
        {
            return Vector4(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z,
                fScalar / rkVector.w);
        }

        inline friend Vector4 operator + (const Vector4& lhs, const float rhs)
        {
            return Vector4(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs,
                lhs.w + rhs);
        }

        inline friend Vector4 operator + (const float lhs, const Vector4& rhs)
        {
            return Vector4(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z,
                lhs + rhs.w);
        }

        inline friend Vector4 operator - (const Vector4& lhs, float rhs)
        {
            return Vector4(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs,
                lhs.w - rhs);
        }

        inline friend Vector4 operator - (const float lhs, const Vector4& rhs)
        {
            return Vector4(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z,
                lhs - rhs.w);
        }

        inline Vector4& operator += ( const Vector4& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;
            w += rkVector.w;

            return *this;
        }

        inline Vector4& operator -= ( const Vector4& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;
            w -= rkVector.w;

            return *this;
        }

        inline Vector4& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            w *= fScalar;
            return *this;
        }

        inline Vector4& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            w += fScalar;
            return *this;
        }

        inline Vector4& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            w -= fScalar;
            return *this;
        }

        inline Vector4& operator *= ( const Vector4& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;
            w *= rkVector.w;

            return *this;
        }

        inline Vector4& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

        inline Vector4& operator /= ( const Vector4& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;
            w /= rkVector.w;

            return *this;
        }

        inline float dot(const Vector4& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }
		
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}
        
        inline GIMBLE_API friend std::ostream& operator <<(std::ostream& o, const Vector4& v)
        {
            o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
            return o;
        }
        
        static const Vector4 ZERO;
	};
}

#endif