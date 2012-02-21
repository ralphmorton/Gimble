
#ifndef _GIMBLE_LOGDEVICE_
#define _GIMBLE_LOGDEVICE_

#include "GimbleReqs.h"

namespace Gimble
{
	class GIMBLE_API LogDevice
	{
	public:
		virtual ~LogDevice(){}

		virtual void info(string message) = 0;
		virtual void warning(string message) = 0;
		virtual void error(string message) = 0;
	};
}

#endif