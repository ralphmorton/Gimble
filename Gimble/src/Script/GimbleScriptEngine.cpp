
#include "Script/GimbleScriptEngine.h"
#include "Script/GimbleScriptFunction.h"

namespace Gimble
{
	template<> Gimble::Script::ScriptEngine* Gimble::Singleton<Gimble::Script::ScriptEngine>::ms_Singleton = 0;

	namespace Script
	{
		void ScriptEngine::runFunc(const ScriptFunctionBase *func, ScriptFunctionContext *parms)
		{
			func->run(parms);
		}
	}
}