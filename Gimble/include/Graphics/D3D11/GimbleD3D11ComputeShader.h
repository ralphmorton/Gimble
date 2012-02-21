

#ifndef _GIMBLE_D3D11COMPUTESHADER_
#define _GIMBLE_D3D11COMPUTESHADER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleComputeShader.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11ComputeShader : public ComputeShader
		{
		private:
			ID3D11ComputeShader *mShader;

		public:
			D3D11ComputeShader(ID3D11ComputeShader *shader);
			~D3D11ComputeShader();

			void run(const MultiBuffer &shaderResourceViews, const MultiBuffer &unorderedAccessViews, const MultiBuffer &constantBuffers, uint x, uint y, uint z);
		};
	}
}

#endif