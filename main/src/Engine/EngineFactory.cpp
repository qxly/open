#include <open/interface/Engine/EngineFactoryInterface.h>

#include "Engine.h"

namespace open
{
	class EngineFactory_Internal : public EngineFactory
	{
	public:
		virtual IEngine* getEngine()
		{
			return &_engine;
		}

		Engine _engine;
	};
	struct Init_EngineFactory_Internal
	{
		Init_EngineFactory_Internal()
		{
			EngineFactory::s_factory = new EngineFactory_Internal();
		}

		~Init_EngineFactory_Internal()
		{
			delete EngineFactory::s_factory;
		}
	};
	static Init_EngineFactory_Internal g_Init_EngineFactory_Internal;
}