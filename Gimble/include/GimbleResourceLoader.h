
#ifndef _GIMBLE_RESOURCELOADER_
#define _GIMBLE_RESOURCELOADER_

#include "GimbleReqs.h"

namespace Gimble
{
	class GIMBLE_API ResourceLoader
	{
	public:
		ResourceLoader() {}
		virtual ~ResourceLoader() {}

		virtual Resource *load(ResourceStream &stream) = 0;
		virtual const std::type_info &getType() = 0;
	};
}

#endif