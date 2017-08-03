#ifndef ENGINE_FACTORYINTERFACE_H
#define ENGINE_FACTORYINTERFACE_H 1

#include <open/openDef.h>

namespace open
{
	class IEngine;
	class COMMON_API EngineFactory
	{
	public:
		static EngineFactory* s_factory;

		virtual IEngine* getEngine() = 0;
	};
}

#endif