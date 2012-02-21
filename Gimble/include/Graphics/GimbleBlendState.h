
#ifndef _GIMBLE_BLENDSTATE_
#define _GIMBLE_BLENDSTATE_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API BlendState
		{
		public:
			enum BlendType
			{
				BLEND_ZERO,
				BLEND_ONE,
				BLEND_SRC_COLOR,
				BLEND_INV_SRC_COLOR,
				BLEND_SRC_ALPHA,
				BLEND_INV_SRC_ALPHA,
				BLEND_DEST_ALPHA,
				BLEND_INV_DEST_ALPHA,
				BLEND_DEST_COLOR,
				BLEND_INV_DEST_COLOR,
				BLEND_SRC_ALPHA_CLAMP,
				BLEND_BLEND_FACTOR,
				BLEND_INV_BLEND_FACTOR,
				BLEND_SRC1_COLOR,
				BLEND_INV_SRC1_COLOR,
				BLEND_SRC1_ALPHA,
				BLEND_INV_SRC1_ALPHA
			};

			enum BlendOp
			{
				BLENDOP_ADD,
				BLENDOP_SUBTRACT,
				BLENDOP_REV_SUBTRACT,
				BLENDOP_MIN,
				BLENDOP_MAX
			};

			enum BlendWriteMask
			{
				BLENDWRITEMASK_ENABLE_RED	= 1,
				BLENDWRITEMASK_ENABLE_GREEN	= 2,
				BLENDWRITEMASK_ENABLE_BLUE	= 4,
				BLENDWRITEMASK_ENABLE_ALPHA	= 8,
				BLENDWRITEMASK_ENABLE_ALL	= ( ( ( BLENDWRITEMASK_ENABLE_RED | BLENDWRITEMASK_ENABLE_GREEN )  | BLENDWRITEMASK_ENABLE_BLUE )  | BLENDWRITEMASK_ENABLE_ALPHA )
			};

		public:
			struct RenderTargetBlendInfo
			{
				bool BlendEnable;
				BlendType SrcBlend;
				BlendType DestBlend;
				BlendState::BlendOp BlendOp;
				BlendType SrcBlendAlpha;
				BlendType DestBlendAlpha;
				BlendState::BlendOp BlendOpAlpha;
				unsigned char RenderTargetWriteMask;
			};

		public:
			virtual bool getAlphaToCoverageEnabled() = 0;
			virtual bool getIndependentBlendEnabled() = 0;
			virtual uint getMaxRenderTargets() = 0;
			virtual RenderTargetBlendInfo getRenderTargetBlendInfo(uint index) = 0;
			virtual void getBlendFactor(float &r, float &g, float &b, float &a) = 0;
			virtual uint getSampleMask() = 0;
			
			virtual void setAlphaToCoverageEnabled(bool enabled) = 0;
			virtual void setIndependentBlendEnabled(bool enabled) = 0;
			virtual void setRenderTargetBlendInfo(uint index, RenderTargetBlendInfo info) = 0;
			virtual void setBlendFactor(float r, float g, float b, float a) = 0;
			virtual void setSampleMask(uint mask) = 0;
		};
	}
}

#endif