
#ifndef _GIMBLE_TIMER_
#define _GIMBLE_TIMER_

#include "GimbleReqs.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <ctime>

namespace Gimble
{
	class GIMBLE_API Timer
	{
	private:
		clock_t mZeroClock;
        DWORD mStartTick;
		LONGLONG mLastTime;
        LARGE_INTEGER mStartTime;
        LARGE_INTEGER mFrequency;
		DWORD_PTR mTimerMask;

	public:
		Timer();
		~Timer();

		void reset();

		unsigned long getMillis();
		unsigned long getMicros();
	};
}

#endif