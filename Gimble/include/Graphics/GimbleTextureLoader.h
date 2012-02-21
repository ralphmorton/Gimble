
#ifndef _GIMBLE_TEXTURELOADER_
#define _GIMBLE_TEXTURELOADER_

#include "GimbleReqs.h"
#include "Graphics/GimbleTexture.h"
#include "GimbleResourceLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API TextureLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(Texture); }
		};
	}
}

#endif