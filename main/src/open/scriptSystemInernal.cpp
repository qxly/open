#include "scriptInternal.h"

namespace open
{
	namespace Script
	{
		int ScriptSystem::ScriptSystemInternal::errorHandler(lua_State *L)
		{
			lua_Debug ar;

			memset(&ar, 0, sizeof(lua_Debug));
			const char *sErr = lua_tostring(L, 1);
			if (sErr)
				printf("[Lua Error] %s", sErr);

			int level = 1;
			while (lua_getstack(L, level++, &ar))
			{
				lua_getinfo(L, "lnS", &ar);
				if (ar.name)
					printf("$6    > %s, (%s: %d)", ar.name, ar.short_src, ar.currentline);
				else
					printf("$6    > (null) (%s: %d)", ar.short_src, ar.currentline);
			}
			if (sErr)
			{
				printf("![Lua Error] %s", sErr);
			}
			return 0;
		}

		ScriptSystem::ScriptSystemInternal::ScriptSystemInternal()
		{
			L = luaL_newstate();
			luaL_openlibs(L);

			lua_pushcfunction(L, ScriptSystemInternal::errorHandler);
			_errorHandler = (void*)lua_ref(L, 1);
		}

		ScriptSystem::ScriptSystemInternal::~ScriptSystemInternal()
		{
		}

		bool ScriptSystem::ScriptSystemInternal::executeBuffer(const char *sBuffer)
		{
			int status;
			{
				status = luaL_loadbuffer(L, sBuffer, strlen(sBuffer), "");
			}

			if (status == 0)
			{ 
				int base = lua_gettop(L);  // function index.
				lua_getref(L, (int)(INT_PTR)_errorHandler);
				lua_insert(L, base);  // put it under chunk and args

				status = lua_pcall(L, 0, LUA_MULTRET, base);  // call main
				lua_remove(L, base);  // remove error handler function.
			}
			if (status != 0)
			{
				const char *sErr = lua_tostring(L, -1);
				printf("[Lua Error] Error executing lua %s", sErr);
				lua_pop(L, 1);
				return false;
			}
			return true;
		}

		void ScriptSystem::ScriptSystemInternal::pushValue(const ScriptValue & var)
		{
			switch (var.type)
			{
			case ScriptValue::VALUETYPE_NULL:
				lua_pushnil(L);
				break;;
			case ScriptValue::VALUETYPE_BOOL:
				lua_pushboolean(L, var.value.bool_value);
				break;;
			case ScriptValue::VALUETYPE_PTR:
				lua_pushlightuserdata(L,var.value.ptr_value);
				break;
			case ScriptValue::VALUETYPE_FLOAT:
				lua_pushnumber(L, var.value.float_value);
				break;
			case ScriptValue::VALUETYPE_STR:
				lua_pushstring(L, var.value.str_value);
				break;
			case ScriptValue::VALUETYPE_TABLE:
				if (var.value.table_value)
					var.value.table_value->get()->pushRef();
				else
					lua_pushnil(L);
				break;
			case ScriptValue::VALUETYPE_FUN:
				if (var.value.fun_value)
					var.value.fun_value->get()->pushRef();
				assert(lua_type(L, -1) == LUA_TFUNCTION);
				break;
			default:
				assert(0);
			}
		}

		bool ScriptSystem::ScriptSystemInternal::toAny(ScriptValue &var, int index)
		{
			if (!lua_gettop(L))
				return false;

			switch (lua_type(L, index))
			{
			case LUA_TNIL:
				var.type = ScriptValue::VALUETYPE_NULL;
				break;
			case LUA_TBOOLEAN:
				var.set(lua_toboolean(L, index) != 0);
				break;
			case LUA_TLIGHTUSERDATA:
				var.set(const_cast<void*>(lua_topointer(L, index)));
				break;
			case LUA_TNUMBER:
				var.set((float)lua_tonumber(L, index));
				break;
			case LUA_TSTRING:
				var.set(lua_tostring(L, index));
				break;
			case LUA_TTABLE:
				if (var.type != ScriptValue::VALUETYPE_TABLE || !var.value.table_value)
				{
					ScriptTable* table = new ScriptTable(true);
					var.set(table);
				}
				lua_pushvalue(L, index);
				var.value.table_value->get()->attach();
				break;
			case LUA_TFUNCTION:
			{
				if (var.type != ScriptValue::VALUETYPE_FUN || !var.value.fun_value)
				{
					ScriptFunHandle* fun = new ScriptFunHandle();
					var.set(fun);
				}
				lua_pushvalue(L, index);
				var.value.fun_value->get()->attach();
			}
			break;
			case LUA_TTHREAD:
			default:
				return false;
			}
			return true;
		}

		bool ScriptSystem::ScriptSystemInternal::popAny(ScriptValue & var)
		{
			bool res = toAny(var, -1);
			lua_pop(L, 1);
			return res;
		}

		bool ScriptSystem::ScriptSystemInternal::beginCall(ScriptFunHandle* handle)
		{
			assert(handle != 0);
			if (!handle)
				return false;

			handle->get()->pushRef();
			if (!lua_isfunction(L, -1))
			{
				_argNum = -1;

				printf("[ScriptSystem::beginCall] Function Ptr:%d not found", (size_t)handle->get()->_ref);
				return false;
			}
			_argNum = 0;
			return true;
		}

		int ScriptSystem::ScriptSystemInternal::beginCall(const char *funName)
		{
			lua_getglobal(L, funName);
			_argNum = 0;

			if (!lua_isfunction(L, -1))
			{
				printf("[ScriptSystem::beginCall] Function %s not found(check for syntax errors or if the file wasn't loaded)", funName);
				_argNum = -1;
				return 0;
			}
			return 1;
		}

		bool ScriptSystem::ScriptSystemInternal::beginCall(ScriptTable * table, const char * funName)
		{
			ScriptValue value(table);
			pushValue(value);

			lua_pushstring(L, funName);
			lua_gettable(L, -2);
			lua_remove(L, -2);

			if (!lua_isfunction(L, -1))
			{
				_argNum = -1;
				printf("[ScriptSystem::beginCall] Function %s not found in the table", funName);
				return 0;
			}

			_argNum = 0;
			return 1;
		}

		bool ScriptSystem::ScriptSystemInternal::endCallN(int returns)
		{
			if (_argNum < 0 || !L)
				return false;

			int base = lua_gettop(L) - _argNum;
			lua_getref(L, (LUA_INTEGER)_errorHandler);
			lua_insert(L, base);

			int status = lua_pcall(L, _argNum, returns, base);
			lua_remove(L, base);

			return status == 0;
		}

		void ScriptSystem::ScriptSystemInternal::pushFuncParamAny(const ScriptValue &any)
		{
			if (_argNum == -1)
				return;
			pushValue(any);
			_argNum++;
		}

		bool ScriptSystem::ScriptSystemInternal::getGlobalValue(const char * key, ScriptValue & any)
		{
			int type = lua_getglobal(L, key);
			int a = lua_gettop(L);

			if (!popAny(any))
			{
				return false;
			}
			return true;
		}
	}
}