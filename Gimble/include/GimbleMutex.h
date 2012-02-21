
#ifndef _GIMBLE_MUTEX_
#define _GIMBLE_MUTEX_

#include "GimbleReqs.h"
#include <windows.h>

namespace Gimble
{
	class GIMBLE_API Mutex
	{
	private:
		CRITICAL_SECTION mSection;

	public:
		Mutex();
		~Mutex();

		inline void lock()
		{
			EnterCriticalSection(&mSection);
		}

		inline void unlock()
		{
			LeaveCriticalSection(&mSection);
		}

		inline bool tryLock()
		{
			return TryEnterCriticalSection(&mSection)!=0;
		}
	};
}

#endif