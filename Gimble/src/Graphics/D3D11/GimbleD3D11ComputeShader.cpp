
#include "Graphics/D3D11/GimbleD3D11ComputeShader.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "Graphics/D3D11/GimbleD3D11Buffer.h"
#include "Graphics/GimbleMultiBuffer.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11ComputeShader::D3D11ComputeShader(ID3D11ComputeShader *shader) : mShader(shader)
		{

		}

		D3D11ComputeShader::~D3D11ComputeShader()
		{
			SAFE_RELEASE(mShader);
		}

		void D3D11ComputeShader::run(const MultiBuffer &shaderResourceViews, const MultiBuffer &unorderedAccessViews, const MultiBuffer &constantBuffers, uint x, uint y, uint z)
		{
			ID3D11DeviceContext *ctx = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getContext();

			uint nsrvs = shaderResourceViews.count();
			uint nuavs = unorderedAccessViews.count();
			uint ncbufs = constantBuffers.count();

			ID3D11ShaderResourceView** srvs = new ID3D11ShaderResourceView*[nsrvs];
			for (int i = -1; ++i < nsrvs;)
				srvs[i] = static_cast<D3D11Buffer*>(shaderResourceViews.get(i))->getSRV();

			ID3D11UnorderedAccessView** uavs = new ID3D11UnorderedAccessView*[nuavs];
			for (int i = -1; ++i < nuavs;)
				uavs[i] = static_cast<D3D11Buffer*>(unorderedAccessViews.get(i))->getUAV();
			
			ID3D11Buffer** cbufs = new ID3D11Buffer*[ncbufs];
			for (int i = -1; ++i < ncbufs;)
				cbufs[i] = static_cast<D3D11Buffer*>(constantBuffers.get(i))->getD3D11Buffer();

			ctx->CSSetShader(mShader, NULL, 0);

			if (nsrvs>0) 
				ctx->CSSetShaderResources(0, nsrvs, srvs);
			if (nuavs>0) 
				ctx->CSSetUnorderedAccessViews(0, unorderedAccessViews.count(), uavs, NULL);
			if (ncbufs>0)
				ctx->CSSetConstantBuffers(0, constantBuffers.count(), cbufs);

			ctx->Dispatch(x, y, z);

			ctx->CSSetShader(NULL, NULL, 0);

			if (nsrvs>0) 
			{
				ID3D11UnorderedAccessView* uavN[1] = { NULL };
				ctx->CSSetUnorderedAccessViews(0, 1, uavN, NULL);
			}

			if (nuavs>0)
			{
				ID3D11ShaderResourceView* srvN[2] = { NULL, NULL };
				ctx->CSSetShaderResources(0, 2, srvN);
			}

			if (ncbufs>0)
			{
				ID3D11Buffer* cbufN[1] = { NULL };
				ctx->CSSetConstantBuffers(0, 1, cbufN);
			}

			delete srvs;
			delete uavs;
			delete cbufs;
		}
	}
}