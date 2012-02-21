
#ifndef _GIMBLE_RASTERIZERSTATE_
#define _GIMBLE_RASTERIZERSTATE_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API RasterizerState
		{
		public:
			enum FillMode
			{
				FILLMODE_SOLID,
				FILLMODE_WIREFRAME
			};

			enum CullMode
			{
				CULLMODE_BACK,
				CULLMODE_FRONT,
				CULLMODE_NONE
			};

		public:
			virtual FillMode getFillMode() = 0;
			virtual CullMode getCullMode() = 0;
			virtual bool getFrontCounterClockwise() = 0;
			virtual int getDepthBias() = 0;
			virtual float getDepthBiasClamp() = 0;
			virtual float getSlopeScaledDepthBias() = 0;
			virtual bool getDepthClipEnabled() = 0;
			virtual bool getScissorEnabled() = 0;
			virtual bool getMultisampleEnabled() = 0;
			virtual bool getAntialiasedLineEnabled() = 0;

			virtual void setFillMode(FillMode mode) = 0;
			virtual void setCullMode(CullMode mode) = 0;
			virtual void setFrontCounterClockwise(bool y) = 0;
			virtual void setDepthBias(int bias) = 0;
			virtual void setDepthBiasClamp(float clamp) = 0;
			virtual void setSlopeScaledDepthBias(float bias) = 0;
			virtual void setDepthClipEnabled(bool enabled) = 0;
			virtual void setScissorEnabled(bool enabled) = 0;
			virtual void setMultisampleEnabled(bool enabled) = 0;
			virtual void setAntialiasedLineEnabled(bool enabled) = 0;
		};
	}
}

#endif