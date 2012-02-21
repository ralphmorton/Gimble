
#include "GimbleFileResourceLocator.h"
#include "GimbleFileResourceStream.h"

namespace Gimble
{
	bool FileResourceLocator::exists(const string &name)
	{
		std::ifstream f(name);
		bool exists = f.is_open();
		f.close();
		return exists;
	}

	ResourceStream *FileResourceLocator::locate(const string &name)
	{
		return FileResourceStream::load(name, ResourceStream::RESOURCESTREAMACCESS_READ);
	}
}