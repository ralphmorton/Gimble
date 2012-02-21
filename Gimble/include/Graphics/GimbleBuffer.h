
#ifndef _GIMBLE_BUFFER_
#define _GIMBLE_BUFFER_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API Buffer
		{
		public:
			enum BufferType
			{
				BUFFERTYPE_VERTEX,
				BUFFERTYPE_INDEX,
				BUFFERTYPE_CONSTANT,
				BUFFERTYPE_SHADERRESOURCE,
				BUFFERTYPE_COMPUTE_STRUCTURED,
				BUFFERTYPE_COMPUTE_STRUCTURED_UNORDERED,
				BUFFERTYPE_COMPUTE_RAW,
				BUFFERTYPE_COMPUTE_RAW_UNORDERED
			};

			enum BufferUsage
			{
				BUFFERUSAGE_STATIC = 1,
				BUFFERUSAGE_DYNAMIC = 2,
				BUFFERUSAGE_WRITEONLY = 4,
				BUFFERUSAGE_STATIC_WRITEONLY = 5,
				BUFFERUSAGE_DYNAMIC_WRITEONLY = 6
			};

			enum MapType
			{
				BUFFERMAPTYPE_READ,
				BUFFERMAPTYPE_WRITE
			};

		protected:
			BufferType mType;
			BufferUsage mUsage;
			uint mSize;
			uint mStride;

		protected:
			Buffer(BufferType type, BufferUsage usage, uint size, uint stride) : mType(type), mUsage(usage), mSize(size), mStride(stride) { }

		public:
			virtual ~Buffer() {}

			virtual void setData(uint start, uint length, const void *source) = 0;

			virtual void *map(MapType mapType) = 0;
			virtual void unmap() = 0;

			inline BufferType getType(void) const { return mType; }
			inline BufferUsage getUsage(void) const { return mUsage; }
			inline uint getSize() const { return mSize; }
			inline uint getStride() const { return mStride; }
		};
	}
}

#endif