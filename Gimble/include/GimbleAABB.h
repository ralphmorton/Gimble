
#ifndef _GIMBLE_AABB_
#define _GIMBLE_AABB_

#include "GimbleReqs.h"
#include "GimbleVector3.h"

namespace Gimble
{
	class GIMBLE_API AABB
	{
	public:
		Vector3 min;
		Vector3 max;

	public:
		AABB() : min(Vector3::ZERO), max(Vector3::ZERO) { }
		AABB(const Vector3 &minimum, const Vector3 &maximum) : min(minimum), max(maximum) { }

		bool intersects(const AABB &other) const
		{
			if (
				other.max.x < min.x || 
				other.max.y < min.y || 
				other.max.z < min.z || 
				max.x < other.min.x || 
				max.y < other.min.y || 
				max.z < other.min.z)
				return false;

			return true;
		}
	};
}

#endif