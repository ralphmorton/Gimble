
#ifndef _GIMBLE_D3D11TEXTURELOADER_
#define _GIMBLE_D3D11TEXTURELOADER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/D3D11/GimbleD3D11Texture.h"
#include "Graphics/GimbleTextureLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11TextureLoader : public TextureLoader
		{
		public:
			D3D11TextureLoader();
			~D3D11TextureLoader() {}

			Resource *load(ResourceStream &stream);
		};
	}
}

#endif