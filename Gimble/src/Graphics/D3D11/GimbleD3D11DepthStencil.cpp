
#include "Graphics/D3D11/GimbleD3D11DepthStencil.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11DepthStencil::D3D11DepthStencil(ID3D11Texture2D *tex, ID3D11DepthStencilView *depthStencil, ID3D11ShaderResourceView *shaderResourceView, uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat) : DepthStencil(width, height, textureFormat, depthFormat, resourceFormat), mTex(tex), mDepthStencil(depthStencil), mShaderResourceView(shaderResourceView)
		{
			
		}

		D3D11DepthStencil::~D3D11DepthStencil()
		{
			SAFE_RELEASE(mShaderResourceView);
			SAFE_RELEASE(mDepthStencil);
			SAFE_RELEASE(mTex);
		}

		D3D11DepthStencil *D3D11DepthStencil::create(ID3D11Device *device, uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat)
		{
			ID3D11Texture2D *tex = 0;
			ID3D11DepthStencilView *depthStencil = 0;
			ID3D11ShaderResourceView *shaderResourceView = 0;

			D3D11_TEXTURE2D_DESC ddesc;
		    ddesc.Width = width;
		    ddesc.Height = height;
		    ddesc.MipLevels = 1;
		    ddesc.ArraySize = 1;
		    ddesc.Format = D3D11Utils::getPixelFormat(textureFormat);
		    ddesc.SampleDesc.Count = 1;
		    ddesc.SampleDesc.Quality = 0;
		    ddesc.Usage = D3D11_USAGE_DEFAULT;
		    ddesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		    ddesc.CPUAccessFlags = 0;
		    ddesc.MiscFlags = 0;

			HRESULT hr = device->CreateTexture2D(&ddesc, NULL, &tex);
		    if(FAILED(hr))
			{
				Log::getPtr()->error("Could not create D3D11 DepthStencil Texture");
				return 0;
			}

		    D3D11_DEPTH_STENCIL_VIEW_DESC dsdesc;
			dsdesc.Format = D3D11Utils::getPixelFormat(depthFormat);
		    dsdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		    dsdesc.Texture2D.MipSlice = 0;
			dsdesc.Flags = 0;

			hr = device->CreateDepthStencilView(tex, &dsdesc, &depthStencil);
		    if(FAILED(hr))
			{
				Log::getPtr()->error("Could not create D3D11 DepthStencil");
				return 0;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC sdesc;
			ZeroMemory(&sdesc, sizeof(sdesc));
			sdesc.Format = D3D11Utils::getPixelFormat(resourceFormat);
		    sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		    sdesc.Texture2D.MostDetailedMip = 0;
		    sdesc.Texture2D.MipLevels = ddesc.MipLevels;

			hr = device->CreateShaderResourceView(tex, &sdesc, &shaderResourceView);
			if (FAILED(hr))
			{
				Log::getPtr()->error("Could not create D3D11 DepthStencil ShaderResourceView");
				return 0;
			}

			return new D3D11DepthStencil(tex, depthStencil, shaderResourceView, width, height, textureFormat, depthFormat, resourceFormat);
		}
	}
}