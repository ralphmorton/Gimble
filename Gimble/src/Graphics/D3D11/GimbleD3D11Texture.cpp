
#include "Graphics/D3D11/GimbleD3D11Texture.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "GimbleResourceStream.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11Texture::D3D11Texture(ID3D11ShaderResourceView *texture, bool mappable, bool readable, bool writeable) : Texture(), mTexture(texture), mMappable(mappable), mReadable(readable), mWriteable(writeable), mWidth(1), mHeight(1), mDepth(1)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC sdesc;
			texture->GetDesc(&sdesc);
			mDimension = sdesc.ViewDimension;

			switch (mDimension)
			{
			case D3D11_SRV_DIMENSION_TEXTURE1D:
				ID3D11Texture1D *tex1;
				mTexture->GetResource((ID3D11Resource**)&tex1);

				D3D11_TEXTURE1D_DESC tdesc1;
				tex1->GetDesc(&tdesc1);

				mWidth = tdesc1.Width;

				SAFE_RELEASE(tex1);
				break;

			case D3D11_SRV_DIMENSION_TEXTURE2D:
				ID3D11Texture2D *tex2;
				mTexture->GetResource((ID3D11Resource**)&tex2);

				D3D11_TEXTURE2D_DESC tdesc2;
				tex2->GetDesc(&tdesc2);

				mWidth = tdesc2.Width;
				mHeight = tdesc2.Height;

				SAFE_RELEASE(tex2);
				break;

			case D3D11_SRV_DIMENSION_TEXTURE3D:
				ID3D11Texture3D *tex3;
				mTexture->GetResource((ID3D11Resource**)&tex3);

				D3D11_TEXTURE3D_DESC tdesc3;
				tex3->GetDesc(&tdesc3);

				mWidth = tdesc3.Width;
				mHeight = tdesc3.Height;
				mDepth = tdesc3.Depth;

				SAFE_RELEASE(tex3);
				break;
			}
		}

		D3D11Texture::~D3D11Texture()
		{
			SAFE_RELEASE(mTexture);
		}

		MappedTexture D3D11Texture::map()
		{
			D3D11_MAP mtype = D3D11_MAP_WRITE_DISCARD;
			if (mReadable && mWriteable)
				mtype = D3D11_MAP_READ_WRITE;
			else if (mReadable)
				mtype = D3D11_MAP_READ;
			else 
				mtype = D3D11_MAP_WRITE_DISCARD;

			ID3D11DeviceContext *ctx = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getContext();

			D3D11_MAPPED_SUBRESOURCE msr;
			msr.pData = NULL;

			switch (mDimension)
			{
			case D3D11_SRV_DIMENSION_TEXTURE2D:

				ID3D11Texture2D *tex;
				mTexture->GetResource((ID3D11Resource**)&tex);
				
				ctx->Map(tex, D3D11CalcSubresource(0,0,1), D3D11_MAP_WRITE_DISCARD, 0, &msr);

				SAFE_RELEASE(tex);
				return MappedTexture(msr.pData, msr.RowPitch);
			}

			return MappedTexture(0, 0);
		}

		void D3D11Texture::unmap()
		{
			ID3D11DeviceContext *ctx = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getContext();

			switch (mDimension)
			{
			case D3D11_SRV_DIMENSION_TEXTURE2D:

				ID3D11Texture2D *tex;
				mTexture->GetResource((ID3D11Resource**)&tex);

				ctx->Unmap(tex, D3D11CalcSubresource(0,0,1));

				SAFE_RELEASE(tex);
			}
		}

		D3D11Texture *D3D11Texture::load(ID3D11Device *device, ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			stream.read(data, 0, stream.getSize());

			ID3D11ShaderResourceView *texture;

			HRESULT hr = D3DX11CreateShaderResourceViewFromMemory(device, data, stream.getSize(), NULL, NULL, &texture, NULL);

			delete [] data;

			if (FAILED(hr))
			{
				Log::getPtr()->error("Could not create D3D11Texture from Resource [" + stream.getResourceID() + "]");
				return 0;
			}

			return new D3D11Texture(texture);
		}

		D3D11Texture *D3D11Texture::create(ID3D11Device *device, uint width, uint height, uint mipLevels, PixelFormat format, TextureAccess access)
		{
			DXGI_FORMAT dformat = D3D11Utils::getPixelFormat(format);

			D3D11_USAGE dusage = D3D11_USAGE_DYNAMIC;

			unsigned int daccess = D3D11_CPU_ACCESS_WRITE;
			switch (access)
			{
			case TEXTUREACCESS_READ:
				daccess = D3D11_CPU_ACCESS_READ;
				break;
			case TEXTUREACCESS_WRITE:
				daccess = D3D11_CPU_ACCESS_WRITE;
				break;
			case TEXTUREACCESS_READWRITE:
				daccess = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
				break;
			}

			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = mipLevels;
			desc.ArraySize = 1;
			desc.Format = dformat;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = dusage;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = daccess;
			desc.MiscFlags = 0;

			ID3D11Texture2D *texture = NULL;
			HRESULT hr = device->CreateTexture2D(&desc, NULL, &texture);
			if (FAILED(hr))
			{
				Log::getPtr()->error("Could not create D3D11Texture");
				return 0;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC sdesc;
			sdesc.Format = desc.Format;
	        sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	        sdesc.Texture2D.MipLevels = desc.MipLevels;
	        sdesc.Texture2D.MostDetailedMip = desc.MipLevels -1;

			ID3D11ShaderResourceView *srv = NULL;
			if (FAILED(device->CreateShaderResourceView(texture, &sdesc, &srv)))
			{
				Log::getPtr()->error("Could not create D3D11Texture ShaderResourceView");
				return 0;
			}

			return new D3D11Texture(srv, true, access == TEXTUREACCESS_READ, access == TEXTUREACCESS_WRITE);
		}
	}
}