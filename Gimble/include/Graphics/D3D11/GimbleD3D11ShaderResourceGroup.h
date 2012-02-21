
#ifndef _GIMBLE_D3D11SHADERRESOURCEGROUP_
#define _GIMBLE_D3D11SHADERRESOURCEGROUP_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleShaderResourceGroup.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11ShaderResourceGroup : public ShaderResourceGroup
		{
		private:
			ID3D11ShaderResourceView **mSRVs;
			uint mNum;

		private:
			void realloc(uint size);

		public:
			D3D11ShaderResourceGroup(unsigned char initialSize);
			~D3D11ShaderResourceGroup();

			void set(Texture *tex, unsigned char index);
			void set(DepthStencil *ds, unsigned char index);
			void set(RenderTarget *rt, unsigned char index);
			void set(Buffer *buf, unsigned char index);

			void clear();

			inline ID3D11ShaderResourceView **getSRVs() { return mSRVs; }
			inline uint getNumSRVs() { return mNum; }
		};
	}
}

#endif