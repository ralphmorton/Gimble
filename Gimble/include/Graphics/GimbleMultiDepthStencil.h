
#ifndef _GIMBLE_MULTIDEPTHSTENCIL_
#define _GIMBLE_MULTIDEPTHSTENCIL_

#include "GimbleReqs.h"
#include "GimbleMulti.h"
#include "Graphics/GimbleDepthStencil.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API MultiDepthStencil : public Multi<DepthStencil> {};
	}
}

#endif