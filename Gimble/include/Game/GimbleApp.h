
#ifndef _GIMBLE_APP_
#define _GIMBLE_APP_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Game
	{
		class GIMBLE_API App
		{
		public:
			virtual ~App(){}

			virtual bool init() = 0;
			virtual bool update(float elapsedSeconds) = 0;
			virtual void draw() = 0;
			virtual void shutdown() = 0;
		};
	}
}

#endif