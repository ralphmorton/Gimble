
#include "Graphics/D3D11/GimbleD3D11Sampler.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11Sampler::D3D11Sampler(Filter filter, AddressMode addressU, AddressMode addressV, AddressMode addressW, float mipLODBias, uint maxAnistropy, ComparisonFunc comparisonFunc, float minLOD, float maxLOD, float borderR, float borderG, float borderB, float borderA)
		{
			D3D11_SAMPLER_DESC desc;
			desc.Filter = D3D11Utils::getFilter(filter);
			desc.AddressU = D3D11Utils::getAddressMode(addressU);
			desc.AddressV = D3D11Utils::getAddressMode(addressV);
			desc.AddressW = D3D11Utils::getAddressMode(addressW);
			desc.MipLODBias = mipLODBias;
			desc.MaxAnisotropy = maxAnistropy;
			desc.ComparisonFunc = D3D11Utils::getComparisonFunc(comparisonFunc);
			desc.MinLOD = minLOD;
			desc.MaxLOD = maxLOD;
			desc.BorderColor[0] = borderR;
			desc.BorderColor[1] = borderG;
			desc.BorderColor[2] = borderB;
			desc.BorderColor[3] = borderA;

			ID3D11Device *device = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getDevice();

			HRESULT hr = device->CreateSamplerState(&desc, &mState);
		}
	}
}