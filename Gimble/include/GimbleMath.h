
#ifndef _GIMBLE_MATH_
#define _GIMBLE_MATH_

#include "GimbleReqs.h"

namespace Gimble
{
	class GIMBLE_API Math
	{
	public:
		static const float PI;
        static const float TWO_PI;
        static const float HALF_PI;

	public:
		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			assert(minval <= maxval && "Invalid clamp range");
			return std::max(std::min(val, maxval), minval);
		}

		template <typename T> 
		static T Min(T a, T b)
		{
			return a<b?a:b;
		}

		template <typename T> 
		static T Max(T a, T b)
		{
			return a>b?a:b;
		}

		static inline bool floatEqual(float a, float b, float tolerance) { return fabs(b-a) <= tolerance; }

		static inline bool isNaN(float f) { return f != f; }

		static inline float Abs(float val) { return fabs(val); }
		static inline float Sqrt(float val) { return sqrt(val); }
		static inline float Pow(float val, float exp) { return pow(val, exp); }

		static inline float Cos(float val) { return cos(val); }
		static inline float Sin(float val) { return sin(val); }
		static inline float Tan(float val) { return tan(val); }

		static inline float ACos(float val) { return acos(val); }
		static inline float ASin(float val) { return asin(val); }
		static inline float ATan(float val) { return atan(val); }
	};
}

#endif