
#include "Graphics/D3D11/GimbleD3D11ShaderResourceGroup.h"
#include "Graphics/D3D11/GimbleD3D11Texture.h"
#include "Graphics/D3D11/GimbleD3D11DepthStencil.h"
#include "Graphics/D3D11/GimbleD3D11RenderTarget.h"
#include "Graphics/D3D11/GimbleD3D11Buffer.h"
#include "GimbleMath.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11ShaderResourceGroup::D3D11ShaderResourceGroup(unsigned char initialSize) : mNum(initialSize), mSRVs(new ID3D11ShaderResourceView*[initialSize])
		{
			memset(mSRVs, 0, mNum);
		}

		D3D11ShaderResourceGroup::~D3D11ShaderResourceGroup()
		{
			if (mSRVs)
				delete [] mSRVs;
		}

		void D3D11ShaderResourceGroup::realloc(uint size)
		{
			ID3D11ShaderResourceView **srvs = new ID3D11ShaderResourceView*[size];
			memset(srvs, 0, size);

			uint to = Math::Min(mNum, size);
			mNum = size;
			for (int i = -1; ++i < to; srvs[i] = mSRVs[i]);;

			delete [] mSRVs;
			mSRVs = srvs;
		}

		void D3D11ShaderResourceGroup::set(Texture *tex, unsigned char index)
		{
			if (index >= mNum)
				realloc(index+1);

			D3D11Texture *tex11 = static_cast<D3D11Texture*>(tex);
			mSRVs[index] = tex11->getTexture();
		}

		void D3D11ShaderResourceGroup::set(DepthStencil *ds, unsigned char index)
		{
			if (index >= mNum)
				realloc(index+1);

			D3D11DepthStencil *ds11 = static_cast<D3D11DepthStencil*>(ds);
			mSRVs[index] = ds11->getShaderResourceView();
		}

		void D3D11ShaderResourceGroup::set(RenderTarget *rt, unsigned char index)
		{
			if (index >= mNum)
				realloc(index+1);

			D3D11RenderTarget *rt11 = static_cast<D3D11RenderTarget*>(rt);
			mSRVs[index] = rt11->getShaderResourceView();
		}

		void D3D11ShaderResourceGroup::set(Buffer *buf, unsigned char index)
		{
			if (index >= mNum)
				realloc(index+1);

			D3D11Buffer *buf11 = static_cast<D3D11Buffer*>(buf);
			mSRVs[index] = buf11->getSRV();
		}

		void D3D11ShaderResourceGroup::clear()
		{
			realloc(0);
		}
	}
}