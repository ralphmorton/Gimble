
#ifndef _GIMBLE_D3D11SAMPLER_
#define _GIMBLE_D3D11SAMPLER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleSampler.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11Sampler : public Sampler
		{
		private:
			ID3D11SamplerState *mState;

		public:
			D3D11Sampler(
				Filter filter, 
				AddressMode addressU, 
				AddressMode addressV, 
				AddressMode addressW, 
				float mipLODBias, 
				uint maxAnistropy, 
				ComparisonFunc comparisonFunc, 
				float minLOD, 
				float maxLOD, 
				float borderR, float borderG, float borderB, float borderA);

			inline ID3D11SamplerState *getSampler() { return mState; }
		};
	}
}

#endif