#include "scriptInternal.h"
#include <assert.h>

namespace open
{
	namespace Script
	{

		ScriptSystem* ScripPtrRef::s_system = nullptr;
		lua_State* ScripPtrRef::L = nullptr;

		void ScripPtrRef::pushRef()
		{
			if (_ref != DELETED_REF && _ref != NULL_REF)
				lua_getref(L, _ref);
			else
			{
				lua_pushnil(L);
				if (_ref == DELETED_REF)
				{
					assert(0 && "Access to deleted script object");
				}
				else
				{
					assert(0 && "Pushing Nil table reference");
				}
			}
		}

		void ScripPtrRef::attach()
		{
			if (_ref != NULL_REF)
				lua_unref(L, _ref);
			_ref = lua_ref(L, 1);
		}

		ScripPtrRef::~ScripPtrRef()
		{
			if (_ref == DELETED_REF)
			{
				assert(0 && "Attempt to release already released script table.");
			}

			if (_ref != NULL_REF)
				lua_unref(L, _ref);
			_ref = DELETED_REF;
		}


		ScriptTable::ScriptTableInternal::ScriptTableInternal(bool empty)
		{
			if (!empty)
			{
				lua_newtable(L);
				attach();
			}
		}

		void ScriptTable::ScriptTableInternal::setValue(const char * key, const ScriptValue & value)
		{
			int top = lua_gettop(L);
			ScriptValue oldValue;
			if (lua_getmetatable(L, -1))
			{
				lua_pop(L, 1);
				if (getValue(key, oldValue) && oldValue == value)
					return;
			}

			pushRef();
			size_t len = strlen(key);
			lua_pushlstring(L, key, len);

			s_system->get()->pushValue(value);
			lua_settable(L, -3);
			lua_settop(L, top);
		}

		bool ScriptTable::ScriptTableInternal::getValue(const char * key, ScriptValue & any)
		{
			int top = lua_gettop(L);
			pushRef();

			lua_pushstring(L, key);

			lua_gettable(L, -2);
			bool res = s_system->get()->popAny(any);

			lua_settop(L, top);

			return res;
		}

		void ScriptTable::ScriptTableInternal::setValue(int32 index, const ScriptValue& value)
		{
			pushRef();
			s_system->get()->pushValue(value);
			lua_rawseti(L, -2, index);
			lua_pop(L, 1);
		}

		bool ScriptTable::ScriptTableInternal::getValue(int32 index, ScriptValue& any)
		{
			bool res = false;
			pushRef();
			lua_rawgeti(L, -1, index);
			res = s_system->get()->popAny(any);
			lua_pop(L, 1);

			return res;
		}

		bool ScriptTable::ScriptTableInternal::haveValue(const char * key)
		{
			ScriptValue any;
			getValue(key, any);

			return !any.isNull();
		}

		void ScriptTable::ScriptTableInternal::setToNull(const char * key)
		{
			setValue(key, ScriptValue::s_nullSrciptValue);
		}

		ScriptValue::ValueType ScriptTable::ScriptTableInternal::getValueType(const char * key)
		{
			ScriptValue::ValueType type = ScriptValue::VALUETYPE_NULL;

			pushRef();
			lua_pushstring(L, key);
			lua_gettable(L, -2);

			int luatype = lua_type(L, -1);
			switch (luatype)
			{
			case LUA_TNIL:
				type = ScriptValue::VALUETYPE_NULL;
				break;
			case LUA_TBOOLEAN:
				type = ScriptValue::VALUETYPE_BOOL;
				break;
			case LUA_TNUMBER:
				type = ScriptValue::VALUETYPE_FLOAT;
				break;
			case LUA_TSTRING:
				type = ScriptValue::VALUETYPE_STR;
				break;
			case LUA_TFUNCTION:
				type = ScriptValue::VALUETYPE_FUN;
				break;
			case LUA_TLIGHTUSERDATA:
				type = ScriptValue::VALUETYPE_PTR;
				break;
			case LUA_TTABLE:
				type = ScriptValue::VALUETYPE_TABLE;;
				break;
			}
			lua_pop(L, 2);
			return type;
		}

		int ScriptTable::ScriptTableInternal::getCount()
		{
			int base = lua_gettop(L);

			pushRef();
			lua_len(L, -1);

			int count = lua_tointeger(L, -1);
			lua_settop(L, base);

			return count;
		}

		bool ScriptTable::ScriptTableInternal::nextIterator(ScriptTable::Iterator& it)
		{
			if (it.internal < 0)
			{
				it.internal = lua_gettop(L) + 1;
				pushRef();
				lua_pushnil(L);
			}

			if (!it.internal)
				return false;

			int nTop = it.internal - 1;
			while ((lua_gettop(L) - (nTop + 1))>1)
			{
				lua_pop(L, 1);
			}
			bool bResult = lua_next(L, nTop + 1) != 0;
			if (bResult)
			{
				bResult = s_system->get()->popAny(it.value);
				ScriptValue keyValue;
				s_system->get()->toAny(keyValue, -1);
				if (lua_type(L, -1) == LUA_TSTRING)
				{
					it.key = (const char*)lua_tostring(L, -1);
					it.numKey = -1;
				}
				else if (lua_type(L, -1) == LUA_TNUMBER)
				{
					it.key = nullptr;
					it.numKey = (int)lua_tonumber(L, -1);
				}
				else
				{
					it.key = 0;
					it.numKey = -1;
				}
			}
			if (!bResult)
			{
				if (it.internal)
				{
					lua_settop(L, it.internal - 1);
				}
			}
			return bResult;
		}
	}
}