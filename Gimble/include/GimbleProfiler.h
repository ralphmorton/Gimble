
#ifndef _GIMBLE_PROFILER_
#define _GIMBLE_PROFILER_

#include "GimbleReqs.h"
#include "GimbleMath.h"
#include "GimbleTimer.h"

namespace Gimble
{
	class GIMBLE_API Profiler
	{
	private:
		Timer mTimer;
		long mT1, mT2;
		long mMin;
		long mMax;
		long mAvg;
		long mCycleCount;
		long mAccum;

	public:
		Profiler() : mT1(0), mT2(0), mMin(2147483647), mMax(-2147483647), mAvg(0), mCycleCount(0), mAccum(0) { mTimer.reset(); }
		~Profiler() { }

		void startCycle()
		{
			mT1 = mTimer.getMicros();
		}

		void endCycle()
		{
			mT2 = mTimer.getMicros();
			long delta = Math::Max(mT2 - mT1, 0L);

			mCycleCount++;
			mAccum += delta;

			if (delta < mMin) mMin = delta;
			if (delta > mMax) mMax = delta;

			mAvg = (long)((double)mAccum / (double)mCycleCount);
		}

		void reset()
		{
			mMin = 2147483647;
			mMax = -2147483647;
			mAvg = 0;
			mCycleCount = 0;
			mAccum = 0;
			mTimer.reset();
		}

		inline long getMin() { return mMin; }
		inline long getMax() { return mMax; }
		inline long getAvg() { return mAvg; }
		inline long getTotal() { return mAccum; }
		inline long getCycleCount() { return mCycleCount; }

		inline GIMBLE_API friend std::ostream& operator <<(std::ostream& o, const Profiler& p)
        {
            o << "Min: " << p.mMin << ", Max: " << p.mMax << ", Avg: " << p.mAvg << ", Total: " << p.mAccum << std::endl;
            return o;
        }
	};
}

#endif