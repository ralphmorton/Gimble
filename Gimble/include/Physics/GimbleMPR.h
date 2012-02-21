
#ifndef _GIMBLE_MPR_
#define _GIMBLE_MPR_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API MPR
		{
		private:
			CollisionWorld *mWorld;
			int mMaxIterations;
			float mTolerance;

		public:
			MPR(CollisionWorld *world, int maxIterations, float tolerance);

			bool collide(CollisionObject *a, CollisionObject *b);
		};
	}
}

#endif