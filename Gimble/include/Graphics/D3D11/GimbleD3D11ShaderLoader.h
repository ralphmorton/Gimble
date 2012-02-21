
#ifndef _GIMBLE_D3D11SHADERLOADER_
#define _GIMBLE_D3D11SHADERLOADER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/D3D11/GimbleD3D11Shader.h"
#include "Graphics/GimbleShaderLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D10ShaderLoader
		{
		protected:
			ID3D10Blob *compile(const string &resid, void *data, uint size, string entry, string profile);
			bool read(const string &resid, void *data, uint size, string type, uint &compilelength, string &entrypoint, string &profile);
		};

		class D3D11VertexShaderLoader : public VertexShaderLoader, public D3D10ShaderLoader
		{
		public:
			Resource *load(ResourceStream &stream);
		};

		class D3D11PixelShaderLoader : public PixelShaderLoader, public D3D10ShaderLoader
		{
		public:
			Resource *load(ResourceStream &stream);
		};

		class D3D11GeometryShaderLoader : public GeometryShaderLoader, public D3D10ShaderLoader
		{
		public:
			Resource *load(ResourceStream &stream);
		};

		class D3D11HullShaderLoader : public HullShaderLoader, public D3D10ShaderLoader
		{
		public:
			Resource *load(ResourceStream &stream);
		};

		class D3D11DomainShaderLoader : public DomainShaderLoader, public D3D10ShaderLoader
		{
		public:
			Resource *load(ResourceStream &stream);
		};
	}
}

#endif