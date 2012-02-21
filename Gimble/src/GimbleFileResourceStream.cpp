
#include "GimbleFileResourceStream.h"

namespace Gimble
{
	void FileResourceStream::read(void *dest, uint offset, uint length)
	{
		assert (offset + length <= mSize);

		void *d = (void *) ((int)mData + offset);

		memcpy(dest, d, length);
	}

	void FileResourceStream::write(const void *src, uint length)
	{
		if ((mAccess & RESOURCESTREAMACCESS_WRITE) == 0)
		{
			Log::getPtr()->error("Attempting to write to a ResourceStream (file: [" + mFilename + "]) that does not support RESOURCESTREAMACCESS_WRITE");
			return;
		}

		delete[] mData;
		mData = (void *)new char[length];
		mSize = length;
		memcpy(mData, src, length);

		std::ofstream f(mFilename, std::ios::trunc | std::ios::binary);
		if (!f.is_open())
		{
			Log::getPtr()->error("Could not write ResourceStream to file [" + mFilename + "], file could not be accessed, created or truncated");
			return;
		}

		f.write((char *)mData, length);
		f.close();
	}


	FileResourceStream *FileResourceStream::load(const string &filename, ResourceStreamAccess access)
	{
		if ((access & RESOURCESTREAMACCESS_READ) != 0)
		{
			std::ifstream f(filename, std::ios::in | std::ios::binary);
			if (!f.is_open())
			{
				Log::getPtr()->error("Could not load ResourceStream from file [" + filename + "], file doesnt exist or could not be accessed");
				return 0;
			}

			f.seekg(0, std::ios::end);
			uint length = (uint)f.tellg();
			f.seekg(0, std::ios::beg);

			char *data = new char[length];
			
			f.read(data, length);
			f.close();

			return new FileResourceStream(filename, (void *)data, length, access);
		}

		Log::getPtr()->error("Attempting to read from a ResourceStream (file: [" + filename + "]) that does not support RESOURCESTREAMACCESS_READ");
		return 0;
	}
}