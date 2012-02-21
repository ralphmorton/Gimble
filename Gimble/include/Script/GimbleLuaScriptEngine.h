
#ifndef _GIMBLE_LUASCRIPTENGINE_
#define _GIMBLE_LUASCRIPTENGINE_

#include "GimbleReqs.h"
#include "Script/GimbleScriptEngine.h"
#include "Script/GimbleScriptFunctionContext.h"

struct lua_State;

namespace Gimble
{
	namespace Script
	{
		class LuaSysParm
		{
		public:
			virtual ~LuaSysParm() { }
		};

		template <class T> class LuaSysParmT : public LuaSysParm
		{
		private:
			T *mT;

		public:
			LuaSysParmT(T *t) : mT(t) { }
			~LuaSysParmT() { if (mT) delete mT; }
		};

		class LuaScriptFunctionContext : public ScriptFunctionContext
		{
		private:
			lua_State *mState;
			bool mIsClassFunction;
			std::vector<LuaSysParm*> mSysParms;

		private:
			void *getObjectContext();
			void *getParm(const type_info &t, uint index);
			void returnValue(const type_info &t, void *val);

		public:
			LuaScriptFunctionContext(lua_State *state, bool isClassFunction = false) : mState(state), mIsClassFunction(isClassFunction) { }
			~LuaScriptFunctionContext();

			void raiseError(const string error);
		};

		class GIMBLE_API LuaScriptEngine : public ScriptEngine
		{
		private:
			lua_State *mState;

			typedef std::map<string, uint> ScriptFunctionLookupMap;
			typedef std::pair<string, uint> ScriptFunctionLookupPair;
			typedef std::map<string, uint>::iterator ScriptFunctionLookupMapIterator;

			typedef std::map<string, uint> ScriptClassLookupMap;
			typedef std::pair<string, uint> ScriptClassLookupPair;
			typedef std::map<string, uint>::iterator ScriptClassLookupMapIterator;

			std::vector<ScriptFunctionBase*> mScriptFunctions;
			std::vector<ScriptClassBase*> mScriptClasses;

			ScriptFunctionLookupMap mScriptFunctionLookups;
			ScriptClassLookupMap mScriptClassLookups;

		public:
			LuaScriptEngine();
			~LuaScriptEngine();

			void registerFunction(ScriptFunctionBase *scriptFunction);
			void registerClass(ScriptClassBase *scriptClass);

			void run(const string &text);

			bool isClassRegistered(const type_info &type);
			const ScriptClassBase *getClass(const type_info &type);

		private:
			static int executeFunction(lua_State *state);

			static int createClass(lua_State *state);
			static int delClass(lua_State *state);
			static int executeClassFunction(lua_State *state);

			static int onError(lua_State *state);
		};
	}
}

#endif