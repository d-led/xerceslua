#include <lua.hpp>

#include "xerceslua_lib.h"

#ifdef _MSC_VER
#define XERCESLUA __declspec(dllexport)
#else
#define XERCESLUA
#endif

extern "C" XERCESLUA int luaopen_xerceslua (lua_State* L) {
	register_xerceslua(L);
	return 1;
}