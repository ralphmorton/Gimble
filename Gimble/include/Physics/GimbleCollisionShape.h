
#ifndef _GIMBLE_COLLISIONSHAPE_
#define _GIMBLE_COLLISIONSHAPE_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API CollisionShape
		{
		public:
			virtual Matrix createInertiaTensor(const Vector3 &scale, float mass) = 0;
			virtual Vector3 supportMap(const Vector3 &direction) = 0;
		};
	}
}

#endif