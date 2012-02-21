
#include "GimbleThread.h"

namespace Gimble
{
	Thread::Thread(uint stackSize) : mThread(0), mStackSize(stackSize), mRunning(false), mComplete(false)
	{
		
	}

	Thread::~Thread()
	{
		if (mThread)
		{
			CloseHandle(mThread);
			mThread = 0;
		}
	}

	DWORD WINAPI Thread::beginRun(void *state)
	{
		DWORD ret = ((Thread*)state)->run();

		((Thread*)state)->mRunning = false;
		((Thread*)state)->mComplete = true;

		return ret;
	}

	void Thread::start()
	{
		if (mRunning)
			return;

		if (mThread)
		{
			CloseHandle(mThread);
			mThread = 0;
		}

		mRunning = true;
		mComplete = false;
		mThread = CreateThread(0, mStackSize, beginRun, this, 0, &mID);
	}

	void Thread::wait(uint millis) const
	{
		WaitForSingleObject(mThread, millis);
	}
}