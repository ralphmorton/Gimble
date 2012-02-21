
#include "GimbleTimer.h"

namespace Gimble
{
	Timer::Timer() : mTimerMask(0)
	{
	
	}

	Timer::~Timer()
	{
	
	}

	void Timer::reset()
	{
		DWORD_PTR procMask;
		DWORD_PTR sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		if (procMask == 0)
			procMask = 1;

		if(mTimerMask == 0)
		{
			mTimerMask = 1;
			while((mTimerMask & procMask) == 0)
				mTimerMask <<= 1;
		}

		HANDLE thread = GetCurrentThread();
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mStartTime);
		mStartTick = GetTickCount();
		SetThreadAffinityMask(thread, oldMask);

		mLastTime = 0;
		mZeroClock = clock();
	}

	unsigned long Timer::getMillis()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();
		DWORD oldMask = SetThreadAffinityMask(thread, mTimerMask);
		QueryPerformanceCounter(&curTime);
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;
			newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
		}

		mLastTime = newTime;

		return newTicks;
	}

	unsigned long Timer::getMicros()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
		// get milliseconds to check against GetTickCount
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
    
		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;
		}

		// Record last time for adjust
		mLastTime = newTime;

		// scale by 1000000 for microseconds
		unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

		return newMicro;
	}
}