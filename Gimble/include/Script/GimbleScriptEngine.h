
#ifndef _GIMBLE_SCRIPTENGINE_
#define _GIMBLE_SCRIPTENGINE_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"

namespace Gimble
{
	namespace Script
	{
		class GIMBLE_API ScriptEngine : public Singleton<ScriptEngine>
		{
		private:
			friend class ScriptFunctionBase;

		public:
			virtual ~ScriptEngine() { }

			virtual void registerFunction(ScriptFunctionBase*) = 0;
			virtual void registerClass(ScriptClassBase*) = 0;

			virtual void run(const string &text) = 0;

			virtual bool isClassRegistered(const type_info &type) = 0;
			virtual const ScriptClassBase *getClass(const type_info &type) = 0;

		protected:
			static void runFunc(const ScriptFunctionBase *func, ScriptFunctionContext *ctx);
		};
	}
}

#endif