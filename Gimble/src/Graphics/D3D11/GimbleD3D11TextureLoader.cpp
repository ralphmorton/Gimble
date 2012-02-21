
#include "Graphics/D3D11/GimbleD3D11TextureLoader.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "GimbleResourceStream.h"
#include "GimbleResource.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11TextureLoader::D3D11TextureLoader() {}

		Resource *D3D11TextureLoader::load(ResourceStream &stream)
		{
			return D3D11Texture::load((static_cast<D3D11RenderSystem*>(RenderSystem::getPtr()))->getDevice(), stream);
		}
	}
}