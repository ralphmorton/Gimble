
#ifndef _GIMBLE_D3D11REQS_
#define _GIMBLE_D3D11REQS_

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

namespace Gimble
{
	namespace Graphics
	{
		class D3D11Utils;
		class D3D11Buffer;
		class D3D11Texture;
		class D3D11TextureLoader;
		class D3D11VertexShader;
		class D3D11PixelShader;
		class D3D11GeometryShader;
		class D3D11HullShader;
		class D3D11DomainShader;
		class D3D11ComputeShader;
		class D3D11VertexShaderLoader;
		class D3D11PixelShaderLoader;
		class D3D11GeometryShaderLoader;
		class D3D11HullShaderLoader;
		class D3D11DomainShaderLoader;
		class D3D11ComputeShaderLoader;
		class D3D11RenderTarget;
		class D3D11DepthStencil;
		class D3D11InputLayout;
		class D3D11DepthStencilState;
		class D3D11RasterizerState;
		class D3D11BlendState;
		class D3D11Sampler;
		class D3D11ShaderResourceGroup;
		class D3D11RenderSystem;
	}
}

#endif