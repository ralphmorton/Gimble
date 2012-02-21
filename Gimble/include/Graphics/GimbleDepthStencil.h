
#ifndef _GIMBLE_DEPTHSTENCIL_
#define _GIMBLE_DEPTHSTENCIL_

#include "GimbleReqs.h"
#include "Graphics/GimblePixelFormat.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API DepthStencil
		{
		protected:
			uint mWidth;
			uint mHeight;
			PixelFormat mTextureFormat, mDepthFormat, mResourceFormat;

		protected:
			DepthStencil(uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat) : mWidth(width), mHeight(height), mTextureFormat(textureFormat), mDepthFormat(depthFormat), mResourceFormat(resourceFormat) { }

		public:
			virtual ~DepthStencil() { }

			inline uint getWidth() const { return mWidth; }
			inline uint getHeight() const { return mHeight; }
			inline PixelFormat getTextureFormat() const { return mTextureFormat; }
			inline PixelFormat getDepthFormat() const { return mDepthFormat; }
			inline PixelFormat getResourceFormat() const { return mResourceFormat; }
		};
	}
}

#endif