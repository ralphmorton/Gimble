
#ifndef _GIMBLE_RESOURCESTREAM_
#define _GIMBLE_RESOURCESTREAM_

#include "GimbleReqs.h"

namespace Gimble
{
	class GIMBLE_API ResourceStream
	{
	public:
		enum ResourceStreamAccess
		{
			RESOURCESTREAMACCESS_READ = 1,
			RESOURCESTREAMACCESS_WRITE = 2
		};

	protected:
		uint mSize;
		ResourceStreamAccess mAccess;

	protected:
		ResourceStream(uint size, ResourceStreamAccess access) : mSize(size), mAccess(access) {}

	public:
		virtual ~ResourceStream() {}

		virtual void read(void *dest, uint offset, uint length) = 0;
		virtual void write(const void *src, uint length) { }

		inline uint getSize() { return mSize; }
		inline ResourceStreamAccess getAccess() { return mAccess; }

		inline bool hasReadAccess() { return (mAccess & RESOURCESTREAMACCESS_READ) != 0; }
		inline bool hasWriteAccess() { return (mAccess & RESOURCESTREAMACCESS_WRITE) != 0; }

		virtual string getResourceID() = 0;
	};
}

#endif