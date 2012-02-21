
#ifndef _GIMBLE_COMPUTESHADERLOADER_
#define _GIMBLE_COMPUTESHADERLOADER_

#include "GimbleReqs.h"
#include "GimbleResourceLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API ComputeShaderLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(ComputeShader); }
		};
	}
}

#endif