
#include "Script/GimbleLuaScriptEngine.h"
#include "Script/GimbleScriptFunction.h"
#include "Script/GimbleScriptClass.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

//Lua defines that dropped off the map in 5.1
#define lua_boxpointer(L,u) \
    (*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#define lua_unboxpointer(L,i)   (*(void **)(lua_touserdata(L, i)))

namespace Gimble
{
	namespace Script
	{
		LuaScriptFunctionContext::~LuaScriptFunctionContext()
		{
			for (uint i = 0; i < mSysParms.size(); i++)
				delete mSysParms[i];

			mSysParms.clear();
		}

		void *LuaScriptFunctionContext::getObjectContext()
		{
			return mIsClassFunction ? lua_unboxpointer(mState, 1) : 0;
		}

		void *LuaScriptFunctionContext::getParm(const type_info &t, uint index)
		{
			int n = lua_gettop(mState);

			int i = mIsClassFunction ? index+2 : index+1;

			if (i > n)
				return 0;

			if (t == typeid(string) && lua_isstring(mState, i))
			{
				string *v = new string(lua_tostring(mState, i));
				mSysParms.push_back(new LuaSysParmT<string>(v));
				return v;
			}
			else if (t == typeid(float) && lua_isnumber(mState, i))
			{
				float *v = new float((float)lua_tonumber(mState, i));
				mSysParms.push_back(new LuaSysParmT<float>(v));
				return v;
			}
			else if (t == typeid(double) && lua_isnumber(mState, i))
			{
				double *v = new double(lua_tonumber(mState, i));
				mSysParms.push_back(new LuaSysParmT<double>(v));
				return v;
			}
			else if (t == typeid(long) && lua_isnumber(mState, i))
			{
				long *v = new long((long)lua_tonumber(mState, i));
				mSysParms.push_back(new LuaSysParmT<long>(v));
				return v;
			}
			else if (t == typeid(int) && lua_isnumber(mState, i))
			{
				int *v = new int((int)lua_tonumber(mState, i));
				mSysParms.push_back(new LuaSysParmT<int>(v));
				return v;
			}
			else if (t == typeid(bool) && lua_isnumber(mState, i))
			{
				bool *v = new bool(lua_toboolean(mState, i)!=0);
				mSysParms.push_back(new LuaSysParmT<bool>(v));
				return v;
			}
			else if (lua_isuserdata(mState, i))
			{
				LuaScriptEngine *eng = static_cast<LuaScriptEngine*>(ScriptEngine::getPtr());

				if (eng->isClassRegistered(t))
					return lua_touserdata(mState, i);
			}

			return 0;
		}

		void LuaScriptFunctionContext::returnValue(const type_info &t, void *val)
		{
			LuaScriptEngine *eng = static_cast<LuaScriptEngine*>(ScriptEngine::getPtr());

			if (t == typeid(string))
				lua_pushstring(mState, ((string*)val)->c_str());
			else if (t == typeid(float))
				lua_pushnumber(mState, *((float*)val));
			else if (t == typeid(double))
				lua_pushnumber(mState, *((double*)val));
			else if (t == typeid(long))
				lua_pushnumber(mState, *((long*)val));
			else if (t == typeid(int))
				lua_pushinteger(mState, *((int*)val));
			else if (t == typeid(bool))
				lua_pushboolean(mState, *((bool*)val));
			else if (eng->isClassRegistered(t))
			{
				const ScriptClassBase *sclass = eng->getClass(t); 
			
				void **op = reinterpret_cast<void**>(val);

				lua_boxpointer(mState, *op);
				luaL_getmetatable(mState, sclass->getName().c_str());
				lua_setmetatable(mState, -2);
			}
		}

		void LuaScriptFunctionContext::raiseError(const string error)
		{
			lua_pushstring(mState, error.c_str());
			lua_error(mState);
		}

		LuaScriptEngine::LuaScriptEngine()
		{
			mState = lua_open();
			luaopen_base(mState);
			luaopen_table(mState);
			//luaopen_io(state);
			luaopen_string(mState);
			luaopen_math(mState);
			luaopen_debug(mState);

			lua_register(mState, "_ALERT", &LuaScriptEngine::onError);
		}

		LuaScriptEngine::~LuaScriptEngine()
		{
			lua_close(mState); 

			for (uint i = 0; i < mScriptClasses.size(); i++)
				delete mScriptClasses[i];
			for (uint i = 0; i < mScriptFunctions.size(); i++)
				delete mScriptFunctions[i];

			mScriptClasses.clear();
			mScriptClassLookups.clear();

			mScriptFunctions.clear();
			mScriptFunctionLookups.clear();
		}

		void LuaScriptEngine::registerFunction(ScriptFunctionBase *scriptFunction)
		{
			if (mScriptFunctionLookups.find(scriptFunction->getName()) != mScriptFunctionLookups.end())
				return;

			lua_pushinteger(mState, reinterpret_cast<int>(scriptFunction));
			lua_pushcclosure(mState, &LuaScriptEngine::executeFunction, 1);
			lua_setglobal(mState, scriptFunction->getName().c_str());

			mScriptFunctions.push_back(scriptFunction);
			mScriptFunctionLookups.insert(ScriptFunctionLookupPair(scriptFunction->getName(), mScriptFunctions.size()-1));
		}

		void LuaScriptEngine::registerClass(ScriptClassBase *scriptClass)
		{
			if (mScriptClassLookups.find(scriptClass->getName()) != mScriptClassLookups.end())
				return;

			int isc = reinterpret_cast<int>(scriptClass);

			lua_newtable(mState);                 
			int methodtable = lua_gettop(mState);
			luaL_newmetatable(mState, scriptClass->getName().c_str()); 
			int metatable = lua_gettop(mState);

			lua_pushliteral(mState, "__metatable");
			lua_pushvalue(mState, methodtable);
			lua_settable(mState, metatable);

			lua_pushliteral(mState, "__index");
			lua_pushvalue(mState, methodtable);
			lua_settable(mState, metatable);

			lua_pushliteral(mState, "__gc");
			lua_pushinteger(mState, isc);
			lua_pushcclosure(mState, &LuaScriptEngine::delClass, 1);
			lua_settable(mState, metatable);

			lua_pop(mState, 1);
		
			//The messy bit
			const char *lname = scriptClass->getName().c_str();
			int size = scriptClass->numFuncs();

			luaL_findtable(mState, LUA_REGISTRYINDEX, "_LOADED", 1);
			lua_getfield(mState, -1, lname);
			if (!lua_istable(mState, -1)) 
			{
				lua_pop(mState, 1);
				if (luaL_findtable(mState, LUA_GLOBALSINDEX, lname, size))
					luaL_error(mState, "name conflict for module " LUA_QS, lname);
				lua_pushvalue(mState, -1);
				lua_setfield(mState, -3, lname);
			}

			lua_remove(mState, -2);

			for (int i = 0; i < size; i++)
			{
				ScriptClassBase::Func f = scriptClass->getFunc(i);
				lua_pushinteger(mState, reinterpret_cast<int>(f.mFunc));
				lua_pushcclosure(mState, &LuaScriptEngine::executeClassFunction, 1);
				lua_setfield(mState, -3, f.mName.c_str());
			}
			//End: The messy bit

			lua_pop(mState, 1);

			lua_pushinteger(mState, isc);
			lua_pushcclosure(mState, &LuaScriptEngine::createClass, 1);
			lua_setglobal(mState, scriptClass->getName().c_str());

			mScriptClasses.push_back(scriptClass);
			mScriptClassLookups.insert(ScriptClassLookupPair(scriptClass->getName(), mScriptClasses.size()-1));
		}

		void LuaScriptEngine::run(const string &text)
		{
			int res = luaL_dostring(mState, text.c_str());
			if (res != 0)
			{
				const char* err = lua_tostring(mState, -1);
				lua_pop(mState, 1);

				//TODO: error callback
			}
		}

		bool LuaScriptEngine::isClassRegistered(const type_info &type)
		{
			for (uint i = 0; i < mScriptClasses.size(); i++)
				if (mScriptClasses[i]->getType() == type) return true;

			return false;
		}

		const ScriptClassBase *LuaScriptEngine::getClass(const type_info &type)
		{
			for (uint i = 0; i < mScriptClasses.size(); i++)
				if (mScriptClasses[i]->getType() == type) return mScriptClasses[i];

			return 0;
		}

		int LuaScriptEngine::executeFunction(lua_State *state)
		{
			const ScriptFunctionBase* func = reinterpret_cast<ScriptFunctionBase*>(lua_tointeger(state, lua_upvalueindex(1)));

			LuaScriptFunctionContext *context = new LuaScriptFunctionContext(state);
			runFunc(func, context);
			delete context;

			return 1; //TODO: should return number of return vals pushed onto the lua stack
		}

		int LuaScriptEngine::createClass(lua_State *state)
		{
			const ScriptClassBase* sclass = reinterpret_cast<ScriptClassBase*>(lua_tointeger(state, lua_upvalueindex(1)));
			void *obj = sclass->create();
		
			lua_boxpointer(state, obj);
			luaL_getmetatable(state, sclass->getName().c_str());
			lua_setmetatable(state, -2);

			return 1;
		}

		int LuaScriptEngine::delClass(lua_State *state)
		{
			const ScriptClassBase* sclass = reinterpret_cast<ScriptClassBase*>(lua_tointeger(state, lua_upvalueindex(1)));
			ScriptFunctionContext *context = new LuaScriptFunctionContext(state, true);
			sclass->del(context);
			delete context;

			return 1;
		}

		int LuaScriptEngine::executeClassFunction(lua_State *state)
		{
			ScriptClassBase::sfunc f = reinterpret_cast<ScriptClassBase::sfunc>(lua_tointeger(state, lua_upvalueindex(1)));
			ScriptFunctionContext *context = new LuaScriptFunctionContext(state, true);
			f(context);
			delete context;

			return 1;
		}

		int LuaScriptEngine::onError(lua_State *state)
		{
			const char* msg = lua_tostring(state, -1);
			lua_pop(state, 1);

			//TODO: error callback

			return 1;
		}
	}
}