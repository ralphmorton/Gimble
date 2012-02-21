
#ifndef _GIMBLE_D3D11COMPUTESHADERLOADER_
#define _GIMBLE_D3D11COMPUTESHADERLOADER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/D3D11/GimbleD3D11ComputeShader.h"
#include "Graphics/GimbleComputeShaderLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11ComputeShaderLoader : public ComputeShaderLoader
		{
		public:
			D3D11ComputeShaderLoader();
			~D3D11ComputeShaderLoader() {}

			Resource *load(ResourceStream &stream);
		};
	}
}

#endif