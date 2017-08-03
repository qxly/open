#ifndef OPEN_SCRIPTINTERNAL_H
#define OPEN_SCRIPTINTERNAL_H

#include <open/Script.h>
#include <assert.h>

extern "C" {

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

#define lua_ref(L,lock) ((lock) ? luaL_ref(L, LUA_REGISTRYINDEX) : \
      (lua_pushstring(L, "unlocked references are obsolete"), lua_error(L), 0))

#define lua_unref(L,ref)        luaL_unref(L, LUA_REGISTRYINDEX, (ref))

#define lua_getref(L,ref)       lua_rawgeti(L, LUA_REGISTRYINDEX, (ref))

namespace open
{
	namespace Script
	{
		class ScriptSystem::ScriptSystemInternal
		{
		public:

			ScriptSystemInternal();

			~ScriptSystemInternal();

			bool executeBuffer(const char* buffer);

			inline lua_State* get() { return L; }

			void pushValue(const ScriptValue& value);

			bool toAny(ScriptValue & var, int index);

			bool popAny(ScriptValue & var);

			bool beginCall(ScriptFunHandle* handle);

			int beginCall(const char * funName);

			bool beginCall(ScriptTable * table, const char * funName);

			bool endCallN(int returns);

			void pushFuncParamAny(const ScriptValue & any);

			bool getGlobalValue(const char* key, ScriptValue& t);

		private:

			static int errorHandler(lua_State *L);

		private:
			lua_State *L;
			int _argNum;

			void* _errorHandler;
		};

		struct ScripPtrRef
		{
			enum
			{
				DELETED_REF = -1,
				NULL_REF = 0,
			};

			void pushRef();

			void attach();

			virtual ~ScripPtrRef();

			static void setup(ScriptSystem* sys, lua_State* s)
			{
				s_system = sys;
				L = s;
			}

			size_t _ref = NULL_REF;

			static ScriptSystem* s_system;

			static lua_State* L;
		};

		class ScriptFunHandle::ScriptFunHandleInternal : public ScripPtrRef
		{
		};

		class ScriptTable::ScriptTableInternal : public ScripPtrRef
		{
		public:
			ScriptTableInternal(bool empty);

			void setValue(const char *key, const ScriptValue& value);

			bool getValue(const char * key, ScriptValue& any);

			void setValue(int32 index, const ScriptValue& value);

			bool getValue(int32 index, ScriptValue& any);

			bool haveValue(const char* key);

			void setToNull(const char *key);

			ScriptValue::ValueType getValueType(const char *key);

			int getCount();

			bool nextIterator(ScriptTable::Iterator& it);
		};
	}
}

#endif