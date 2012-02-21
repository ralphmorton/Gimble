
#include "Physics/GimbleBoxShape.h"
#include "GimbleMatrix.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Physics
	{
		Matrix BoxShape::createInertiaTensor(const Vector3 &scale, float mass)
		{
			Matrix it = Matrix::IDENTITY;

            it.m[0][0] = (1.0f / 12.0f) * mass * (scale.y * scale.y + scale.z * scale.z);
            it.m[1][1] = (1.0f / 12.0f) * mass * (scale.x * scale.x + scale.z * scale.z);
            it.m[2][2] = (1.0f / 12.0f) * mass * (scale.x * scale.x + scale.y * scale.y);

            return it;
		}

		Vector3 BoxShape::supportMap(const Vector3 &direction)
		{
			Vector3 support = Vector3::ZERO;
            support.x = (direction.x > 0.f) ? 0.5f : -0.5f;
            support.y = (direction.y > 0.f) ? 0.5f : -0.5f;
            support.z = (direction.z > 0.f) ? 0.5f : -0.5f;
            return support;
		}
	}
}