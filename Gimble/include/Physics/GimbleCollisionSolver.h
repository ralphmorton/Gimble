
#ifndef _GIMBLE_COLLISIONSOLVER_
#define _GIMBLE_COLLISIONSOLVER_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Physics
	{
		class CollisionSolver
		{
		private:
			Simulator *mSimulator;

		public:
			CollisionSolver();

			void prep(float dt, const std::vector<ContactManifold*> &manifolds);
			void step(const std::vector<ContactManifold*> &manifolds);
		};
	}
}

#endif