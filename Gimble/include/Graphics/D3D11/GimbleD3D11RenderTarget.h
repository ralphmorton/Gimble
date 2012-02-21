
#ifndef _GIMBLE_D3D11RENDERTARGET_
#define _GIMBLE_D3D11RENDERTARGET_

#include "GimbleReqs.h"
#include "Graphics/GimbleRenderTarget.h"
#include "Graphics/GimblePixelFormat.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11RenderTarget : public RenderTarget
		{
		private:
			ID3D11Texture2D *mTex;
			ID3D11RenderTargetView *mRenderTarget;
			ID3D11ShaderResourceView *mShaderResourceView;

		protected:
			D3D11RenderTarget(ID3D11Texture2D *tex, ID3D11RenderTargetView *renderTarget, ID3D11ShaderResourceView *shaderResourceView, uint width, uint height, PixelFormat format);

		public:
			~D3D11RenderTarget();

			static D3D11RenderTarget *create(ID3D11Device *device, uint width, uint height, PixelFormat format);

			inline ID3D11Texture2D *getTex() const { return mTex; }
			inline ID3D11RenderTargetView *getRenderTarget() const { return mRenderTarget; }
			inline ID3D11ShaderResourceView *getShaderResourceView() const { return mShaderResourceView; }
		};
	}
}

#endif