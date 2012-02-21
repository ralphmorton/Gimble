
#include "GimbleMutex.h"

namespace Gimble
{
	Mutex::Mutex()
	{
		InitializeCriticalSection(&mSection);
	}

	Mutex::~Mutex()
	{
		DeleteCriticalSection(&mSection);
	}
}