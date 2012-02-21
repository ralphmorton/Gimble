
#ifndef _GIMBLE_D3D11_DEPTHSTENCILSTATE_
#define _GIMBLE_D3D11_DEPTHSTENCILSTATE_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleDepthStencilState.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11DepthStencilState : public DepthStencilState
		{
		private:
			ID3D11DepthStencilState *mState;
			D3D11_DEPTH_STENCIL_DESC mDesc;

			bool mDirty;

			bool mDepthEnabled;
			bool mStencilEnabled;
			DepthWriteMask mDepthWriteMask;
			ComparisonFunc mDepthComparisonFunc;
			unsigned char mStencilReadMask;
			unsigned char mStencilWriteMask;
			StencilInfo mFrontFace;
			StencilInfo mBackFace;
			uint mStencilRef;

		private:
			void update();

		public:
			D3D11DepthStencilState();
			~D3D11DepthStencilState();

			bool getDepthEnabled();
			DepthWriteMask getDepthWriteMask();
			ComparisonFunc getDepthFunc();
			bool getStencilEnabled();
			unsigned char getStencilReadMask();
			unsigned char getStencilWritemask();
			StencilInfo getFrontFace();
			StencilInfo getBackFace();
			uint getStencilRef();

			void setDepthEnabled(bool enabled);
			void setDepthWriteMask(DepthWriteMask mask);
			void setDepthFunc(ComparisonFunc func);
			void setStencilEnabled(bool enabled);
			void setStencilReadMask(unsigned char mask);
			void setStencilWriteMask(unsigned char mask);
			void setFrontFace(StencilInfo face);
			void setBackFace(StencilInfo face);
			void setStencilRef(uint val);

			void set();
		};
	}
}

#endif

