#ifndef OPEN_SCRIPT_H
#define OPEN_SCRIPT_H

#include <open/Memory.h>
#include <open/Ptr.h>

namespace open
{
	namespace Script
	{
		class ScriptTable;
		class ScripPtr {};
		class ScriptFunHandle;

		struct ScriptValue
		{
			enum ValueType
			{
				VALUETYPE_BOOL,
				VALUETYPE_INT,
				VALUETYPE_FLOAT,
				VALUETYPE_DOUBLE,
				VALUETYPE_STR,
				VALUETYPE_PTR,
				VALUETYPE_FUN,
				VALUETYPE_TABLE,
				VALUETYPE_NULL
			} type = VALUETYPE_NULL;

			union
			{
				bool bool_value;
				int32 int_value;
				float float_value;
				double double_value;
				const char* str_value;
				void* ptr_value;
				ScriptTable* table_value;
				ScriptFunHandle* fun_value;
			} value;

			ScriptValue();

			template<typename T>
			ScriptValue(T t);

			template<typename T>
			void set(T v);

			template<typename T>
			T get() const;

			bool operator == (const ScriptValue& r) const;

			void swap(ScriptValue& value);

			bool isNull() const;

			static const ScriptValue s_nullSrciptValue;
		};

		class COMMON_API ScriptSystem
		{
			class ScriptSystemInternal;
		public:

			static ScriptSystem& instance();

			bool executeBuffer(const char* buffer);

			bool executeFile(const char * file);

			bool getGlobalValue(const char* key, ScriptValue& t);

			template<typename Ret, typename ...Types>
			Ret call(ScriptFunHandle* handle, Types... args);

			template<typename ...Types>
			void call(ScriptFunHandle* handle, Types... args);

			template<typename Ret, typename ...Types>
			Ret call(ScriptTable* pTable, const char* funName, Types... args);

			template<typename ...Types>
			void call(ScriptTable* pTable, const char* funName, Types... args);

			ScriptSystemInternal* get();

		private:

			ScriptSystem();

			bool beginCall(ScriptFunHandle* handle);

			bool beginCall(ScriptTable* pTable, const char* funName);

			bool endCall();

			bool endCall(ScriptValue& value);

			void pushParma(const ScriptValue& value);

			template<typename Type, typename ...Types>
			void pushParma(ScriptValue& value, Type arg, Types... args);

			template<typename Type>
			void pushParma(ScriptValue& value, Type arg);

		private:
			ScriptSystemInternal* _internalPtr;
		};

		class COMMON_API ScriptFunHandle : public TPtr<ScripPtr>
		{
			class ScriptFunHandleInternal;
		public:
			ScriptFunHandle();

			~ScriptFunHandle();

			ScriptFunHandleInternal* get();

		private:
			ScriptFunHandleInternal* _internalPtr;
		};

		class COMMON_API ScriptTable : public TPtr<ScripPtr>
		{
			class ScriptTableInternal;
			friend class ScriptSystem;

		public:
			struct Iterator
			{
				const char* key = nullptr;
				int numKey = -1;
				ScriptValue value;
				int internal = -1;
			};
		public:

			ScriptTable(bool empty = false);

			~ScriptTable();

			int getCount();

			bool nextIterator(Iterator& it);

			void setValue(const char *key, const ScriptValue& value);

			bool getValue(const char *key, ScriptValue& any);

			void setValue(int32 index, const ScriptValue& value);

			bool getValue(int32 index, ScriptValue& any);

			bool haveValue(const char* key);

			void setToNull(const char *key);

			ScriptValue::ValueType getValueType(const char *key);

			ScriptTableInternal* get();

		private:
			ScriptTableInternal* _internalPtr;
		};

		inline ScriptValue::ScriptValue()
		{
			type = VALUETYPE_NULL;
			value.ptr_value = nullptr;
		}

		inline bool ScriptValue::operator==(const ScriptValue & r) const
		{
			if (type != r.type)
				return false;
			return value.ptr_value == r.value.ptr_value;
		}

		template<typename T>
		inline ScriptValue::ScriptValue(T t)
		{
			set(t);
		}

		template<>
		inline void ScriptValue::set(bool v) {
			type = VALUETYPE_BOOL; value.bool_value = v;
		}

		template<>
		inline void ScriptValue::set(int32 v) {
			type = VALUETYPE_INT; value.int_value = v;
		}

		template<>
		inline void ScriptValue::set(float v) {
			type = VALUETYPE_FLOAT;value.float_value = v;
		}

		template<>
		inline void ScriptValue::set(double v) {
			type = VALUETYPE_DOUBLE; value.double_value = v;
		}

		template<>
		inline void ScriptValue::set(const char * v) {
			type = VALUETYPE_STR; value.str_value = v;
		}

		template<>
		inline void ScriptValue::set(void* v) {
			type = VALUETYPE_PTR; value.ptr_value = (void*)v;
		}

		template<>
		inline void ScriptValue::set(ScriptFunHandle* v) {
			type = VALUETYPE_FUN; value.fun_value = v;
		}

		template<>
		inline void ScriptValue::set(ScriptTable* v) {
			type = VALUETYPE_TABLE; value.table_value = v;
		}

		template<typename T>
		void ScriptValue::set(T v) {
			type = VALUETYPE_PTR; value.ptr_value = (void*)v;
		}

		template<>
		inline bool ScriptValue::get() const {
			return value.bool_value;
		}

		template<>
		inline int ScriptValue::get() const {
			return value.int_value;
		}

		template<>
		inline float ScriptValue::get() const {
			return value.float_value;
		}

		template<>
		inline double ScriptValue::get() const {
			return value.double_value;
		}

		template<>
		inline const char*  ScriptValue::get() const {
			return value.str_value;
		}

		template<>
		inline void* ScriptValue::get() const {
			return value.ptr_value;
		}

		template<>
		inline ScriptFunHandle* ScriptValue::get() const {
			return value.fun_value;
		}

		template<>
		inline ScriptTable* ScriptValue::get() const {
			return value.table_value;
		}

		template<typename T>
		inline T ScriptValue::get() const {
			return value.ptr_value;
		}

		inline void ScriptValue::swap(ScriptValue& value)
		{
			char temp[sizeof(ScriptValue)];
			memcpy(temp, this, sizeof(ScriptValue));
			memcpy(this, &value, sizeof(ScriptValue));
			memcpy(&value, temp, sizeof(ScriptValue));
		}

		inline bool ScriptValue::isNull() const
		{
			return type == VALUETYPE_NULL;
		}

		inline ScriptSystem::ScriptSystemInternal* ScriptSystem::get()
		{
			return _internalPtr;
		}

		template<typename Type, typename ...Types>
		void ScriptSystem::pushParma(ScriptValue& value, Type arg, Types... args)
		{
			pushParma(value, arg);
			pushParma(value, arg, args...);
		}

		template<typename Type>
		inline void ScriptSystem::pushParma(ScriptValue& value, Type arg)
		{
			value.set<Type>(arg);
			pushParma(value);
		}

		template<typename Ret, typename ...Types>
		Ret ScriptSystem::call(ScriptFunHandle* handle, Types... args)
		{
			ScriptValue value;
			beginCall(handle);
			pushParma(value, args...);

			endCall(value);
			return value.get<Ret>();
		}

		template<typename ...Types>
		void ScriptSystem::call(ScriptFunHandle* handle, Types... args)
		{
			ScriptValue value;
			beginCall(handle);
			pushParma(value, args...);

			endCall();
		}

		template<typename Ret, typename ...Types>
		Ret ScriptSystem::call(ScriptTable* pTable, const char* funName, Types... args)
		{
			ScriptValue value;
			beginCall(pTable, funName);
			pushParma(value, args...);

			endCall(value);
			return value.get<Ret>();
		}

		inline ScriptTable::ScriptTableInternal* ScriptTable::get()
		{
			return _internalPtr;
		}
	}
}

#endif