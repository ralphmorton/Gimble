
#ifndef _GIMBLE_COLOR_
#define _GIMBLE_COLOR_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class Color
		{
		public:
			float r,g,b,a;

		public:
			inline Color() : r(0), g(0), b(0), a(1){}
			inline Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha){}
		};
	}
}

#endif