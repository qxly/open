#include <open/Script.h>
#include <open/Module.h>

extern "C" {

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


void main()
{
	open::ModuleManager::s_manager->loadModule("RenderSystem.dll");
	open::ModuleManager::s_manager->loadModule("Render.dll");
	open::ModuleManager::s_manager->loadModule("Engine.dll");

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	bool err = luaL_dofile(L, "init.lua");

	if (err)
	{
		printf("/n %s /n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	getchar();
}