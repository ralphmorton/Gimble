
#ifndef _GIMBLE_FILELOGDEVICE_
#define _GIMBLE_FILELOGDEVICE_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"
#include "GimbleLogDevice.h"

namespace Gimble
{
	class FileLogDevice : public LogDevice, public Singleton<FileLogDevice>
	{
	private:
		string mFilename;
		std::vector<string> mEntries;

	public:
		FileLogDevice(string filename);
		~FileLogDevice();

		void info(string message);
		void warning(string message);
		void error(string message);

		void save();
	};
}

#endif