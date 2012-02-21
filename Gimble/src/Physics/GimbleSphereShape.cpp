
#include "Physics/GimbleSphereShape.h"
#include "GimbleMatrix.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Physics
	{
		Matrix SphereShape::createInertiaTensor(const Vector3 &scale, float mass)
		{
			Matrix it = Matrix::IDENTITY;

            it.m[0][0] = 0.4f * (scale.x * scale.x) * mass;
            it.m[1][1] = 0.4f * (scale.y * scale.y) * mass;
            it.m[2][2] = 0.4f * (scale.z * scale.z) * mass;

            return it;
		}

		Vector3 SphereShape::supportMap(const Vector3 &direction)
		{
			Vector3 d = direction;
			d.normalise();

			return d * 0.5f;
		}
	}
}