
#ifndef _GIMBLE_D3D11BLENDSTATE_
#define _GIMBLE_D3D11BLENDSTATE_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleBlendState.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11BlendState : public BlendState
		{
		private:
			ID3D11BlendState *mState;
			D3D11_BLEND_DESC mDesc;

			bool mDirty;

			bool mAlphaToCoverageEnabled;
			bool mIndependentBlendEnabled;
			float mBlendFactor[4];
			uint mSampleMask;

			RenderTargetBlendInfo mBlendInfo[8];

		private:
			void update();

		public:
			D3D11BlendState();
			~D3D11BlendState();

			bool getAlphaToCoverageEnabled();
			bool getIndependentBlendEnabled();
			uint getMaxRenderTargets();
			RenderTargetBlendInfo getRenderTargetBlendInfo(uint index);
			void getBlendFactor(float &r, float &g, float &b, float &a);
			uint getSampleMask();
			
			void setAlphaToCoverageEnabled(bool enabled);
			void setIndependentBlendEnabled(bool enabled);
			void setRenderTargetBlendInfo(uint index, RenderTargetBlendInfo info);
			void setBlendFactor(float r, float g, float b, float a);
			void setSampleMask(uint mask);

			void set();
		};
	}
}

#endif