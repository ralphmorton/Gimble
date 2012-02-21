
#ifndef _GIMBLE_VECTOR2_
#define _GIMBLE_VECTOR2_

#include "GimbleMath.h"

namespace Gimble
{
	class GIMBLE_API Vector2
	{
	public:
		float x,y;

	public:
		inline Vector2() : x(0), y(0) {}
		inline Vector2(float fx, float fy) : x(fx), y(fy) {}

		inline float operator [](const uint i) const
        {
            assert(i < 2);
            return *(&x+i);
        }

		inline float& operator [](const uint i)
        {
            assert(i < 2);
            return *(&x+i);
        }

		inline Vector2& operator =(const Vector2& rkVector)
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }

		inline Vector2& operator =(const float fScalar)
		{
			x = fScalar;
			y = fScalar;

			return *this;
		}

        inline bool operator == (const Vector2& rkVector) const
        {
            return (x == rkVector.x && y == rkVector.y);
        }

        inline bool operator !=(const Vector2& rkVector) const
        {
            return (x != rkVector.x || y != rkVector.y );
        }

        inline Vector2 operator +(const Vector2& rkVector) const
        {
            return Vector2(x + rkVector.x, y + rkVector.y);
        }

        inline Vector2 operator -(const Vector2& rkVector) const
        {
            return Vector2(x - rkVector.x, y - rkVector.y);
        }

        inline Vector2 operator *(const float fScalar) const
        {
            return Vector2(x * fScalar, y * fScalar);
        }

        inline Vector2 operator *(const Vector2& rhs) const
        {
            return Vector2(x * rhs.x, y * rhs.y);
        }

        inline Vector2 operator /(const float fScalar) const
        {
            assert(fScalar != 0.0);

            float fInv = 1.0f / fScalar;

            return Vector2(x * fInv, y * fInv);
        }

        inline Vector2 operator /(const Vector2& rhs) const
        {
            return Vector2(x / rhs.x, y / rhs.y);
        }

        inline const Vector2& operator +() const
        {
            return *this;
        }

        inline Vector2 operator -() const
        {
            return Vector2(-x, -y);
        }

        inline friend Vector2 operator *(const float fScalar, const Vector2& rkVector)
        {
            return Vector2(fScalar * rkVector.x, fScalar * rkVector.y);
        }

        inline friend Vector2 operator /(const float fScalar, const Vector2& rkVector)
        {
            return Vector2(fScalar / rkVector.x, fScalar / rkVector.y);
        }

        inline friend Vector2 operator +(const Vector2& lhs, const float rhs)
        {
            return Vector2(lhs.x + rhs, lhs.y + rhs);
        }

        inline friend Vector2 operator +(const float lhs, const Vector2& rhs)
        {
            return Vector2(lhs + rhs.x, lhs + rhs.y);
        }

        inline friend Vector2 operator -(const Vector2& lhs, const float rhs)
        {
            return Vector2(lhs.x - rhs, lhs.y - rhs);
        }

        inline friend Vector2 operator -(const float lhs, const Vector2& rhs)
        {
            return Vector2(lhs - rhs.x, lhs - rhs.y);
        }

        inline Vector2& operator +=(const Vector2& rkVector)
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }

        inline Vector2& operator +=(const float fScaler)
        {
            x += fScaler;
            y += fScaler;

            return *this;
        }

        inline Vector2& operator -=( const Vector2& rkVector)
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline Vector2& operator -=( const float fScaler)
        {
            x -= fScaler;
            y -= fScaler;

            return *this;
        }

        inline Vector2& operator *=(const float fScalar)
        {
            x *= fScalar;
            y *= fScalar;

            return *this;
        }

        inline Vector2& operator *=(const Vector2& rkVector)
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline Vector2& operator /=(const float fScalar)
        {
            assert(fScalar != 0.0);

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline Vector2& operator /=(const Vector2& rkVector)
        {
            x /= rkVector.x;
            y /= rkVector.y;

            return *this;
        }

        inline float length () const
        {
			return Math::Sqrt(x * x + y * y);
        }

        inline float lengthSquared() const
        {
            return x * x + y * y;
        }

        inline float distance(const Vector2& rhs) const
        {
            return (*this - rhs).length();
        }

        inline float distanceSquared(const Vector2& rhs) const
        {
            return (*this - rhs).lengthSquared();
        }

        inline float dot(const Vector2& vec) const
        {
            return x * vec.x + y * vec.y;
        }

        inline float normalise()
        {
            float fLength = Math::Sqrt(x * x + y * y);

            if (fLength > 1e-08)
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
            }

            return fLength;
        }

        inline Vector2 midPoint(const Vector2& vec) const
        {
            return Vector2((x + vec.x) * 0.5f, (y + vec.y) * 0.5f);
        }

        inline bool operator <(const Vector2& rhs) const
        {
            if(x < rhs.x && y < rhs.y)
                return true;
            return false;
        }

        inline bool operator > (const Vector2& rhs) const
        {
            if( x > rhs.x && y > rhs.y )
                return true;
            return false;
        }

        inline Vector2 perpendicular(void) const
        {
            return Vector2 (-y, x);
        }
        
        inline float crossProduct(const Vector2& rkVector) const
        {
            return x * rkVector.y - y * rkVector.x;
        }

        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (1e-06 * 1e-06));
        }

        inline Vector2 normalisedCopy(void) const
        {
            Vector2 ret = *this;
            ret.normalise();
            return ret;
        }

        inline Vector2 reflect(const Vector2& normal) const
        {
            return Vector2(*this - (2 * this->dot(normal) * normal));
        }
		
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y);
		}

        static const Vector2 ZERO;
		static const Vector2 ONE;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;

        inline GIMBLE_API friend std::ostream& operator <<(std::ostream& o, const Vector2& v)
        {
            o << "Vector2[" << v.x << ", " << v.y <<  "]";
            return o;
        }
	};
}

#endif