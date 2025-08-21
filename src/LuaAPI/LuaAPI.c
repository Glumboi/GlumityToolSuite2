#include "LuaAPI.h"

GlumityLuaLoader g_luaLoader = {0};

DWORD __stdcall LuaThreadFunc(LPVOID param)
{
    LuaThreadData *data = (LuaThreadData *)param;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    GlumityLuaLoader_InitScriptAPI(L);

    if (luaL_dofile(L, data->filename) != LUA_OK)
    {
        const char *err = lua_tostring(L, -1);
        GlumityPlugin_printf("Failed to run file: %s\nError: %s\n", PRINT_HEAD, data->filename, err ? err : "Unknown error");
        lua_pop(L, 1);
    }

    lua_close(L);
    free(data); // free the allocated data
    return 0;
}

void GlumityLuaLoader_Init()
{
    g_luaLoader.state = luaL_newstate();
    if (!g_luaLoader.state)
        return;
    luaL_openlibs(g_luaLoader.state); // Load Lua standard libraries

    GlumityPlugin_printf("Initialized Lua!\n", PRINT_HEAD);

    GlumityV2DumperExports_Init(&g_luaLoader.dumperExports);

    g_luaLoader.dumperExports.GlumityV2Dumper_WaitForDumper();
    GlumityPlugin_printf("Dumper initialized!\n", PRINT_HEAD);
}

void GlumityLuaLoader_LoadScripts()
{
    int outCount = 0;
    char scriptsDir[DLL_PATH_MAX];
    sprintf(scriptsDir, "%s\\%s", PLUGINS_PATH, SCRIPTS_DIRNAME);
    GlumityPlugin_printf("Scripts directory: %s\n", PRINT_HEAD, scriptsDir);
    CreateDirectoryA(scriptsDir, NULL);
    char **files = Glumity_FileSystem_GetAllFilesWithExtensionFromDirectory(scriptsDir, ".lua", &outCount);

    if (!files)
    {
        GlumityPlugin_printf("Could not load scripts directory!\n", PRINT_HEAD);
        return;
    }

    for (size_t i = 0; i < outCount; i++)
    {
        if (luaL_dofile(g_luaLoader.state, files[i]) != LUA_OK)
        {
            const char *err = lua_tostring(g_luaLoader.state, -1);
            GlumityPlugin_printf("Failed to run file: %s\nError: %s\n", PRINT_HEAD, files[i], err ? err : "Unknown error");
            lua_pop(g_luaLoader.state, 1);
        }
    }

    free(files);
    files = NULL;
}

void GlumityLuaLoader_LoadScripts_MT()
{
    int outCount = 0;
    char scriptsDir[DLL_PATH_MAX];
    sprintf(scriptsDir, "%s\\%s", PLUGINS_PATH, SCRIPTS_DIRNAME);
    GlumityPlugin_printf("Scripts directory: %s\n", PRINT_HEAD, scriptsDir);
    CreateDirectoryA(scriptsDir, NULL);
    char **files = Glumity_FileSystem_GetAllFilesWithExtensionFromDirectory(scriptsDir, ".lua", &outCount);

    if (!files)
    {
        GlumityPlugin_printf("Could not load scripts directory!\n", PRINT_HEAD);
        return;
    }

    HANDLE *threads = malloc(sizeof(HANDLE) * outCount);

    for (int i = 0; i < outCount; i++)
    {
        LuaThreadData *data = malloc(sizeof(LuaThreadData));
        data->filename = files[i]; // thread owns this pointer

        threads[i] = CreateThread(NULL, 0, LuaThreadFunc, data, 0, NULL);
        if (!threads[i])
        {
            GlumityPlugin_printf("Failed to create thread for %s\n", PRINT_HEAD, files[i]);
            free(data);
        }
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(outCount, threads, TRUE, INFINITE);
    free(threads);
    free(files);
}

void GlumityLuaLoader_Destroy()
{
    lua_close(g_luaLoader.state);
    GlumityPlugin_printf("Closed Lua!\n", PRINT_HEAD);
}

void GlumityLuaLoader_InitScriptAPI(lua_State *L)
{
    LUA_EXPORT(glumity_print, L);
    LUA_EXPORT(glumity_getFunctionPtr, L);
    LUA_EXPORT(glumity_waitForDumper, L);
}

int glumity_waitForDumper(lua_State *L)
{
    g_luaLoader.dumperExports.GlumityV2Dumper_WaitForDumper();
    return 0;
}

int glumity_print(lua_State *L)
{
    int nargs = lua_gettop(L); // total number of arguments passed
    if (nargs < 1)
        return luaL_error(L, "glumity_print requires at least a format string");

    const char *fmt = luaL_checkstring(L, 1);

    const char *header = "GlumityV2LuaAPI";
    if (nargs >= 2 && lua_isstring(L, 2))
        header = lua_tostring(L, 2);

    // Call string.format(fmt, ...) in Lua
    lua_getglobal(L, "string");    // push string table
    lua_getfield(L, -1, "format"); // push string.format function
    lua_pushstring(L, fmt);        // push format string

    // push the remaining arguments (skip fmt and header)
    for (int i = 3; i <= nargs; i++)
        lua_pushvalue(L, i);

    int format_args = nargs - 2;                       // number of extra args for string.format
    if (lua_pcall(L, format_args + 1, 1, 0) != LUA_OK) // +1 for fmt itself
    {
        const char *err = lua_tostring(L, -1);
        return luaL_error(L, "string.format error: %s", err);
    }

    const char *msg = lua_tostring(L, -1);
    if (!msg)
        msg = "(null)";

    GlumityPlugin_printf(msg, header);

    lua_pop(L, 2); // pop string.format result and string table

    return 0;
}

int glumity_getFunctionPtr(lua_State *L)
{
    if (!g_luaLoader.dumperExports.GlumityV2Dumper_GetFunctionPointer)
        return 0; // no values returned

    const char *className = lua_tostring(L, 1);
    const char *functionName = luaL_checkstring(L, 2);

    void *ptr = g_luaLoader.dumperExports.GlumityV2Dumper_GetFunctionPointer(className, functionName);
    lua_pushlightuserdata(L, ptr); // push the pointer safely
    return 1;
}
