
#ifndef _GIMBLE_DEPTHSTENCILSTATE_
#define _GIMBLE_DEPTHSTENCILSTATE_

#include "GimbleReqs.h"
#include "Graphics/GimbleComparisonFunc.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API DepthStencilState
		{
		public:
			enum DepthWriteMask
			{
				DEPTHWRITEMASK_ZERO,
				DEPTHWRITEMASK_ALL
			};

			enum StencilOp
			{
				STENCILOP_KEEP,
				STENCILOP_ZERO,
				STENCILOP_REPLACE,
				STENCILOP_INCR_CLAMP,
				STENCILOP_DECR_CLAMP,
				STENCILOP_INVERT,
				STENCILOP_INCR,
				STENCILOP_DECR
			};

		public:
			struct StencilInfo
			{
				StencilOp StencilFailOp;
				StencilOp StencilDepthFailOp;
				StencilOp StencilPassOp;
				ComparisonFunc StencilFunc;
			};

		public:
			virtual bool getDepthEnabled() = 0;
			virtual DepthWriteMask getDepthWriteMask() = 0;
			virtual ComparisonFunc getDepthFunc() = 0;
			virtual bool getStencilEnabled() = 0;
			virtual unsigned char getStencilReadMask() = 0;
			virtual unsigned char getStencilWritemask() = 0;
			virtual StencilInfo getFrontFace() = 0;
			virtual StencilInfo getBackFace() = 0;
			virtual uint getStencilRef() = 0;

			virtual void setDepthEnabled(bool enabled) = 0;
			virtual void setDepthWriteMask(DepthWriteMask mask) = 0;
			virtual void setDepthFunc(ComparisonFunc func) = 0;
			virtual void setStencilEnabled(bool enabled) = 0;
			virtual void setStencilReadMask(unsigned char mask) = 0;
			virtual void setStencilWriteMask(unsigned char mask) = 0;
			virtual void setFrontFace(StencilInfo face) = 0;
			virtual void setBackFace(StencilInfo face) = 0;
			virtual void setStencilRef(uint val) = 0;
		};
	}
}

#endif