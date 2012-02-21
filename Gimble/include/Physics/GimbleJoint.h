
#ifndef _GIMBLE_JOINT_
#define _GIMBLE_JOINT_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API Joint
		{
		public:
			virtual void prep(float dt) = 0;
			virtual void solve(float dt) = 0;
		};
	}
}

#endif