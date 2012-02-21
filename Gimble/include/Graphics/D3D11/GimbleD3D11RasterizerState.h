
#ifndef _GIMBLE_D3D11RASTERIZERSTATE_
#define _GIMBLE_D3D11RASTERIZERSTATE_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleRasterizerState.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11RasterizerState : public RasterizerState
		{
		private:
			ID3D11RasterizerState *mState;
			D3D11_RASTERIZER_DESC mDesc;

			bool mDirty;

			FillMode mFillMode;
			CullMode mCullMode;
			bool mFrontCCW;
			int mDepthBias;
			float mDepthBiasClamp;
			float mSlopeScaledDepthBias;
			bool mDepthClipEnabled;
			bool mScissorEnabled;
			bool mMultisampleEnabled;
			bool mAntialiasedLineEnabled;

		private:
			void update();

		public:
			D3D11RasterizerState();
			~D3D11RasterizerState();

			FillMode getFillMode();
			CullMode getCullMode();
			bool getFrontCounterClockwise();
			int getDepthBias();
			float getDepthBiasClamp();
			float getSlopeScaledDepthBias();
			bool getDepthClipEnabled();
			bool getScissorEnabled();
			bool getMultisampleEnabled();
			bool getAntialiasedLineEnabled();

			void setFillMode(FillMode mode);
			void setCullMode(CullMode mode);
			void setFrontCounterClockwise(bool y);
			void setDepthBias(int bias);
			void setDepthBiasClamp(float clamp);
			void setSlopeScaledDepthBias(float bias);
			void setDepthClipEnabled(bool enabled);
			void setScissorEnabled(bool enabled);
			void setMultisampleEnabled(bool enabled);
			void setAntialiasedLineEnabled(bool enabled);

			void set();
		};
	}
}

#endif