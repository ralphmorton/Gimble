
#ifndef _GIMBLE_SCRIPTFUNCTIONCONTEXT_
#define _GIMBLE_SCRIPTFUNCTIONCONTEXT_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Script
	{
		class GIMBLE_API ScriptFunctionContext
		{
		private:
			virtual void *getObjectContext() = 0;
			virtual void *getParm(const type_info &type, uint index) = 0;
			virtual void returnValue(const type_info &type, void *value) = 0;

		public:
			virtual ~ScriptFunctionContext() { }

			template <class T> T *getObjectContext() { return static_cast<T*>(getObjectContext()); }
			template <class T> T *getParm(uint index) { return static_cast<T*>(getParm(typeid(T), index)); }
			template <class T> void returnValue(T value) { returnValue(typeid(T), &value); }

			virtual void raiseError(const string error) = 0;
		};
	}
}

#endif