
#ifndef _GIMBLE_RENDERTARGET_
#define _GIMBLE_RENDERTARGET_

#include "GimbleReqs.h"
#include "Graphics/GimblePixelFormat.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API RenderTarget
		{
		protected:
			uint mWidth;
			uint mHeight;
			PixelFormat mFormat;

		protected:
			RenderTarget(uint width, uint height, PixelFormat format) : mWidth(width), mHeight(height), mFormat(format) {}

		public:
			virtual ~RenderTarget() {}

			inline uint getWidth() const { return mWidth; }
			inline uint getHeight() const { return mHeight; }
			inline PixelFormat getFormat() const { return mFormat; }
		};
	}
}

#endif