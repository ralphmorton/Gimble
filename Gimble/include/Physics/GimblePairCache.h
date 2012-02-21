
#ifndef _GIMBLE_PAIRCACHE_
#define _GIMBLE_PAIRCACHE_

#include "GimbleReqs.h"
#include "Physics/GimbleBroadphase.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API PairCache
		{
		private:
			uint *mHashTable;
			uint *mNext;
			Broadphase::BroadphasePair *mPairs;
			uint mHashSize;
			uint mMask;
			uint mNumPairs;

		public:
			PairCache();
			~PairCache();

			Broadphase::BroadphasePair *add(uint ida, uint idb);
			Broadphase::BroadphasePair *find(uint ida, uint idb);
			void remove(uint ida, uint idb);
			void clear();

			inline const Broadphase::BroadphasePair *getPairs() { return mPairs; }
			inline uint getNumPairs() { return mNumPairs; }

			inline uint getIndex(const Broadphase::BroadphasePair *pair)
			{
				return ((uint)((uint(pair) - uint(mPairs))) / sizeof(Broadphase::BroadphasePair));
			}
			
		private:
			inline uint hash(uint key)
			{
				key += ~(key << 15);
				key ^=  (key >> 10);
				key +=  (key << 3);
				key ^=  (key >> 6);
				key += ~(key << 11);
				key ^=  (key >> 16);
				return key;
			}

			inline uint hash(uint id1, uint id2)
			{
				return hash((id1&0xffff)|(id2<<16));
			}

			inline int nextPowerOfTwo(int x)
			{
				x |= (x >> 1);
				x |= (x >> 2);
				x |= (x >> 4);
				x |= (x >> 8);
				x |= (x >> 16);
				return x+1;
			}
		};
	}
}

#endif