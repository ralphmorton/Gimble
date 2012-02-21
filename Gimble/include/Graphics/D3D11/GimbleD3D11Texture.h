
#ifndef _GIMBLE_D3D11TEXTURE_
#define _GIMBLE_D3D11TEXTURE_

#include "GimbleReqs.h"
#include "Graphics/GimbleTexture.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11Texture : public Texture
		{
		private:
			ID3D11ShaderResourceView *mTexture;
			D3D11_SRV_DIMENSION mDimension;

			bool mMappable;
			bool mReadable;
			bool mWriteable;

			uint mWidth, mHeight, mDepth;

		protected:
			D3D11Texture(ID3D11ShaderResourceView *texture, bool mappable = false, bool readable = false, bool writeable = false);

		public:
			~D3D11Texture();

			MappedTexture map();
			void unmap();

			inline ID3D11ShaderResourceView *getTexture() const { return mTexture; }
			inline bool isMappable() const { return mMappable; }

			inline uint getWidth() const { return mWidth; }
			inline uint getHeight() const { return mHeight; }
			inline uint getDepth() const { return mDepth; }

			static D3D11Texture *load(ID3D11Device *device, ResourceStream &stream);
			static D3D11Texture *create(ID3D11Device *device, uint width, uint height, uint mipLevels, PixelFormat format, TextureAccess access);
		};
	}
}

#endif