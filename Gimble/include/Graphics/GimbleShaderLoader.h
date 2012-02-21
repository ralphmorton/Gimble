
#ifndef _GIMBLE_SHADERLOADER_
#define _GIMBLE_SHADERLOADER_

#include "GimbleReqs.h"
#include "GimbleResourceLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API VertexShaderLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(VertexShader); }
		};

		class GIMBLE_API PixelShaderLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(PixelShader); }
		};

		class GIMBLE_API GeometryShaderLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(GeometryShader); }
		};

		class GIMBLE_API HullShaderLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(HullShader); }
		};

		class GIMBLE_API DomainShaderLoader : public ResourceLoader
		{
		public:
			inline const std::type_info &getType() { return typeid(DomainShader); }
		};
	}
}

#endif