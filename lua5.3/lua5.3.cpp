// lua5.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lua/lua.h"
#include "lua/lauxlib.h"

//int _tmain(int argc, _TCHAR* argv[])
//{
//        lua_State *L = luaL_newstate();
//        
//        luaL_openlibs(L);
//
//        if (luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0)) {
//                printf("load file occurs error%s\n", lua_tostring(L, -1));
//        }
//
//        lua_close(L);
//
//	return 0;
//}
//
//