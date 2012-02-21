
#ifndef _GIMBLE_THREAD_
#define _GIMBLE_THREAD_

#include "GimbleReqs.h"
#include <windows.h>

namespace Gimble
{
	class GIMBLE_API Thread
	{
	private:
		HANDLE mThread;
		DWORD mID;
		uint mStackSize;
		bool mRunning;
		bool mComplete;

	private:
		static DWORD WINAPI beginRun(void *state);

	protected:
		virtual DWORD run() = 0;

	public:
		Thread(uint stackSize = 0);
		virtual ~Thread();

		void start();
		void wait(uint millis = INFINITE) const;

		inline bool running() const { return mRunning; }
		inline bool complete() const { return mComplete; }
	};
}

#endif