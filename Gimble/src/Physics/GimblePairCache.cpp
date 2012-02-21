
#include "Physics/GimblePairCache.h"

namespace Gimble
{
	namespace Physics
	{
		PairCache::PairCache() : mHashSize(0), mMask(0), mHashTable(0), mNext(0), mNumPairs(0), mPairs(0)
		{

		}

		PairCache::~PairCache()
		{
			clear();
		}

		Broadphase::BroadphasePair *PairCache::find(uint ida, uint idb)
		{
			if (!mHashTable) return 0;

			if (ida>idb)
			{
				int idt = ida;
				ida = idb;
				idb = idt;
			}

			const uint hvalue = hash(ida, idb) & mMask;

			uint offset = mHashTable[hvalue];
			while(offset != 0xffffffff && (mPairs[offset].IDA != ida || mPairs[offset].IDB != idb))
			{
				assert(mPairs[offset].IDA != 0xffffffff);
				offset = mNext[offset];
			}

			if(offset == 0xffffffff) return 0;

			assert(offset<mNumPairs);

			return &mPairs[offset];
		}

		Broadphase::BroadphasePair *PairCache::add(uint ida, uint idb)
		{
			if (ida>idb)
			{
				int idt = ida;
				ida = idb;
				idb = idt;
			}

			uint hvalue = hash(ida, idb) & mMask;

			Broadphase::BroadphasePair *pair = find(ida, idb);
			if (pair) return pair;

			if (mNumPairs >= mHashSize)
			{
				mHashSize = nextPowerOfTwo(mNumPairs+1);
				mMask = mHashSize - 1;

				delete [] mHashTable;
				mHashTable = new uint[mHashSize];

				for(uint i = -1; ++i < mHashSize;) mHashTable[i] = 0xffffffff;

				Broadphase::BroadphasePair *newpairs = new Broadphase::BroadphasePair[mHashSize];
				uint *newnext = new uint[mHashSize];

				if (mNumPairs) memcpy(newpairs, mPairs, mNumPairs * sizeof(Broadphase::BroadphasePair));

				for (uint i = -1; ++i < mNumPairs;)
				{
					uint hval = hash(mPairs[i].IDA, mPairs[i].IDB) & mMask;
					newnext[i] = mHashTable[hval];
					mHashTable[hval] = i;
				}

				delete [] mNext;
				delete [] mPairs;

				mPairs = newpairs;
				mNext = newnext;

				hvalue = hash(ida, idb) & mMask;
			}

			Broadphase::BroadphasePair *p = &mPairs[mNumPairs];
			p->IDA = ida;
			p->IDB = idb;

			mNext[mNumPairs] = mHashTable[hvalue];
			mHashTable[hvalue] = mNumPairs++;

			return p;
		}

		void PairCache::remove(uint ida, uint idb)
		{
			if (ida>idb)
			{
				int idt = ida;
				ida = idb;
				idb = idt;
			}

			const uint hvalue = hash(ida, idb) & mMask;

			const Broadphase::BroadphasePair *pair = find(ida, idb);
			if (!pair) return;

			const uint index = getIndex(pair);
			uint offset = mHashTable[hvalue];
			uint previous = 0xffffffff;

			while (offset != index)
			{
				previous = offset;
				offset = mNext[offset];
			}

			if (previous != 0xffffffff)
				mNext[previous] = mNext[index];
			else
				mHashTable[hvalue] = mNext[index];

			if (1)
			{
				const uint lastindex = mNumPairs - 1;

				if (lastindex == index)
				{
					mNumPairs--;
					return;
				}

				const Broadphase::BroadphasePair* last = &mPairs[lastindex];
				const uint lasthvalue = hash(last->IDA, last->IDB) & mMask;

				uint offset = mHashTable[lasthvalue];

				uint previous = 0xffffffff;
				while(offset != lastindex)
				{
					previous = offset;
					offset = mNext[offset];
				}

				if(previous != 0xffffffff)
					mNext[previous] = mNext[lastindex];
				else 
					mHashTable[lasthvalue] = mNext[lastindex];

				mPairs[index] = mPairs[lastindex];
				mNext[index] = mHashTable[lasthvalue];
				mHashTable[lasthvalue] = index;

				mNumPairs--;
			}
		}

		void PairCache::clear()
		{
			delete [] mHashTable;
			delete [] mNext;
			delete [] mPairs;

			mHashSize = 0;
			mMask = 0;
			mNumPairs = 0;
		}
	}
}