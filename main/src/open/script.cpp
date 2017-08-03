#include <open/Script.h>

#include "scriptInternal.h"
#include <assert.h>

namespace open
{
	namespace Script
	{
		ScriptSystem* g_scriptSys = nullptr;

		ScriptSystem& ScriptSystem::instance()
		{
			if (!g_scriptSys)
				g_scriptSys = new ScriptSystem;

			return *g_scriptSys;
		}
		const ScriptValue ScriptValue::s_nullSrciptValue = ScriptValue();
		ScriptSystem::ScriptSystem()
		{
			_internalPtr = new ScriptSystem::ScriptSystemInternal;
			ScripPtrRef::setup(this, _internalPtr->get());
		}

		bool ScriptSystem::executeBuffer(const char * buffer)
		{
			return _internalPtr->executeBuffer(buffer);
		}

		bool ScriptSystem::executeFile(const char * filePath)
		{
			FILE* file = fopen(filePath, "rb");
			if (!file)
			{
				printf("[Lua Error] Failed to load script file %s", filePath);
				return false;
			}

			fseek(file, 0, SEEK_END);
			size_t fileSize = ftell(file);
			fseek(file, 0, SEEK_SET);

			if (!fileSize)
			{
				fclose(file);
				return false;
			}

			char* buffer = new char[fileSize + 1];
			buffer[fileSize] = 0;
			fread(buffer, 1, fileSize, file);
			fclose(file);

			bool state = executeBuffer(buffer);
			delete[] buffer;

			return state;
		}

		bool ScriptSystem::getGlobalValue(const char* key, ScriptValue& t)
		{
			return _internalPtr->getGlobalValue(key, t);
		}

		bool ScriptSystem::beginCall(ScriptFunHandle* handle)
		{
			return _internalPtr->beginCall(handle);
		}

		bool ScriptSystem::beginCall(ScriptTable* pTable, const char* funName)
		{
			return _internalPtr->beginCall(pTable, funName);
		}

		bool ScriptSystem::endCall()
		{
			return _internalPtr->endCallN(0);
		}

		bool ScriptSystem::endCall(ScriptValue & value)
		{
			if (!_internalPtr->endCallN(1))
				return false;

			return _internalPtr->popAny(value);
		}

		void ScriptSystem::pushParma(const ScriptValue & value)
		{
			_internalPtr->pushFuncParamAny(value);
		}

		ScriptFunHandle::ScriptFunHandle()
		{
			_internalPtr = new ScriptFunHandle::ScriptFunHandleInternal;
		}

		ScriptFunHandle::ScriptFunHandleInternal* ScriptFunHandle::get()
		{
			return _internalPtr;
		}

		ScriptFunHandle::~ScriptFunHandle()
		{
			delete _internalPtr;
		}

		//////////////////////////////////////////////
		ScriptTable::ScriptTable(bool empty)
		{
			_internalPtr = new ScriptTableInternal(empty);
		}

		ScriptTable::~ScriptTable()
		{
			delete _internalPtr;
		}

		int ScriptTable::getCount()
		{
			return _internalPtr->getCount();
		}

		bool ScriptTable::nextIterator(ScriptTable::Iterator& it)
		{
			return _internalPtr->nextIterator(it);
		}

		void ScriptTable::setValue(const char * key, const ScriptValue & value)
		{
			_internalPtr->setValue(key, value);
		}

		bool ScriptTable::getValue(const char * key, ScriptValue & any)
		{
			return _internalPtr->getValue(key, any);
		}

		void ScriptTable::setValue(int32 index, const ScriptValue& value)
		{
			_internalPtr->setValue(index, value);
		}

		bool ScriptTable::getValue(int32 index, ScriptValue& any)
		{
			return _internalPtr->getValue(index, any);
		}

		bool ScriptTable::haveValue(const char * key)
		{
			return _internalPtr->haveValue(key);
		}

		void ScriptTable::setToNull(const char * key)
		{
			return _internalPtr->setToNull(key);
		}

		ScriptValue::ValueType ScriptTable::getValueType(const char * key)
		{
			return _internalPtr->getValueType(key);
		}
	}
}