
#include "GimbleLog.h"
#include "GimbleLogDevice.h"

template<> Gimble::Log* Gimble::Singleton<Gimble::Log>::ms_Singleton = 0;

namespace Gimble
{
	Log::Log()
	{
	
	}

	Log::~Log()
	{
		for (int i = -1; ++i < mDevices.size();)
			delete (mDevices[i]);
	}

	void Log::addDevice(LogDevice *device)
	{
		for (int i = -1; ++i < mDevices.size();)
			if (mDevices[i] == device) return; //Already included

		mDevices.push_back(device);
	}

	void Log::removeDevice(LogDevice *device)
	{
		for (std::vector<LogDevice*>::iterator iter = mDevices.begin(); iter != mDevices.end(); iter++)
			if (*(iter) == device)
			{
				mDevices.erase(iter);
				return;
			}
	}

	void Log::info(string message)
	{
		for (int i = -1; ++i < mDevices.size();)
			mDevices[i]->info(message);
	}

	void Log::warning(string message)
	{
		for (int i = -1; ++i < mDevices.size();)
			mDevices[i]->warning(message);
	}

	void Log::error(string message)
	{
		for (int i = -1; ++i < mDevices.size();)
			mDevices[i]->error(message);
	}
}