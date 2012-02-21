
#ifndef _GIMBLE_D3D11DEPTHSTENCIL_
#define _GIMBLE_D3D11DEPTHSTENCIL_

#include "GimbleReqs.h"
#include "Graphics/GimbleDepthStencil.h"
#include "Graphics/GimblePixelFormat.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11DepthStencil : public DepthStencil
		{
		private:
			ID3D11Texture2D *mTex;
			ID3D11DepthStencilView *mDepthStencil;
			ID3D11ShaderResourceView *mShaderResourceView;

		protected:
			D3D11DepthStencil(ID3D11Texture2D *tex, ID3D11DepthStencilView *depthStencil, ID3D11ShaderResourceView *shaderResourceView, uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat);

		public:
			~D3D11DepthStencil();

			static D3D11DepthStencil *create(ID3D11Device *device, uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat);

			inline ID3D11Texture2D *getTex() const { return mTex; }
			inline ID3D11DepthStencilView *getDepthStencil() const { return mDepthStencil; }
			inline ID3D11ShaderResourceView *getShaderResourceView() const { return mShaderResourceView; }
		};
	}
}

#endif