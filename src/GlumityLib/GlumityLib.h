#ifndef GLUMITYLIB_H
#define GLUMITYLIB_H

#include <windows.h>
#include <stdio.h>
#include <MinHook.h>

#define GLUMITYV2_DUMPER_MODULE "001GlumityV2IL2CPPDumper"
#define GLUMITYV2_DUMPER_DLL "001GlumityV2IL2CPPDumper.dll"

#define GLUMITYV2_MODULE "GlumityToolSuite2"
#define GLUMITYV2_DLL "GlumityToolSuite2.dll"

#ifdef __cplusplus
extern "C"
{
#endif

#include "../MainLoader/PluginLoader.h"
#include "../MainLoader/Plugin.h"

    /// @brief Printf for glumity plugin specific code, uses malloc and free for a temp char*, caller doesn't need to manage
    /// @param fmt
    /// @param
    void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...);

    /// @brief entry point of plugins, passes a handle of glumity's loaded main dll (the loader)
    typedef void (*GlumityPlugin_EntryPoint)();

#define GLUMITYV2_PLUGIN_ENTRY EXPORT void GlumityMain()

/// @brief exit point of plugins, get's called on application termination/loader termination
#define GLUMITYV2_PLUGIN_EXIT EXPORT void GlumityExit()

    typedef void (*GlumityPlugin_ExitPoint)();

    // Exports of MainLoader
    typedef GlumityPluginLoader *(*GetLoaderInstance_t)();
    typedef struct
    {
        GetLoaderInstance_t GetLoaderInstance;
    } GlumityV2Exports;

#define INIT_GLUMITYV2_EXPORT(mod, exportName, exportType) \
    (exportType) GetProcAddress(mod, exportName);

    void GlumityV2Exports_Init(GlumityV2Exports *exports);

    typedef void *(*GlumityV2Dumper_GetFunctionPointer_t)(const char *, const char *);
    typedef void (*GlumityV2Dumper_WaitForDumper_t)();
    typedef struct
    {
        GlumityV2Dumper_GetFunctionPointer_t GlumityV2Dumper_GetFunctionPointer;
        GlumityV2Dumper_WaitForDumper_t GlumityV2Dumper_WaitForDumper;
    } GlumityV2DumperExports;

    // Only works if dumper plugin is loaded
    void GlumityV2DumperExports_Init(GlumityV2DumperExports *dumperExports);

#define GLUMITYV2_INIT_HOOKING(pluginName, onFail)                           \
    if (MH_Initialize() != MH_OK)                                            \
    {                                                                        \
        GlumityPlugin_printf("Failed to initialize Minhook!\n", pluginName); \
        onFail;                                                              \
    }                                                                        \
    else

#define GLUMITYV2_GAME_HOOK_CREATE(name, original, hook)  \
    {                                                     \
        MH_STATUS stat = MH_CreateHook(                   \
            (LPVOID *)(original),                         \
            &hook,                                        \
            (LPVOID *)&original);                         \
        if (stat)                                         \
            Glumity_printf("Created a hook: %s\n", name); \
    }

#define GLUMITYV2_GAME_HOOK_ENABLE(name, original)        \
    {                                                     \
        MH_STATUS stat = MH_EnableHook((LPVOID)original); \
        if (stat)                                         \
            Glumity_printf("Enabled a hook: %s\n", name); \
    }

// Recommended to use this to run most code,
// specially function hooking since you have to rely on the dumper being loaded
#define GLUMITYV2_PLUGIN_THREADRUN(function, pParam)                                     \
    HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)function, pParam, 0, 0); \
    if (hThread)                                                                         \
        CloseHandle(hThread);

#define GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports)   \
    if (!dumperExports.GlumityV2Dumper_WaitForDumper)  \
    {                                                  \
        GlumityV2DumperExports_Init(&dumperExports);   \
    }                                                  \
    if (dumperExports.GlumityV2Dumper_WaitForDumper)   \
    {                                                  \
        dumperExports.GlumityV2Dumper_WaitForDumper(); \
    }

#define GLUMITYV2_DUMPER_GET_GAME_FUNCTION(className, functionName, dumperExports) \
    dumperExports.GlumityV2Dumper_GetFunctionPointer(className, functionName);

#ifdef __cplusplus
}
#endif

#endif