
#ifndef _GIMBLE_BROADPHASE_
#define _GIMBLE_BROADPHASE_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API Broadphase
		{
		public:
			struct BroadphasePair
			{
				uint IDA, IDB;
				CollisionObject *A;
				CollisionObject *B;

				BroadphasePair() : IDA(0), IDB(0), A(0), B(0) { }
			};

		public:
			virtual GIMBLE_HANDLE add(CollisionObject *o) = 0;
			virtual void remove(GIMBLE_HANDLE o) = 0;
			virtual void update(GIMBLE_HANDLE o) = 0;

			virtual const BroadphasePair *getOverlappingPairs() = 0;
			virtual uint getNumOverlappingPairs() = 0;
		};
	}
}

#endif