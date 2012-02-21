
#ifndef _GIMBLE_FILERESOURCESTREAM_
#define _GIMBLE_FILERESOURCESTREAM_

#include "GimbleReqs.h"
#include "GimbleResourceStream.h"
#include "GimbleLog.h"

namespace Gimble
{
	class GIMBLE_API FileResourceStream : public ResourceStream
	{
	private:
		string mFilename;
		void *mData;
		
	protected:
		FileResourceStream(const string &filename, void *data, uint size, ResourceStreamAccess access) : ResourceStream(size, access), mFilename(filename), mData(data) {}

	public:
		~FileResourceStream() 
		{
			delete[] mData;
		}

		virtual void read(void *dest, uint offset, uint length);
		virtual void write(const void *src, uint length);

		static FileResourceStream *load(const string &filename, ResourceStreamAccess access);

		inline string getResourceID() { return mFilename; }
	};
}

#endif