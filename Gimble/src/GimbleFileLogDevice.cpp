
#include "GimbleFileLogDevice.h"

template<> Gimble::FileLogDevice* Gimble::Singleton<Gimble::FileLogDevice>::ms_Singleton = 0;

namespace Gimble
{
	FileLogDevice::FileLogDevice(string filename) : mFilename(filename)
	{
	
	}

	FileLogDevice::~FileLogDevice()
	{
		save();
	}

	void FileLogDevice::info(string message)
	{
		mEntries.push_back("INFO: " + message);
	}

	void FileLogDevice::warning(string message)
	{
		mEntries.push_back("WARNING: " + message);
	}

	void FileLogDevice::error(string message)
	{
		mEntries.push_back("ERROR: " + message);
	}

	void FileLogDevice::save()
	{
		std::ofstream of;
		of.open(mFilename.c_str(), std::ios::trunc);
		if (!of.is_open()) 
			return;

		for (unsigned int i = 0; i < mEntries.size(); i++)
			of << mEntries[i] << std::endl;

		of.close();
		return;
	}
}