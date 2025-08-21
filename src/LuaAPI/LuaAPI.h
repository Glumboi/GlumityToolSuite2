#ifndef LUAPAI_H
#define LUAPAI_H

#include <GlumityLib.h>
#include <MinHook.h>

#include <stdio.h>
#include <stdbool.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>

#include "defines.h"

#define SCRIPTS_DIRNAME "Scripts"

typedef struct
{
    const char *filename;
} LuaThreadData;

DWORD WINAPI LuaThreadFunc(LPVOID param);

typedef struct
{
    lua_State *state;
    GlumityV2DumperExports dumperExports;
} GlumityLuaLoader;

extern GlumityLuaLoader g_luaLoader;

#define LUA_EXPORT(function, state)                        \
    lua_pushcfunction(state, function); \
    lua_setglobal(state, #function);

void GlumityLuaLoader_Init();
void GlumityLuaLoader_LoadScripts();
void GlumityLuaLoader_LoadScripts_MT();
void GlumityLuaLoader_Destroy();
void GlumityLuaLoader_InitScriptAPI(lua_State* L);

// lua exposed functions
int glumity_waitForDumper(lua_State *L);
int glumity_print(lua_State *L);
int glumity_getFunctionPtr(lua_State *L);

#endif