
#include "Graphics/D3D11/GimbleD3D11Shader.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11VertexShader::D3D11VertexShader(ID3D10Blob *vertexBlob, ID3D11VertexShader *shader) : mVertexBlob(vertexBlob), mShader(shader) { }
		D3D11VertexShader::~D3D11VertexShader()
		{
			SAFE_RELEASE(mVertexBlob);
			SAFE_RELEASE(mShader);
		}

		D3D11PixelShader::D3D11PixelShader(ID3D11PixelShader *shader) : mShader(shader) { }
		D3D11PixelShader::~D3D11PixelShader()
		{
			SAFE_RELEASE(mShader);
		}

		D3D11GeometryShader::D3D11GeometryShader(ID3D11GeometryShader *shader) : mShader(shader) { }
		D3D11GeometryShader::~D3D11GeometryShader()
		{
			SAFE_RELEASE(mShader);
		}

		D3D11HullShader::D3D11HullShader(ID3D11HullShader *shader) : mShader(shader) { }
		D3D11HullShader::~D3D11HullShader()
		{
			SAFE_RELEASE(mShader);
		}

		D3D11DomainShader::D3D11DomainShader(ID3D11DomainShader *shader) : mShader(shader) { }
		D3D11DomainShader::~D3D11DomainShader()
		{
			SAFE_RELEASE(mShader);
		}
	}
}