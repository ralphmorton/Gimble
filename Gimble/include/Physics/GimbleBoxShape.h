
#ifndef _GIMBLE_BOXSHAPE_
#define _GIMBLE_BOXSHAPE_

#include "GimbleReqs.h"
#include "Physics/GimbleCollisionShape.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API BoxShape : public CollisionShape
		{
		public:
			Matrix createInertiaTensor(const Vector3 &scale, float mass);
			Vector3 supportMap(const Vector3 &direction);
		};
	}
}

#endif