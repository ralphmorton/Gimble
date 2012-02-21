
#include "Graphics/D3D11/GimbleD3D11RenderTarget.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11RenderTarget::D3D11RenderTarget(ID3D11Texture2D *tex, ID3D11RenderTargetView *renderTarget, ID3D11ShaderResourceView *shaderResourceView, uint width, uint height, PixelFormat format) : RenderTarget(width, height, format), mTex(tex), mRenderTarget(renderTarget), mShaderResourceView(shaderResourceView)
		{
			
		}

		D3D11RenderTarget::~D3D11RenderTarget()
		{
			SAFE_RELEASE(mTex);
			SAFE_RELEASE(mShaderResourceView);
			SAFE_RELEASE(mRenderTarget);
		}

		D3D11RenderTarget *D3D11RenderTarget::create(ID3D11Device *device, uint width, uint height, PixelFormat format)
		{
			ID3D11Texture2D *tex = 0;
			ID3D11RenderTargetView *renderTarget = 0;
			ID3D11ShaderResourceView *shaderResourceView = 0;

			DXGI_FORMAT dformat = D3D11Utils::getPixelFormat(format);

			D3D11_TEXTURE2D_DESC desc;
		    desc.Width = width;
		    desc.Height = height;
		    desc.MipLevels = 1;
		    desc.ArraySize = 1;
		    desc.Format = dformat;
		    desc.SampleDesc.Count = 1;
		    desc.SampleDesc.Quality = 0;
		    desc.Usage = D3D11_USAGE_DEFAULT;
		    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		    desc.CPUAccessFlags = 0;
		    desc.MiscFlags = 0;

		    if (FAILED(device->CreateTexture2D(&desc, NULL, &tex)))
			{
				Log::getPtr()->error("Could not create D3D11 RenderTarget Texture");
				return 0;
			}

			D3D11_RENDER_TARGET_VIEW_DESC rdesc;
		    rdesc.Format = dformat;
		    rdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		    rdesc.Texture2D.MipSlice = 0;

			if (FAILED(device->CreateRenderTargetView(tex, &rdesc, &renderTarget)))
			{
				Log::getPtr()->error("Could not create D3D11 RenderTarget");
				return 0;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC sdesc;
		    sdesc.Format = dformat;
		    sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		    sdesc.Texture2D.MostDetailedMip = 0;
		    sdesc.Texture2D.MipLevels = desc.MipLevels;

			if (FAILED(device->CreateShaderResourceView(tex, &sdesc, &shaderResourceView)))
			{
				Log::getPtr()->error("Could not create D3D11 RenderTarget ShaderResourceView");
				return 0;
			}

			return new D3D11RenderTarget(tex, renderTarget, shaderResourceView, width, height, format);
		}
	}
}