
#ifndef _GIMBLE_RESOURCELOCATOR_
#define _GIMBLE_RESOURCELOCATOR_

#include "GimbleReqs.h"
#include "GimbleResourceStream.h"

namespace Gimble
{
	class GIMBLE_API ResourceLocator
	{
	public:
		ResourceLocator() {}
		virtual ~ResourceLocator() {}

		virtual bool exists(const string &name) = 0;
		virtual ResourceStream *locate(const string &name) = 0;
	};
}

#endif