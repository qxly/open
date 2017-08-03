#ifndef WYIN_MODULE_H
#define WYIN_MODULE_H 1

#include <open/openDef.h>

namespace open
{
	class COMMON_API ModuleManager
	{
	public:
		static ModuleManager* s_manager;

		void loadModule(const char* modulePath);

		void unloadModule(const char* modulePath);

	protected:
		ModuleManager();

		~ModuleManager();
	};
}

#endif