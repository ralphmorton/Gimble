
#ifndef _GIMBLE_FILERESOURCELOCATOR_
#define _GIMBLE_FILERESOURCELOCATOR_

#include "GimbleReqs.h"
#include "GimbleResourceLocator.h"

namespace Gimble
{
	class GIMBLE_API FileResourceLocator : public ResourceLocator
	{
	public:
		bool exists(const string &name);
		ResourceStream *locate(const string &name);
	};
}

#endif