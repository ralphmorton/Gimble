
#ifndef _GIMBLE_RESOURCE_
#define _GIMBLE_RESOURCE_

#include "GimbleReqs.h"

namespace Gimble
{
	class GIMBLE_API Resource
	{
	public:
		virtual ~Resource() { }

		virtual const std::type_info &getType() = 0;
		virtual const string &getTypeName() = 0;
	};
}

#endif