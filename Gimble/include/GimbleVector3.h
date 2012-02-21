
#ifndef _GIMBLE_VECTOR3_
#define _GIMBLE_VECTOR3_

#include "GimbleMath.h"

namespace Gimble
{
	class GIMBLE_API Vector3
	{
	public:
		float x,y,z;

	public:
		inline Vector3() {}
        inline Vector3(const float fX, const float fY, const float fZ) : x( fX ), y( fY ), z( fZ ) {}
        inline explicit Vector3(const float val) : x(val) , y(val) , z(val) {}

		inline float operator [](const uint i) const
        {
            assert( i < 3 );
            return *(&x+i);
        }

		inline float& operator [](const uint i)
        {
            assert( i < 3 );
            return *(&x+i);
        }

        inline Vector3& operator = ( const Vector3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

        inline Vector3& operator = ( const float fScalar )
        {
            x = fScalar;
            y = fScalar;
            z = fScalar;

            return *this;
        }

        inline bool operator == ( const Vector3& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const Vector3& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        inline Vector3 operator + ( const Vector3& rkVector ) const
        {
            return Vector3(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline Vector3 operator - ( const Vector3& rkVector ) const
        {
            return Vector3(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline Vector3 operator * ( const float fScalar ) const
        {
            return Vector3(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline Vector3 operator * ( const Vector3& rhs) const
        {
            return Vector3(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline Vector3 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline Vector3 operator / ( const Vector3& rhs) const
        {
            return Vector3(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const Vector3& operator + () const
        {
            return *this;
        }

        inline Vector3 operator - () const
        {
            return Vector3(-x, -y, -z);
        }

        inline friend Vector3 operator * ( const float fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend Vector3 operator / ( const float fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend Vector3 operator + (const Vector3& lhs, const float rhs)
        {
            return Vector3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend Vector3 operator + (const float lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend Vector3 operator - (const Vector3& lhs, const float rhs)
        {
            return Vector3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend Vector3 operator - (const float lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        inline Vector3& operator += ( const Vector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline Vector3& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline Vector3& operator -= ( const Vector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline Vector3& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const Vector3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline Vector3& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline Vector3& operator /= ( const Vector3& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

            return *this;
        }

        inline float length () const
        {
            return Math::Sqrt( x * x + y * y + z * z );
        }

        inline float lengthSquared () const
        {
            return x * x + y * y + z * z;
        }

        inline float distance(const Vector3& rhs) const
        {
            return (*this - rhs).length();
        }

        inline float distanceSquared(const Vector3& rhs) const
        {
            return (*this - rhs).lengthSquared();
        }

        inline float dot(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        inline float normalise()
        {
            float fLength = Math::Sqrt( x * x + y * y + z * z );

            float fInvLength = 1.0f / fLength;
            x *= fInvLength;
            y *= fInvLength;
            z *= fInvLength;

            return fLength;
        }

        inline Vector3 cross( const Vector3& rkVector ) const
        {
            return Vector3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        inline Vector3 midPoint( const Vector3& vec ) const
        {
            return Vector3(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

        inline bool operator < ( const Vector3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        inline bool operator > ( const Vector3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }

        inline Vector3 perpendicular() const
        {
            static const float fSquareZero = (float)(1e-06 * 1e-06);

            Vector3 perp = this->cross( Vector3::UNIT_X );
            if( perp.lengthSquared() < fSquareZero )
            {
                perp = this->cross(Vector3::UNIT_Y);
            }
			perp.normalise();

            return perp;
        }

		inline float angleBetween(const Vector3& dest)
		{
			float lenProduct = length() * dest.length();

			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			float f = dot(dest) / lenProduct;

			f = Math::Clamp(f, (float)-1.0, (float)1.0);
			return Math::ACos(f);
		}

        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));

        }

        inline Vector3 normalisedCopy(void) const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

        inline Vector3 reflect(const Vector3& normal) const
        {
            return Vector3( *this - ( 2 * this->dot(normal) * normal ) );
        }

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z);
		}

        static const Vector3 ZERO;
		static const Vector3 ONE;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;

        inline GIMBLE_API friend std::ostream& operator <<(std::ostream& o, const Vector3& v)
        {
            o << "Vector3[" << v.x << ", " << v.y << ", " << v.z << "]";
            return o;
        }
	};
}

#endif