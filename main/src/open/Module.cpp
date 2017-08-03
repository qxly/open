#include <open/Module.h>
#include <open/Array.h>

namespace open
{
	ModuleManager* ModuleManager::s_manager = new ModuleManager();

#if _WIN32
#include <windows.h>

	ModuleManager::ModuleManager()
	{
	}

	ModuleManager::~ModuleManager()
	{
	}

	void ModuleManager::loadModule(const char* modulePath)
	{
		LoadLibrary(modulePath);
	}

	void ModuleManager::unloadModule(const char* modulePath)
	{

	}

#endif
}
