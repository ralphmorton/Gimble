
#ifndef _GIMBLE_LOG_
#define _GIMBLE_LOG_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"

namespace Gimble
{
	class GIMBLE_API Log : public Singleton<Log>
	{
	private:
		std::vector<LogDevice *> mDevices;

	public:
		Log();
		~Log();

		void addDevice(LogDevice *device);
		void removeDevice(LogDevice *device);

		void info(string message);
		void warning(string message);
		void error(string message);
	};
}

#endif