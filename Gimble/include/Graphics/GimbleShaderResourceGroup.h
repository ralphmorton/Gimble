
#ifndef _GIMBLE_SHADERRESOURCEGROUP_
#define _GIMBLE_SHADERRESOURCEGROUP_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API ShaderResourceGroup
		{
		public:
			virtual void set(Texture *tex, unsigned char index) = 0;
			virtual void set(DepthStencil *ds, unsigned char index) = 0;
			virtual void set(RenderTarget *rt, unsigned char index) = 0;
			virtual void set(Buffer *buf, unsigned char index) = 0;

			virtual void clear() = 0;
		};
	}
}

#endif