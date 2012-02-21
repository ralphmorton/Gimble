
#ifndef _GIMBLE_SPHERESHAPE_
#define _GIMBLE_SPHERESHAPE_

#include "GimbleReqs.h"
#include "Physics/GimbleCollisionShape.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API SphereShape : public CollisionShape
		{
		public:
			Matrix createInertiaTensor(const Vector3 &scale, float mass);
			Vector3 supportMap(const Vector3 &direction);
		};
	}
}

#endif